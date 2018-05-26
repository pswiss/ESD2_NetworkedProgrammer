// Author: Petras Swissler
// Task 3
////////////////////////////////////////////////////////////////////////////////////////////////////
/* File: button.c
This file contains two functions: button_handler() handles behavior when the interrupt on 
the pin is activated; configure_button() performs the actual actions necessary to configure the 
button pin
////////////////////////////////////////////////////////////////////////////////////////////////////
*/
// Include the associated .h file
#include "button.h"

// Variables
volatile uint32_t button_flag = false;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Button handler: what happens when the interrupt is triggered?
static void button_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	button_flag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// configures the button
void configure_button(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(PUSH_BUTTON_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(PUSH_BUTTON_PIO, PUSH_BUTTON_ID, PUSH_BUTTON_PIN_MSK,
			PUSH_BUTTON_ATTR, button_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)PUSH_BUTTON_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK);
}

