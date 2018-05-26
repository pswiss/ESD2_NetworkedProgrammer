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
	
	// Configure SWD Stuff
	setupSWDPins();
	
	// Reset the wifi
	resetWifi();
	delay_ms(8000);
	//writeWifiConfigurationCommands();
	
	// Main Loop //////////////////////////////////////////////////////////////////////////////////////////////////
	while(1){
		// If GoFile says go, then do things, otherwise wait
		//if(checkGoFile()){
		if(1){
			//Load_Hex_File();
			Clear_Target();
			//SWD_Start();
			//SWD_Program();
			//SWD_Cleanup();
		}
		else{
			delay_ms(5000);
		}
		delay_ms(5000);///xxxxxxxxxxxxxxxxxxxxxDELETE_MExxxxxxxxxxxxx
	}
}
