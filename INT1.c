/*
 * INT1.c
 *
 *  Created on: Apr 27, 2024
 *      Author: Abdo Essa
 */

#include "INT1.h"
#include <avr/interrupt.h>
#include <avr/io.h>

// Global pointer to function for the callback
static void (*INT1_Callback)(void) = NULL;

// Interrupt Service Routine for INT1
ISR(INT1_vect)
{
    // Ensure the interrupt flag is cleared
    GIFR |= (1 << INTF1);

    // Call the user-defined callback function
    if (INT1_Callback != NULL)
    {
        INT1_Callback();
    }
}

// Initialization function for INT1
void INT1_Init(void)
{
    // Configure INT1 to trigger on the falling edge
    MCUCR |= (1 << ISC11);  // Set ISC11
    MCUCR &= ~(1 << ISC10); // Clear ISC10

    // Clear any existing interrupt flag for INT1
    GIFR |= (1 << INTF1);

    // Enable INT1 interrupt
    GICR |= (1 << INT1);

    // Enable global interrupts
    sei();
}

// Function to set the callback for INT1
void INT1_SetCallback(void (*callback)(void))
{
    INT1_Callback = callback;
}
