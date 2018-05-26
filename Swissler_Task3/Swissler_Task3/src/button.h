// Author: Petras Swissler
// Task 3
////////////////////////////////////////////////////////////////////////////////////////////////////
/* File: button.h
This file contains various #define statements that are used to define the specific configuration 
of the pin used for the pushbutton. A volitile variable and interrupt handler prototype are also 
defined here
////////////////////////////////////////////////////////////////////////////////////////////////////
*/

#ifndef BUTTON_H_
#define BUTTON_H_

// boilerplate include
#include <asf.h>

// Pin Definitions
/* Push button pin configuration. */
#define PUSH_BUTTON_ID                 ID_PIOB
#define PUSH_BUTTON_PIO                PIOB
#define PUSH_BUTTON_PIN_MSK            PIO_PB4
// Use the edge interrupt mode to trigger on rise AND fall
#define PUSH_BUTTON_ATTR               PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_EDGE

////////////////////////////////////////////////////////////////////////////////////////////////////
// Variables: interrupt flag
volatile uint32_t button_flag;

////////////////////////////////////////////////////////////////////////////////////////////////////
// function prototype for the configuration function
void configure_button(void);


#endif /* BUTTON_H_ */