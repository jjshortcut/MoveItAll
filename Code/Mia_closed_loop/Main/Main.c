/*
*	Main.c
*	MoveItAll project
*	Author: J-J_LAPTOP
*	Version: 0.1
*/

/*
TODO:
- Stop/Reset command uart S/R
- Check P controller
- P+1 implementation
*/

//#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
//#include <avr/eeprom.h>
//#include "uart/uart.h"
#include "uart_handler.h"

// Prototypes
void init_io(void);
void init_int(void);
void init_adc(void);

volatile uint8_t check_movement = FALSE;	// Closed loop check
uint16_t print_counter = 0;

ISR (TIMER1_COMPA_vect)  // timer1 compA
{
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
	
	check_movement = TRUE;
}

int main(void)
{
	init_io();			/* Init IO*/
	init_int();			/* Init int for resfresh display every x ms	*/
	init_adc();			/* Init ADC */	
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );	/* Init Uart */
	sei();						/* Enable global interrupts for uart*/
	//uart_puts("\n\rInit Uart OK\n");
	
	device.current_limit = DEFAULT_CURRENT_LIMIT;	// set current limit
	device.movementEnabled = FALSE;

	/* Now the device is ready! */
	//uart_puts("MoveItAll hand ready!");		/* Print version number	*/
	//print_float(VERSION,1);
	
	//set_motor_dir(FALSE);
	//set_motor_speed(127);//50%
	device.setpoint_angle = 50;
	
	while(1)
	{	
		get_serial();
		if (command_ready) {process_command();}
		
		if (check_movement)
		{
			p_loop();	// 	do p loop stuff
			check_movement = FALSE;
		}
		
		if (print_counter == REFRESH_LOOP_MS)
		{
			//print_values();
			print_HMI();
			print_counter = 0;
		}
		print_counter++;	
		
	}
}

void init_io(void)
{
	// Motor pins
	DIR_A_PIN_DDR |= (1<<DIR_A_PIN);		// Output
	PWM_A_PIN_DDR |= (1<<PWM_A_PIN);		// Output
	BRAKE_A_PIN_DDR |= (1<<BRAKE_A_PIN);	// Output
	
	//SW_PIN_DDR &= ~(1<<SW_PIN);	// Buttons/switches as input
	//SW_PIN_PORT |= (1<<SW_PIN);	// With pullups on
	
	//CHARGE_ST_DDR &= ~(1<<CHARGE_ST_PIN);	// Charge status pin as input
	LED_PIN_DDR |= (1<<LED_PIN);	// Init test pin
	//DDRB |= (1<<0);	// PB0 AS OUTPUT
	//BATT_PWR_INIT;	// Leds as output
	//BATT_PWR_OFF;	// Leds off
}

void init_int(void)
{
	//OCRn =  [ (clock_speed / Prescaler_value) * Desired_time_in_Seconds ] - 1
	
	// Init motor timer2, output pin is PD3, OC2B
	TCCR2A |= (1<<COM2B1);	// None-inverted mode (HIGH at bottom, LOW on Match)
	TCCR2A |= (1 << WGM21) | (1 << WGM20);	// set fast PWM Mode
	TCCR2B |= (1 << CS21);	// set prescaler to 8 and starts PWM
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
	
	uart_puts(" Spd=");
	print_int(device.speed, FALSE);
	
	uart_puts(" Dir=");
	if (device.direction == FORWARD)
	{
		uart_puts("PULL\n");		
	}
	else
	{
		uart_puts("RELEASE\n");	
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
	
	if (device.status==WORKING)
	{
		uart_puts("t3.txt=\"WORKING\"");
		uart_puts("ÿÿÿ");
	}
	else if (device.status==DONE)
	{
		uart_puts("t3.txt=\"DONE\"");
		uart_puts("ÿÿÿ");
	}
	
	if (device.status==STOP)
	{
		uart_puts("t3.txt=\"STOP\"");
		uart_puts("ÿÿÿ");
	}
	
	uart_puts("n2.val=");
	print_int(device.current, FALSE);
	uart_puts("ÿÿÿ");	
	//g0.txt="Hello"ÿÿÿ
}

void set_motor_dir(uint8_t dir)
{
	if (dir == FORWARD)
	{
		DIR_A_ON;
	}
	else if (dir == BACKWARD)
	{
		DIR_A_OFF;
	}	
	device.direction = dir;
}

void set_motor_speed(uint8_t speed)
{
	if (speed>=0 && speed<=255)
	{
		OCR2B = speed;	// set pwm duty cycle
		device.speed = speed;
	}
}

uint16_t calculate_error(void)
{
	return 0;
}
/*
void p_loop(void)
{
	uint16_t speed = 0;
	static uint16_t error_old = 0;
	uint16_t response = 0;
	static uint16_t timeout_counter;
	
	if (device.current>=device.current_limit)
	{
		set_motor_speed(0);	// Turn motor off
		// TODO set brake pin
		device.movementEnabled = FALSE;
	}
	
	if (device.current_angle < device.setpoint_angle)	// setpoint is further, pull
	{
		device.error = (device.setpoint_angle - device.current_angle);	// Calc error
		set_motor_dir(FORWARD);
		speed = (P_GAIN * device.error);								// calculate output pwm value
		speed = (speed > 255) ? 255 : speed;							// limit output
		speed = (device.error<=MIN_ACT_ON_ERROR) ? 0 : speed;			// if error too small stop motor
		set_motor_speed(speed);	// do output
	}
	else  // past the setpoint, release
	{	
		device.error = (device.current_angle - device.setpoint_angle); // Calc error
		set_motor_dir(BACKWARD);
		speed = (P_GAIN * device.error);								// calculate output pwm value
		//speed = (speed > 255) ? 255 : speed;							// limit output
		speed = (speed > 0) ? 255 : 0;							// limit output
		speed = (device.error<=MIN_ACT_ON_ERROR) ? 0 : speed;			// if error too small stop motor
		
		
		if (timeout_counter >= (TIMEOUT_MS/INTERRUPT_MS))	// time to check response
		{
			response = (device.error - error_old);
			//uart_puts("response=");
			//print_int(response, TRUE);
			error_old = device.error;	// get new old error
			
			if(response <= 5)		//no response
			{
				set_motor_speed(0);	// stop motor		
			}
			else
			{				
				set_motor_speed(speed);	// keep moving	
				uart_puts("ON\n");
			}
			timeout_counter=0;	// reset
		}
		timeout_counter++;
	}
}
*/

void p_loop(void)
{
	uint16_t speed = 0;
	static uint16_t error_old = 0;
	static uint16_t timer = 0;
	static uint8_t init_move = FALSE;
	
	if (device.current>=device.current_limit)
	{
		set_motor_speed(0);	// Turn motor off
		// TODO set brake pin
		device.movementEnabled = FALSE;
		device.status = STOP;
	}
	
	if (device.movementEnabled == TRUE)
	{
		if ((device.current_angle + MIN_ACT_ON_ERROR) <= device.setpoint_angle)	// setpoint is further, pull
		{
			device.error = (device.setpoint_angle - device.current_angle);	// Calc error
			set_motor_dir(FORWARD);
			speed = (P_GAIN * device.error);								// calculate output pwm value
			speed = (speed > 255) ? 255 : speed;							// limit output
			//set_motor_speed(speed);	// do output
			set_motor_speed(255);	// do output
			device.status = WORKING;
		}
		else if ((device.current_angle > MIN_ACT_ON_ERROR) && ((device.current_angle - MIN_ACT_ON_ERROR) >= device.setpoint_angle))  // past the setpoint, release
		{
			device.error = (device.current_angle - device.setpoint_angle); // Calc error
			set_motor_dir(BACKWARD);
			
			if (!init_move)	// do release a bit first
			{
				set_motor_speed(255);
				init_move = TRUE;
				//uart_puts("INITSPEED\n");
			}
			else
			{
				if (timer == (FUNCTION_TIMER_MS/INTERRUPT_MS))
				{
					LED_TOGGLE;	// every 200ms?
					if (device.error < error_old)	// if moved a bit
					{
						speed = (P_GAIN * device.error);								// calculate output pwm value
						speed = (speed > 255) ? 255 : speed;							// limit output
						//set_motor_speed(speed);
						set_motor_speed(255);	// do output
						//uart_puts("ON\n");
					}
					else
					{
						set_motor_speed(0);	// wait for the user to release a bit
						//uart_puts("OFF\n");
					}
					
					error_old = device.error;
					timer = 0;
					device.status = WORKING;
				}
				else
				{
					timer++;
				}
			}
		}
		else
		{
			set_motor_speed(0);	// at position
			init_move = TRUE;
			device.status = DONE;
		}
	}
	else
	{
		set_motor_speed(0);
		device.status = STOP;
	}	
}








