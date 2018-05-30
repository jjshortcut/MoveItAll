/*
*	Main.c
*	MoveItAll project
*	Author: Jan-Jaap Schuurman @ ProtoSpace 2018
*	Version: 1
*/

/*
TODO:
- Use of break pin of motor driver
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "uart_handler.h"

// Prototypes
void init_io(void);
void init_int(void);
void init_adc(void);

volatile uint8_t check_movement = FALSE;	// Closed loop check
uint16_t print_counter = 0;

ISR (TIMER1_COMPA_vect)  // timer1 compA
{
	static volatile uint16_t current_ovf_counter = 0;
	//LED_TOGGLE;	// alive LED
	
	device.current = read_adc(SENSE_A_PIN);	// current measurement of motor
	device.current_angle = ((read_adc(ANGLE_PIN)-MIN_ANGLE) / ((MAX_ANGLE-MIN_ANGLE)/MAX_DEGREES));	//
	if (device.current_angle > 200)
	{
		device.current_angle = 0;
	}	
	else if(device.current_angle > MAX_DEGREES)
	{
		device.current_angle = MAX_DEGREES;
	}
	
	if (device.current>device.current_limit)
	{
		current_ovf_counter++;		
	}
	else
	{
		current_ovf_counter=0;
	}
	
	if (current_ovf_counter == (CURR_PEAK_OVF_TIME_MS/INTERRUPT_MS))
	{
		device.movementEnabled = FALSE;
		device.status = CURR_OVF;
		device.setpoint_angle=0;
		device.multiply_movement=0;
		current_ovf_counter=0;
	}
	
	check_movement = TRUE;
}

int main(void)
{
	init_io();			/* Init IO*/
	init_int();			/* Init int for resfresh display every x ms	*/
	init_adc();			/* Init ADC */	
	
	LCD_ON;
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );	/* Init Uart */
	sei();						/* Enable global interrupts for uart*/
	//uart_puts("\n\rInit Uart OK\n");
	//uart_puts("ÿÿÿrestÿÿÿ");	// reset HMI

	// Start values
	device.current_limit = DEFAULT_CURRENT_LIMIT;	// set current limit
	device.movementEnabled = FALSE;
	device.min_act_on_error=MIN_ACT_ON_ERROR_DEFAULT;
	device.status=STOP;
	device.setpoint_angle = 0;
	
	
	
	/* Now the device is ready! */
	//uart_puts("MoveItAll hand ready!");		/* Print version number	*/
	//print_float(VERSION,1);

	while(1)
	{	
		get_serial();
		
		if (check_movement)	// triggered bij 10ms int
		{
			if (command_ready) {process_command();}
			p_loop();	// 	do p loop stuff
			check_movement = FALSE;
		}
		
		//if (print_counter == REFRESH_LOOP_MS)
		{
			#ifdef DEBUG_MSG
				print_values();
			#else
				print_HMI();
			#endif
		
			print_counter = 0;
		}
		//else {print_counter++;}
	}
}

void init_io(void)
{
	// Motor pins
	DIR_A_PIN_DDR |= (1<<DIR_A_PIN);		// Output
	PWM_A_PIN_DDR |= (1<<PWM_A_PIN);		// Output
	BRAKE_A_PIN_DDR |= (1<<BRAKE_A_PIN);	// Output
	
	LED_PIN_DDR |= (1<<LED_PIN);	// Init test pin
	LCD_PIN_DDR |= (1<<LCD_PIN);	// Init test pin
}

void init_int(void)
{
	//OCRn =  [ (clock_speed / Prescaler_value) * Desired_time_in_Seconds ] - 1
	
	/* PWM control directtly from timer, fast pwm, not used due to only using on/off control */
	// Init motor timer2, output pin is PD3, OC2B
	//TCCR2A |= (1<<COM2B1);	// None-inverted mode (HIGH at bottom, LOW on Match)
	//TCCR2A |= (1 << WGM21) | (1 << WGM20);	// set fast PWM Mode
	//TCCR2B |= (1 << CS21);	// set prescaler to 8 and starts PWM
	//OCR2B = 127;	//50%duty
	
	/*
	TCCR0A = 0;							// Set the Timer Mode to normal, 0
	//TCCR0B |= (1<<CS02);	// set prescaler to 1024 and start the timer, pwm freq. 244Hz
	TCCR0B |= ((1 << CS00)|(1<<CS02));	// set prescaler to 1024 and start the timer, pwm freq. 61Hz
	TIMSK0 |= (1 << OCIE0A);		// Enable int on compA
	TIMSK0 |= (1 << TOIE0);			// Enable int on OVF
	TCNT0 = 0;	// init counter*/
	
	// Timer 1, interrupt routine for checking device status
	//print_int((((F_CPU/1024)*(DISPLAY_REFRESH_MS/1000))-1),TRUE);
	TCCR1B |= (1 << WGM12);				// Set the Timer Mode to CTC, reset at compA
	TCCR1B |= ((1 << CS10)|(1<<CS12));	// set prescaler to 1024 and start the timer	
	TIMSK1 |= (1 << OCIE1A);			// Enable int on compA
	OCR1A = (((((F_CPU/1024)*INTERRUPT_MS)/1000))-1);
	TCNT1 = 0;	// init counter
	
	sei();								//enable global interrupts
}

void init_adc(void)
{
	ADMUX |= (1<<REFS0);						/* Select Vref=AVcc */
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	/* set prescaller to 128  */
	ADCSRA |= _BV(ADEN);						/* Enable ADC */
}

uint16_t read_adc(uint8_t adcx)
{
	/* adcx is the analog pin we want to use. ADMUX's first few bits are
	* the binary representations of the numbers of the pins so we can
	* just 'OR' the pin's number with ADMUX to select that pin.
	* We first zero the four bits by setting ADMUX equal to its higher
	* four bits. */
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (adcx & 0x0F);
	
	/* This starts the conversion. */
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	
	/* This is an idle loop that just wait around until the conversion
	* is finished. It constantly checks ADCSRA's ADSC bit, which we just
	* set above, to see if it is still set. This bit is automatically
	* reset (zeroed) when the conversion is ready so if we do this in
	* a loop the loop will just go until the conversion is ready. */
	while( ADCSRA & (1<<ADSC) );
	
	/* Finally, we return the converted value to the calling function. */
	return ADC;
}

void print_values(void)
{
	uart_puts("Curr=");
	print_int(device.current, FALSE);

	uart_puts(" Setp=");
	print_int(device.setpoint_angle, FALSE);
	
	uart_puts(" Angl=");
	print_int(device.current_angle, FALSE);
	
	uart_puts(" Err=");
	print_int(device.error, FALSE);
	
	uart_puts(" old=");
	print_int(device.error_old, FALSE);
	
	uart_puts(" Spd=");
	print_int(device.speed, FALSE);
	
	uart_puts(" Dir=");
	if (device.direction == PULL)
	{
		uart_puts("PULL");		
	}
	else
	{
		uart_puts("RELEASE");	
	}
	
	uart_puts(" STAT=");
	
	switch (device.status)
	{
		case WORKING_UP:
			uart_puts("WORKING UP\n");
		break;
		
		case WORKING_DOWN:
			uart_puts("WORKING DOWN\n");
		break;
		
		case DONE:
			uart_puts("DONE\n");
		break;
		
		case STOP:
			uart_puts("STOP\n");
		break;
		
		case CURR_OVF:
			uart_puts("CURR OVF\n");
		break;
		
		default:
		break;
	}	
}

void print_HMI(void)
{
	uart_puts("n0.val=");
	print_int(device.current_angle, FALSE);
	uart_puts("ÿÿÿ");
	
	uart_puts("n1.val=");
	print_int(device.setpoint_angle, FALSE);
	uart_puts("ÿÿÿ");
	
	switch (device.status) 
	{
		case WORKING_UP:
			uart_puts("t3.txt=\"TREK..\"");
			uart_puts("ÿÿÿ");			
			uart_puts("t3.picc=0");	// Blue
			uart_puts("ÿÿÿ");
		break;
		
		case WORKING_DOWN:
			uart_puts("t3.txt=\"ONTSPAN..\"");
			uart_puts("ÿÿÿ");
			uart_puts("t3.picc=0");	// Blue
			uart_puts("ÿÿÿ");
		break;
		
		case DONE:
			uart_puts("t3.txt=\"KLAAR\"");
			uart_puts("ÿÿÿ");
			uart_puts("t3.picc=1");	// Green
			uart_puts("ÿÿÿ");
		break;
		
		case STOP:
			uart_puts("t3.txt=\"STOP, RESET\"");
			uart_puts("ÿÿÿ");	
			uart_puts("t3.picc=2");	// Red
			uart_puts("ÿÿÿ");
		break;
		
		case CURR_OVF:
			uart_puts("t3.txt=\"STOP, KRACHT\"");
			uart_puts("ÿÿÿ");
			uart_puts("t3.picc=2");	// Red
			uart_puts("ÿÿÿ");
		break;
		
		default:
		break;
	}
		
	//uart_puts("n2.val=");
	//print_int(device.current, FALSE);
	//uart_puts("ÿÿÿ");	
	//g0.txt="Hello"ÿÿÿ
}

void set_motor_dir(uint8_t dir)
{
	if (dir == PULL)
	{
		DIR_A_ON;
	}
	else if (dir == RELEASE)
	{
		DIR_A_OFF;
	}	
	device.direction = dir;
}

void set_motor_speed(uint8_t speed)
{
	if (speed>=0 && speed<=255)
	{
		if (speed==255)
		{
			PWM_A_ON;
			device.speed=255;
		}
		else if (speed==0)
		{
			PWM_A_OFF;
			device.speed=0;
		}
		else
		{
			OCR2B = speed;	// set pwm duty cycle
			device.speed = speed;
		}
	}
}

void p_loop(void)
{
	uint16_t speed = 0;
	//static uint16_t error_old = 0;
	static uint16_t timer = 0;
	static uint8_t move_kick = 0;	// to start the movement downwards

	if (device.movementEnabled == TRUE)
	{
		if ((device.current_angle + device.min_act_on_error) <= device.setpoint_angle)	// setpoint is further, pull
		{
			device.error = (device.setpoint_angle - device.current_angle);	// Calc error
			set_motor_dir(PULL);
			speed = (P_GAIN * device.error);								// calculate output pwm value
			speed = (speed > 255) ? 255 : speed;							// limit output
			//set_motor_speed(speed);	// do output
			set_motor_speed(255);	// do output
			device.status = WORKING_UP;
		}
		else if ((device.current_angle > device.min_act_on_error) && ((device.current_angle - device.min_act_on_error) >= device.setpoint_angle))  // past the setpoint, release
		{
			device.error = (device.current_angle - device.setpoint_angle); // Calc error
			set_motor_dir(RELEASE);

			if (timer == (FUNCTION_TIMER_MS/INTERRUPT_MS))
			{
				//if (move_kick<=1)
				//{
				//	set_motor_speed(255);	// start with a bit of movement
				//	move_kick++;	
					//error_old = device.error;	
				//	uart_puts("kick\n");			
				//}
				//else
				{
					//LED_TOGGLE;	// every 200ms?
					if (device.error < device.error_old)	// if moved a bit
					{
						speed = (P_GAIN * device.error);		// calculate output pwm value
						speed = (speed > 255) ? 255 : speed;	// limit output
						//set_motor_speed(speed);
						set_motor_speed(255);
					}
					else
					{
						set_motor_speed(0);	// wait for the user to release a bit
					}
				}
				if ((device.error < device.error_old) || (device.error_old==0))
				{
					device.error_old = device.error;	// only do this when the hand has moved down
				}
				//uart_puts("err_old=");
				//print_int(error_old, 1);
				
				timer = 0;
			}
			else
			{
				timer++;
			}
			device.status = WORKING_DOWN;
		}
		else
		{
			set_motor_speed(0);	// at position
			device.min_act_on_error = MIN_ACT_ON_ERROR_EXTENDED;
			//move_kick=0;
			device.status = DONE;
		}
		check_auto_movement();	// Check if it has to do auto movement
	}
	else
	{
		set_motor_speed(0);	
	}	
}

void check_auto_movement(void)
{
	static uint8_t move_up = FALSE;	// begin to move to 0
	
	if (device.multiply_movement>0)
	{
		if (device.status == DONE)
		{
			device.min_act_on_error=MIN_ACT_ON_ERROR_DEFAULT;
			
			if (move_up)
			{
				device.setpoint_angle = device.setpoint_angle_previous;
				move_up = FALSE;
				//uart_puts("Set to up:");
				//print_int(device.multiply_movement, TRUE);
				device.multiply_movement--;
			}
			else
			{
				//device.setpoint_angle = 0;
				device.setpoint_angle = (device.setpoint_angle_previous-15);
				move_up = TRUE;
				device.error_old=0;
				//uart_puts("Set to down\n");
			}
		}	
	}
}









