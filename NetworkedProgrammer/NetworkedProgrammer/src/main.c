// Include necessary libraries
#include <asf.h>
#include "wifi.h"
#include "timer_interface.h"
#include "conf_clock.h"
#include "conf_board.h"
#include "ProgrammerInterface.h"

// Global variables because 2/3 of us are mechanical engineers and that's just how we roll
volatile uint32_t receivedMessage;


int main (void)
{
	// boilerplate
	sysclk_init();
	wdt_disable(WDT);
	board_init();
	
	
	// start the clock that counts the seconds
	configure_tc();
	tc_start(TC0, 0);

	// Configure the Wifi UART communication stuff
	configure_usart_wifi();
	configure_wifi_comm_pin();
	configure_wifi_web_setup_pin();
	
	// Configure the Wifi programming communication stuff
	//configure_usart_programmer();
	//configure_programmer_interfacePins();
	
	// Reset the wifi
	resetWifi();
	//delay_ms(8000); // RE-ENABLE ME

	// Configure programmer usart
 	configure_usart_prog();
	Clear_Target();
	Write_Program();
	
	// Main Loop //////////////////////////////////////////////////////////////////////////////////////////////////
	while(1){
		// If GoFile says go, then do things, otherwise wait
		//if(checkGoFile()){
		if(1){
			//Load_Hex_File();
			//Clear_Target();
			//Parse_Program();
			//Write_Program();
			//Cleanup_Program();
			
		}
		else{
			delay_ms(5000);
		}
		delay_ms(5000);///xxxxxxxxxxxxxxxxxxxxxDELETE_MExxxxxxxxxxxxx
	}
}
