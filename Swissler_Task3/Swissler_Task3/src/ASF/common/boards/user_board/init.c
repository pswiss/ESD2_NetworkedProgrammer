// Author: Petras Swissler
// Task 3
////////////////////////////////////////////////////////////////////////////////////////////////////
/* File: init.c
This file contains the board_init() function, which initializes the IO ports and then configures 
pins for (1) LED output and (1) button input 
////////////////////////////////////////////////////////////////////////////////////////////////////
*/
// Boilerplate include
#include <asf.h>
#include <board.h>
#include <conf_board.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
void board_init(void)
{
	// boilerplate initialization
	ioport_init();	
	
	/* Set output direction on the given LED IOPORTs */
	ioport_set_pin_dir(LED_PIN, IOPORT_DIR_OUTPUT);
	
	// Set the LED OFF
	ioport_set_pin_level(LED_PIN,LOW);

	/* Set direction and pullup on the given button IOPORT */
	ioport_set_pin_dir(BUTTON_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(BUTTON_PIN, IOPORT_MODE_PULLUP);
}
