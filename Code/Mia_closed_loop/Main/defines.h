/*
* defines.h
*
* Created: 2-4-16
*  Author: J-J_LAPTOP
*/
#ifndef DEFINES_H_
#define DEFINES_H_

#include <avr/eeprom.h>
// Defines
#include <avr/io.h>
#include <util/delay.h>
#include "Hal.h"
#define VERSION 1

#define ON 1
#define OFF 0

/* Hardware variables */
#define MIN_ANGLE 348
#define MAX_ANGLE 665
#define MAX_DEGREES 105


/* P loop variables */
#define P_GAIN				1	// P loop gain
#define MIN_ACT_ON_ERROR	5	// Max error value to act on, otherwise stop
#define TIMEOUT_MS			500 	// Stop motor after no response

#define INTERRUPT_MS 10	// check max also!
#define REFRESH_LOOP_MS 500	/* delay in main loop */ 

#define TIMEOUT_TIME (5000/REFRESH_LOOP_MS) // 5 SEC

#define FALSE 0
#define TRUE 1

struct database {
	uint8_t direction;
	uint8_t speed;
	uint8_t setpoint_angle;
	uint16_t current_angle;
	uint16_t error;
	uint16_t current;
	uint16_t current_limit;
	volatile uint8_t movementEnabled;
	uint8_t status;
};

struct database device;

void p_loop(void);
uint16_t calculate_error(void);

void print_int(int c, uint8_t ln);
void print_float(double c, uint8_t ln);
void print_value (char id, int value);

void set_motor_dir(uint8_t dir);
void set_motor_speed(uint8_t speed);

void print_values(void);	// debug info

#define FORWARD 0
#define BACKWARD 1

/* LED PIN PIN5/PD5 */
#define LED_PIN		PD5
#define LED_PIN_DDR	DDRD
#define LED_PIN_PORT	PORTD
#define LED_PIN_REG	PIND
#define LED_ON			(LED_PIN_PORT = (1 << LED_PIN))			// debug led on
#define LED_OFF 		(LED_PIN_PORT &= ~(1 << LED_PIN))		// debug led off
#define LED_TOGGLE		(LED_PIN_PORT ^= (1 << LED_PIN))		// debug toggle

// DIR D13/D12
// PWM D3/D11
// BRAKE D8/D9
// SENSE A0/A1

/* MOTOR PINS */
#define DIR_A_PIN		PB4	//D12
#define DIR_A_PIN_DDR	DDRB
#define DIR_A_PIN_PORT	PORTB
#define DIR_A_PIN_REG	PINB
#define DIR_A_ON		(DIR_A_PIN_PORT = (1 << DIR_A_PIN))
#define DIR_A_OFF 		(DIR_A_PIN_PORT &= ~(1 << DIR_A_PIN))

#define DIR_B_PIN		PB5	//D13
#define DIR_B_PIN_DDR	DDRB
#define DIR_B_PIN_PORT	PORTB
#define DIR_B_PIN_REG	PINB
#define DIR_B_ON		(DIR_B_PIN_PORT = (1 << DIR_B_PIN))
#define DIR_B_OFF 		(DIR_B_PIN_PORT &= ~(1 << DIR_B_PIN))

#define PWM_A_PIN		PD3	//D3
#define PWM_A_PIN_DDR	DDRD
#define PWM_A_PIN_PORT	PORTD
#define PWM_A_PIN_REG	PIND
#define PWM_A_ON		(PWM_A_PIN_PORT = (1 << PWM_A_PIN))
#define PWM_A_OFF 		(PWM_A_PIN_PORT &= ~(1 << PWM_A_PIN))

#define PWM_B_PIN		PB3	//D11
#define PWM_B_PIN_DDR	DDRB
#define PWM_B_PIN_PORT	PORTB
#define PWM_B_PIN_REG	PINB
#define PWM_B_ON		(PWM_B_PIN_PORT = (1 << PWM_B_PIN))
#define PWM_B_OFF 		(PWM_B_PIN_PORT &= ~(1 << PWM_B_PIN))

#define BRAKE_A_PIN			PB1	//D9
#define BRAKE_A_PIN_DDR		DDRB
#define BRAKE_A_PIN_PORT	PORTB
#define BRAKE_A_PIN_REG		PINB
#define BRAKE_A_ON			(BRAKE_A_PIN_PORT = (1 << BRAKE_A_PIN))
#define BRAKE_A_OFF 		(BRAKE_A_PIN_PORT &= ~(1 << BRAKE_A_PIN))

#define BRAKE_B_PIN			PB0	//D8
#define BRAKE_B_PIN_DDR		DDRB
#define BRAKE_B_PIN_PORT	PORTB
#define BRAKE_B_PIN_REG		PINB
#define BRAKE_B_ON			(BRAKE_B_PIN_PORT = (1 << BRAKE_B_PIN))
#define BRAKE_B_OFF 		(BRAKE_B_PIN_PORT &= ~(1 << BRAKE_B_PIN))

#define SENSE_A_PIN			PC0	//A0
#define SENSE_A_PIN_DDR		DDRC
#define SENSE_A_PIN_PORT	PORTC
#define SENSE_A_PIN_REG		PINC
#define SENSE_A_ON			(SENSE_A_PIN_PORT = (1 << SENSE_A_PIN))
#define SENSE_A_OFF 		(SENSE_A_PIN_PORT &= ~(1 << SENSE_A_PIN))

#define SENSE_B_PIN			PC1	//A1
#define SENSE_B_PIN_DDR		DDRC
#define SENSE_B_PIN_PORT	PORTC
#define SENSE_B_PIN_REG		PINC
#define SENSE_B_ON			(SENSE_B_PIN_PORT = (1 << SENSE_B_PIN))
#define SENSE_B_OFF 		(SENSE_B_PIN_PORT &= ~(1 << SENSE_B_PIN))

/* ANGLE PIN */
#define ANGLE_PIN			PC3	//IN 3

/* ON/OFF SWITCH */
#define SW_PIN			PD7
#define SW_PIN_REG		PIND
#define SW_PIN_DDR		DDRD
#define SW_PIN_PORT		PORTD
#define SW_STATUS		(SW_PIN_REG & (1<<SW_PIN))

/* BATTERY/ADC PIN */
#define BATT_PIN		PC0	// ADC0
#define BATT_PWR_PIN	PC1	// To turn on the Battery ADC measurement (LOW=ON)
#define BATT_PWR_DDR	DDRC
#define BATT_PWR_PORT	PORTC
#define BATT_PWR_REG	PINC
#define BATT_PWR_INIT	(BATT_PWR_DDR |= (1<<BATT_PWR_PIN))			// INIT pin
#define BATT_PWR_OFF	(BATT_PWR_PORT = (1 << BATT_PWR_PIN))		// Reversed logic, on=gnd	
#define BATT_PWR_ON 	(BATT_PWR_PORT &= ~(1 << BATT_PWR_PIN))		// Reversed logic, on=gnd	

/* CHARGE STATUS PIN */
#define CHARGING 1
#define FULL 0
#define CHARGE_ST_PIN		PC2
#define CHARGE_ST_REG		PINC
#define CHARGE_ST_DDR		DDRC
#define CHARGE_ST_PORT		PORTC
#define CHARGE_PIN_STATUS	(CHARGE_ST_REG & (1<<CHARGE_ST_PIN))

/* POWER ON PIN */
#define PWR_ON_PIN	PD6
#define PWR_ON_DDR	DDRD
#define PWR_ON_PORT	PORTD
#define PWR_ON_REG	PIND
#define PWR_ON_INIT	(PWR_ON_DDR |= (1<<PWR_ON_PIN))			
#define PWR_ON		(PWR_ON_PORT = (1 << PWR_ON_PIN))		
#define PWR_OFF		(PWR_ON_PORT &= ~(1 << PWR_ON_PIN))		

#define AT_PIN		PD3
#define AT_PIN_DDR	DDRD
#define AT_PIN_PORT	PORTD
#define AT_PIN_REG	PIND
#define AT_PIN_INIT	(AT_PIN_DDR |= (1<<AT_PIN))		
#define AT_ON		(AT_PIN_PORT = (1 << AT_PIN))			// debug led on
#define AT_OFF 		(AT_PIN_PORT &= ~(1 << AT_PIN))			// debug led off


#endif