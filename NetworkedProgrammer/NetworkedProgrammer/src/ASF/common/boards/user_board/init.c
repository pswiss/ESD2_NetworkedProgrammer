// Pin definitions and board setup

#include <asf.h>
#include <board.h>
#include <conf_board.h>


void board_init(void)
{
	ioport_init();
	ioport_set_pin_dir(PIN_WIFI_RESET,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_WIFI_RESET,LOW);
	
	ioport_set_pin_dir(PIN_WIFI_NETWORK_STATUS,IOPORT_DIR_INPUT);
	//ioport_set_pin_level(PIN_WIFI_NETWORK_STATUS,LOW);
	
	ioport_set_pin_dir(PUSH_BUTTON,IOPORT_DIR_INPUT);
	//ioport_set_pin_level(PUSH_BUTTON,HIGH);
	
	ioport_set_pin_dir(WIFI_COM_COMPLETE,IOPORT_DIR_INPUT);
	//ioport_set_pin_level(WIFI_COM_COMPLETE,LOW);



}
