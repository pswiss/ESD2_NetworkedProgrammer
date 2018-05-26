// Author: Petras Swissler
// Task 3
////////////////////////////////////////////////////////////////////////////////////////////////////
/* File: conf_board.h
This file contains various #define statements that are used to define the general configuration 
of the pins used for the LED and pushbutton
////////////////////////////////////////////////////////////////////////////////////////////////////
*/

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

// GPIO initialization for the pins (Defined using S11.2 of the data sheet
// LED OUTPUT
#define LED_PIN		 				PIO_PA0_IDX

 // Pushbutton INPUT
#define BUTTON_PIN					(PIO_PB4_IDX)

#endif // CONF_BOARD_H
