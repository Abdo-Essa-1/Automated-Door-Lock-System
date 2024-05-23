/*
 * Description:
 * Create an automated door lock system where the door is locked/unlocked using a keypad.
 * The system should display the status on the LCD.
 * use LEDs to indicate if the door is locked or unlocked.
 *
 * main.c
 *
 *  Created on: May 23, 2024
 *      Author: Abdo Essa
 */

#include "DoorLockSystem.h"

int main()
{
	/******Initializations******/
	InitalSets();

	/******PROGRAM******/
	while(1)
	{
		AutomatedDoorLock();
	}

	return 0;
}
