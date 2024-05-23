/*
 * Functions.c
 *
 *  Created on: May 23, 2024
 *      Author: Abdo Essa
 */

#include "DoorLockSystem.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "common_macros.h"
#include "std_types.h"
#include "gpio.h"
#include "keypad.h"
#include "lcd.h"
#include "INT1.h"

// Global variables
uint8 OpenPressed = FALSE;
uint8 InLoop = FALSE;
uint8 pass[PassSize] = {7, 4, 6, 3, 5, 8};
uint8 passcheck = TRUE;
uint8 i = 0;

// Function to adjust PWM to operate servo at suitable angle
void pwm_timer0(uint8 pwm)
{
	TCCR0 = 0;
	TCNT0 = 0;
	TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS01); // Compare mode with PWM, prescaler 8
	OCR0 = pwm;
}

// Initialize system settings
void InitalSets(void) {
	LCD_init();
	INT1_Init();
	INT1_SetCallback(ISR_INT1_Callback);
	GPIO_setupPinDirection(OpenLed_PORT_ID, OpenLed_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(CloseLed_PORT_ID, CloseLed_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(SERVO_PORT_ID, SERVO_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(OpenButton_PORT_ID, OpenButton_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(CloseButton_PORT_ID, CloseButton_PIN_ID, PIN_INPUT);

	GPIO_writePin(OpenLed_PORT_ID, OpenLed_PIN_ID, LOGIC_LOW);
	GPIO_writePin(CloseLed_PORT_ID, CloseLed_PIN_ID, LOGIC_HIGH);
	GPIO_writePin(OpenButton_PORT_ID, OpenButton_PIN_ID, LOGIC_HIGH);
	GPIO_writePin(CloseButton_PORT_ID, CloseButton_PIN_ID, LOGIC_HIGH);
	GPIO_writePin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID, LOGIC_HIGH);
	GPIO_writePin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + 1, LOGIC_HIGH);
	GPIO_writePin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + 2, LOGIC_HIGH);
	GPIO_writePin(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + 3, LOGIC_HIGH);
	pwm_timer0(0);
}

// Main automated door lock function
void AutomatedDoorLock(void) {
	Passwindow();
	if(!CheckEmpty()){
		if(CheckPass()){
			OpenDoor();
			CheckClose();
		}
		else{
			PassWrong();
		}
	}
}

// Function to handle password entry
void Passwindow(void) {
	LCD_clearScreen();
	LCD_displayString("Door Lock System");
	LCD_displayStringRowColumn(1, 0, "Pass: ");
	LCD_moveCursor(1, 6);
	uint8 key;
	i = 0;
	passcheck = TRUE;
	InLoop = TRUE;

	key = KEYPAD_getPressedKey();

	while (i < 10 && !OpenPressed) {
		LCD_intgerToString(key);
		_delay_ms(500);
		if (i >= PassSize) {
			passcheck = FALSE;
		} else {
			if (key != pass[i]) {
				passcheck = FALSE;
			}
		}
		++i;
		key = KEYPAD_getPressedKey();
	}
	InLoop = FALSE;
	OpenPressed = FALSE;
	CheckPass();
}

// Check if any keys were entered
uint8 CheckEmpty(void) {
	if (i == 0) {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 1, "Please Enter a");
		LCD_displayStringRowColumn(1, 6, "Pass");
		_delay_ms(2000);
		return TRUE;
	}
	return FALSE;
}

// Function to check the entered password
uint8 CheckPass(void) {
	if (passcheck && i == PassSize) {
		return TRUE;
	} else {
		return FALSE;
	}
}

// Function to handle door opening
void OpenDoor(void) {
	LCD_clearScreen();
	LCD_displayString("Pass is correct!");
	LCD_displayStringRowColumn(1, 1, "Door opens now");
	pwm_timer0(186);
	GPIO_writePin(OpenLed_PORT_ID, OpenLed_PIN_ID, LOGIC_HIGH);
	GPIO_writePin(CloseLed_PORT_ID, CloseLed_PIN_ID, LOGIC_LOW);
}

// Function to handle incorrect password
void PassWrong(void) {
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 1, "Pass is wrong!");
	LCD_displayStringRowColumn(1, 0, "Enter pass again");
	_delay_ms(2000);
}

// Function to handle door closing
void CheckClose(void) {
	uint8 Unpressed;
	while (1) {
		Unpressed = GPIO_readPin(CloseButton_PORT_ID, CloseButton_PIN_ID);
		if (!Unpressed) {
			LCD_clearScreen();
			LCD_displayString("Closing Door now");
			LCD_displayStringRowColumn(1, 4, "Good bye");
			pwm_timer0(0);
			GPIO_writePin(OpenLed_PORT_ID, OpenLed_PIN_ID, LOGIC_LOW);
			GPIO_writePin(CloseLed_PORT_ID, CloseLed_PIN_ID, LOGIC_HIGH);
			_delay_ms(2000);
			break;
		}
	}
}

void ISR_INT1_Callback(void){
	if(InLoop){
		OpenPressed = TRUE;
	}
}
