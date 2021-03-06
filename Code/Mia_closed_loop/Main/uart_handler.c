/*
* Uart handler, handles the incoming and outgoing stream
*/

#include <avr/io.h>
#include "defines.h"
#include "uart_handler.h"
#include <util/atomic.h>
#include <string.h>
#include <avr/interrupt.h>

uint8_t debug_on = 0;	// Debug msg's

uint16_t readCommand(void)
{
	char *pch;
	char cmdValue[16];
	char data[10];
	uint8_t counter = 0;
	unsigned int c;
	
	c = uart_getc();	// get new byte
	
	//unsigned int c = uart_getc();
	while (!(c & UART_NO_DATA)	// If data is correct
	&&	!(c & UART_FRAME_ERROR)
	&&	!(c & UART_OVERRUN_ERROR)
	&&	!(c & UART_BUFFER_OVERFLOW)
	)
	{
		data[counter] = c;
		if (data[counter] == '\n') {
			} else {
			counter++;
		}
		
		c = uart_getc();	// get new byte
	}
	
	/*uart_puts("data in-");
	for(uint8_t i=0; i<sizeof(data); i++) {
		uart_putc(data[i]);
	}
	uart_puts("-\n");*/
	
	if (data[0] == '=') {
			// Find the position the equals sign is
			// in the string, keep a pointer to it
			pch = strchr(data, '=');
			// Copy everything after that point into
			// the buffer variable
			strcpy(cmdValue, pch+1);
			// Now turn this value into an integer and
			// return it to the caller.
			
			return atoi(cmdValue);
	}
	else
	{
		return 0;
	}
}

void print_int(int c, uint8_t ln)
{
	char buffer[8];
	itoa( c, buffer, 10);
	uart_puts(buffer);
	
	if (ln)	// if add line end
	{
		uart_puts("\n");
	}
}

void print_float(double c, uint8_t ln)
{
	char buffer[20];
	dtostrf(c, 10, 7, buffer);
	uart_puts(buffer);
	
	if (ln)	// if add line end
	{
		uart_puts("\n");
	}
}

void print_value (char id, int value)
{
	char buffer[8];
	itoa(value, buffer, 10);
	uart_putc(id);
	uart_putc('=');
	uart_puts(buffer);
	uart_putc('\n');
}

void get_serial(void)
{
	unsigned int c = uart_getc();
	if (!(c & UART_NO_DATA)	// If data is correct
	&&	!(c & UART_FRAME_ERROR)
	&&	!(c & UART_OVERRUN_ERROR)
	&&	!(c & UART_BUFFER_OVERFLOW)
	)
	{
		command_in[data_count] = c;
		if ((command_in[data_count] == '\n') || (command_in[data_count] == '\r'))
		{
			uart_puts("Received =<");
			for(uint8_t i=0; i<data_count; i++) {
				uart_putc(command_in[i]);
			}
			uart_puts(">\n");
			
			command_ready = TRUE;
			process_command();
			data_count = 0;	// reset
			memset(command_in, 0, sizeof(command_in));	// set command to 0
		}
		else
		{
			data_count++;
		}
	}	
}

unsigned long read_int_value ()
{
	char *pch;
	char cmdValue[16];
	// Find the position the equals sign is
	// in the string, keep a pointer to it
	pch = strchr(command_in, '=');
	// Copy everything after that point into
	// the buffer variable
	strcpy(cmdValue, pch+1);
	// Now turn this value into an integer and
	// return it to the caller.
	return atoi(cmdValue);
}

void process_command()
{
	uint16_t val = 0;
	switch (command_in[0]) {
			
		case 'V':	/* duty cycle */
		if (command_in[1] == '=') {
			val = read_int_value();}

		if (val>=0 && val<=255)
		{
			set_motor_speed(val);
			uart_puts("Speed = ");
			print_int(val, TRUE);
		}
		else
		{
			uart_puts("Speed not in range of 0-255\n");
		}
		break;	
		
		case 'e':	/* direction test */
		if (command_in[1] == '=') {
		val = read_int_value();}

		if (val>=0 && val<=100)
		{
			uart_puts("e value = ");
			print_int(val, TRUE);
		}
		break;
		
		case 'P':	/* Setpoint Angle */
		if (command_in[1] == '=') 
		{
			if ((!device.multiply_movement) && (device.status==DONE))
			{
				val = read_int_value();
				if (val>=0 && val<=255)
				{
					device.setpoint_angle = val;
					device.min_act_on_error=MIN_ACT_ON_ERROR_DEFAULT;
					device.error_old=0;
					uart_puts("Setpoint = ");
					print_int(val, TRUE);
				}
				else
				{
					uart_puts("Setpoint not in range 0-255\n");
				}
			}
		}

		break;
		
		case '+':	/* Setpoint Angle + */
			if (command_in[1] == '=')
			{
				if ((!device.multiply_movement) && (device.status==DONE))
				{
					val = read_int_value();
					if (val>0 && val<=MAX_DEGREES)
					{
						device.setpoint_angle += val;
						device.setpoint_angle = (device.setpoint_angle >= MAX_DEGREES) ? MAX_DEGREES : device.setpoint_angle;
						device.min_act_on_error=MIN_ACT_ON_ERROR_DEFAULT;
						device.error_old=0;
						uart_puts("Setpoint = ");
						print_int(device.setpoint_angle, TRUE);
					}
					else
					{
						uart_puts("Setpoint not in range too big\n");
					}
				}
			}
		break;
		
		case '-':	/* Setpoint Angle - */
			if (command_in[1] == '=')
			{
				if ((!device.multiply_movement) && (device.status==DONE))
				{
					val = read_int_value();
					if (device.setpoint_angle >= val)
					{
						device.setpoint_angle -= val;
						device.min_act_on_error=MIN_ACT_ON_ERROR_DEFAULT;
						device.error_old=0;
						uart_puts("Setpoint = ");
						print_int(device.setpoint_angle, TRUE);
					}
					else
					{
						uart_puts("Setpoint not in range, too small\n");
					}
				}
			}
		break;
		
		case 'R':	/* Reset */
			device.movementEnabled = TRUE;
			device.multiply_movement = 0;
			device.min_act_on_error=MIN_ACT_ON_ERROR_DEFAULT;
			device.error_old=0;
			uart_puts("Reset");
		break;
		
		case 'S':	/* STOP */
			device.movementEnabled = FALSE;
			device.status=STOP;
			uart_puts("STOP!");
		break;
		
		case 'X':	/* Action for X times */
		if (command_in[1] == '=')
		{
			val = read_int_value();
			if (val>=0 && val<=100)
			{
				device.setpoint_angle_previous = device.setpoint_angle;	// save current setpoint
				device.multiply_movement = val;
				device.min_act_on_error=MIN_ACT_ON_ERROR_DEFAULT;
				uart_puts("Doing auto movement\n");
			}
			else
			{
				uart_puts("Multiply movement not in range\n");
			}
		}
		break;
		
		case 'U':	/* PULL */
		set_motor_dir(PULL);
		set_motor_speed(255);
		_delay_ms(500);
		set_motor_speed(0);
		break;
		
		case 'D':	/* RELEASE */
		set_motor_dir(RELEASE);
		set_motor_speed(255);
		_delay_ms(500);
		set_motor_speed(0);
		break;
				
		default:
		uart_puts("No valid command:");
		uart_putc(command_in[0]);
		uart_puts(" (");
		print_int(command_in[0], 0);
		uart_puts(")\n");
		break;	
	}
	command_ready = FALSE;
}
