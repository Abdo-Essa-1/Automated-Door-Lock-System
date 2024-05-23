/*
 * Functions.h
 *
 *  Created on: May 23, 2024
 *      Author: Abdo Essa
 */

#ifndef DOORLOCKSYSTEM_H_
#define DOORLOCKSYSTEM_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PassSize						6

#define CloseButton_PORT_ID				PORTD_ID
#define CloseButton_PIN_ID				PIN2_ID

#define OpenButton_PORT_ID				PORTD_ID
#define OpenButton_PIN_ID				PIN3_ID

#define OpenLed_PORT_ID					PORTB_ID
#define OpenLed_PIN_ID					PIN2_ID

#define SERVO_PORT_ID					PORTB_ID
#define SERVO_PIN_ID					PIN3_ID

#define CloseLed_PORT_ID				PORTB_ID
#define CloseLed_PIN_ID					PIN4_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * adjust pwm to operate servo at suitable angle
 */
void pwm_timer0(uint8 pwm);

void InitalSets(void);

void AutomatedDoorLock(void);

void Passwindow(void);

uint8 CheckEmpty(void);

uint8 CheckPass(void);

void PassWrong(void);

void OpenDoor(void);

void CheckClose(void);

void ISR_INT1_Callback(void);

#endif /* DOORLOCKSYSTEM_H_ */
