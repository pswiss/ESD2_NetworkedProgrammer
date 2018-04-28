// Include necessary libraries
#include <asf.h>
#include "wifi.h"
#include "timer_interface.h"
#include "conf_clock.h"
#include "conf_board.h"

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
	
	// Reset the wifi
	resetWifi();
	delay_ms(8000);
	//writeWifiConfigurationCommands();
	
	// Main Loop //////////////////////////////////////////////////////////////////////////////////////////////////
	while(1){

		/*// If user has pushed the button to enter web setup, enter this routine
		if(wifi_setup_flag == true){

			// Send the setup web string to the wifi chip
			
			write_wifi_command("setup web\r\n",3);

			// Clear the flag
			wifi_setup_flag = false;
			waitForWifiNetworkConnect();
		}

		// If user has not requested web setup, try to capture and display an image
		else{

			checkGoFile();
		}*/
		checkGoFile();
		delay_ms(5000);
	}
}
