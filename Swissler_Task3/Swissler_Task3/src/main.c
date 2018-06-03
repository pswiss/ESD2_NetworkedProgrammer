// Author: Petras Swissler
// Task 3
////////////////////////////////////////////////////////////////////////////////////////////////////
/* File: main.c
This file contains the main function, which will call all requisite helper functions to accomplish
the task of turning on an LED when a button is pushed, and turning it off when it is not.
////////////////////////////////////////////////////////////////////////////////////////////////////
*/

// Boilerplate include
#include <asf.h>

// Task-specific includes
#include "button.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
int main (void)
{
	// Boilerplate Initialization
	sysclk_init();
	wdt_disable(WDT);
	board_init();
	
	// Task-specific initialization
	configure_button();
	
	// Default button flag to be false.
	button_flag = false;
	
	// Default set LED state based on the button
	ioport_set_pin_level(LED_PIN,!ioport_get_pin_level(BUTTON_PIN));
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	while(true){

		ioport_toggle_pin_level(LED_PIN);
		delay_ms(100);
		// Bulk of code: When pushbutton changes state, it throws and innterrupt. Here, we toggle the LED level when the interrupt is detected.
		if(button_flag){
			// Clear the flag
			button_flag = false;
			
			// toggle the level
			ioport_toggle_pin_level(LED_PIN);
			
			
			// Another option would be the command:
			// ioport_set_pin_level(LED_PIN,!ioport_get_pin_level(BUTTON_PIN));
			// My rationale for doing it via the toggle instead of explicitly setting the pin level (which would never have issues with bouncing)
			// is simply that the toggle method is ONLY possible using interrupts, whereas the explicit setting method can be implemented
			// without interrupts
		}	
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Appendix: Code that may be useful to reference later on
/* //EXAMPLE: Blinking LED
	ioport_set_pin_level(LED_PIN,HIGH);
	delay_ms(500);
	ioport_set_pin_level(LED_PIN,LOW);
	delay_ms(500);		*/
		
	/*// EXAMPLE: if button pushed, turn on the LED
	ioport_set_pin_level(LED_PIN,!ioport_get_pin_level(BUTTON_PIN));*/
