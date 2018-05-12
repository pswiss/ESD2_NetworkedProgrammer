/*///////////////////////////////////////////////////////////////////
Author: Petras Swissler
---------------------------------------------------------------------
wifi.c contains
WiFi variable initializations.
void WIFI USART HANDLER(void)
void process incoming byte wifi(uint8 t in byte)
void wifi command response handler(uint32 t ul id, uint32 t ul mask)
void process data wifi (void)
void wifi web setup handler(uint32 t ul id, uint32 t ul mask)
void configure usart wifi(void)
void configure wifi comm pin(void)
void configure wifi web setup pin(void)
void write wifi command(char* comm, uint8 t cnt)
void write image to file(void)
//////////////////////////////////////////////////////////////////*/
#include "wifi.h"

// Variables for wifi communications
volatile uint32_t received_byte_wifi = 0;
volatile bool new_rx_wifi = false;
volatile uint8_t buffer_wifi[MAX_INPUT_WIFI];
volatile uint32_t input_pos_wifi = 0;
volatile uint8_t counts = 0;
volatile bool wifi_setup_flag = false;
volatile uint32_t receivedMessage = NO_MESSAGE;
volatile uint8_t rawRecievedMessage[maxWifiMessage];

// Variables for Hex Files
volatile uint32_t linesInHexFile;
volatile uint8_t hexfile[100000];


/////////////////////////////////////////////////////////////////////
/*
Handler for incoming data from the WiFi. Should call
process incoming byte wifi when a new byte arrives
*/
void WIFI_USART_HANDLER(void) 
{
	uint32_t ul_status;
	
	/* Read USART status. */
	ul_status = usart_get_status(BOARD_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(BOARD_USART, &received_byte_wifi);
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}

/*
Stores every incoming byte (in byte) from the AMW136 in a buffer.
*/
void process_incoming_byte_wifi(uint8_t in_byte) 
{
	buffer_wifi[input_pos_wifi] = in_byte;
	input_pos_wifi++;
}

/*
Handler for command complete rising-edge interrupt from AMW136.
When this is triggered, it is time to process the response of the AMW136.
*/
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask) 
{
	unused(ul_id);
	unused(ul_mask);

	process_data_wifi();
	
	// reset the buffer
	input_pos_wifi = 0;
	for(uint32_t ii = 0 ;ii < MAX_INPUT_WIFI; ii++){
		buffer_wifi[ii] = 0;
	}
	
}

/*
Processes the response of the AMW136, which should be stored in the buffer
filled by process incoming byte wifi. This processing should be looking for certain
responses that the AMW136 should give, such as start transfer when it is ready to
receive the image.
*/
void process_data_wifi (void) 
{
	strcpy(rawRecievedMessage,buffer_wifi);
	// Compare the received string with some other string
	/*if(strstr(buffer_wifi, "StringToCompare")){
		// set receivedMessage variable as appropriate
	}*/
	if(strstr(buffer_wifi,msg_START_TRANSFER)){
		receivedMessage = START_TRANSFER;
	}
	else{
    	if(strstr(buffer_wifi,msg_CLIENT_NOT_CONNECTED)){
			receivedMessage = CLIENT_NOT_CONNECTED	;
		}
		else{
			if(strstr(buffer_wifi,msg_COMMAND_FAILED)){
				receivedMessage = COMMAND_FAILED;
			}
			else{
				if(strstr(buffer_wifi,msg_RECIEVE_NONE)){
					receivedMessage = RECIEVE_NONE;
				}
				else{
					receivedMessage = DEFAULT;
				}
			}
		}
	}
}

/*
Handler for button to initiate web setup of AMW136. Should set a flag indicating a
request to initiate web setup
*/
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask) 
{
	unused(ul_id);
	unused(ul_mask);

	wifi_setup_flag = true;
}

/*
Configuration of USART port used to communicate with the AMW136.
*/
void configure_usart_wifi(void) 
{
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		BOARD_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(BOARD_ID_USART);

	/* Configure USART. */
	usart_init_hw_handshaking(BOARD_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(BOARD_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(BOARD_USART);
	usart_enable_rx(BOARD_USART);

	usart_enable_interrupt(BOARD_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(USART_IRQn);

	// Configure the RX / TX pins
	/* Configure USART RXD pin */
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	/* Configure USART TXD pin */
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	/* Configure USART CTS pin */
	//gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	ioport_set_pin_dir(PIN_USART0_CTS_IDX,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_USART0_CTS_IDX,false);
	/* Configure USART RTS pin */
	gpio_configure_pin(PIN_USART0_RTS_IDX, PIN_USART0_RTS_FLAGS);
}

/*
Configuration of command complete rising-edge interrupt.
*/
void configure_wifi_comm_pin(void) 
{
	/* Configure PIO clock. */
	
	pmc_enable_periph_clk(WIFI_COM_COMPLETE_ID);
	
	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WIFI_COM_COMPLETE_PIO, WIFI_COM_COMPLETE_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h	**/
	pio_handler_set(WIFI_COM_COMPLETE_PIO, WIFI_COM_COMPLETE_ID, WIFI_COM_COMPLETE_MSK,
			WIFI_COM_COMPLETE_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COM_COMPLETE_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COM_COMPLETE_PIO, WIFI_COM_COMPLETE_MSK);
}

/*
Configuration of button interrupt to initiate web setup.
*/
void configure_wifi_web_setup_pin(void) 
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(PUSH_BUTTON_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h**/
	pio_handler_set(PUSH_BUTTON_PIO, PUSH_BUTTON_ID, PUSH_BUTTON_PIN_MSK,
			PUSH_BUTTON_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)PUSH_BUTTON_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(PUSH_BUTTON_PIO, PUSH_BUTTON_PIN_MSK);
}

/*
Writes a command (comm) to the AMW136, and waits either for an acknowledgment
or a timeout. The timeout can be created by setting the global variable counts
to zero, which will automatically increment every second, and waiting while counts < cnt.
*/
void write_wifi_command(char* comm, uint8_t cnt) 
{
	counts = 0;
	// send a message via USART:
	//usart_write_line(BOARD_USART, "string to write\r\n");
	usart_write_line(BOARD_USART, comm);
	receivedMessage = NO_MESSAGE;
	// Wait for timeout or received message
	while((counts<cnt)&&(receivedMessage==NO_MESSAGE)){
		// Do nothing
		int dv = 0;
		dv++;
	}
}

// Simple function to reset the wifi
void resetWifi(void){
	// Reset the wifi by pulling the wifi reset pin low, then bringing it back high.
	ioport_set_pin_level(PIN_WIFI_RESET,LOW); //reset WIFI
	delay_ms(1000);
	ioport_set_pin_level(PIN_WIFI_RESET,HIGH); //turn Wifi Back on
	delay_ms(2000); // Account for ~0.7s high during reset
}

void writeWifiConfigurationCommands(void){
	///////////////////////////////////////////////////////////////////////
	// Write all wifi chip setup commands
	///////////////////////////////////////////////////////////////////////
	write_wifi_command("set system.cmd.echo off\r\n",2);
	write_wifi_command("set uart.flow 0 on\r\n",2);
	write_wifi_command("set bu c r 10000\r\n",2);
	write_wifi_command("set system.indicator.gpio wlan 20\r\n",2);
	write_wifi_command("set system.indicator.gpio network 18\r\n",2);
	write_wifi_command("set system.indicator.gpio softap 21\r\n",2);
	write_wifi_command("set system.cmd.gpio 16\r\n",2);
	write_wifi_command("set wlan.network.status_gpio 14\r\n",2);
	write_wifi_command("save\r\n",2);
	delay_ms(100);
}

// Don't do anything until the wifi chip has connected to the network
void waitForWifiNetworkConnect(void){
	ioport_set_pin_dir(PIN_WIFI_NETWORK_STATUS,IOPORT_DIR_INPUT);
	while(ioport_get_pin_level(PIN_WIFI_NETWORK_STATUS)==0){
		// If user has pushed the button to start wifi setup, do so.
		if(wifi_setup_flag == true){

			write_wifi_command("setup web\r\n",3);
			// Clear the flag
			wifi_setup_flag = false;
			waitForWifiNetworkConnect();
		}
	}
}

// Check whether the go file exists, and its contents
uint8_t checkGoFile(void){

	write_wifi_command("bump\r\n",3);
	// reset the wifi buffer
	input_pos_wifi = 0;
	for(uint32_t ii = 0 ;ii < MAX_INPUT_WIFI; ii++){
		buffer_wifi[ii] = 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	uint8_t fileStream[maxWifiMessage];
	for(int ii = 0; ii< maxWifiMessage; ii++){
		fileStream[ii] = 0x20;
	}
	// Send command to the wifi chip telling it to open the file
	write_wifi_command("fop test.txt\r\n",3);

	// save the output
	int ii = 9;
	while((rawRecievedMessage[ii]!=93)&&(ii<100)){
		fileStream[ii-9] = rawRecievedMessage[ii];
		ii++;
	}
	/////////////////////////////////////////////////////////////////////////////

	// reset the wifi buffer
	input_pos_wifi = 0;
	for(uint32_t ii = 0 ;ii < MAX_INPUT_WIFI; ii++){
		buffer_wifi[ii] = 0;
	}
	// save the output, remove unneeded portions of the file
	uint8_t fileSize[maxWifiMessage];
	for(int ii = 0; ii< maxWifiMessage; ii++){
		fileSize[ii] = 0x20;
	}
	// get the size of the file
	write_wifi_command("fst test.txt\r\n",3);
	
	// save the output
	ii = 0;
	while(rawRecievedMessage[ii]!=44){
		fileSize[ii] = rawRecievedMessage[ii];
		ii++;
	}

	/////////////////////////////////////////////////////////////////////
	// Read the file
	// Create the command string
	uint8_t readFileCommand[100];
	strcpy(readFileCommand,"read ");
	for(int ii = 4; ii < sizeof(readFileCommand); ii++){
		readFileCommand[ii] = 32;
	}

	// Check for space, add file stream to string
	ii = 0;
	while(fileStream[ii] != 32){
		readFileCommand[ii+5] = fileStream[ii];
		ii++;
	}

	readFileCommand[5+ii] = 32;
	int sizetrack = 5+ii+1;

	
	ii = 0;
	// Check for space, add file size to string
	while((fileSize[ii]!=32)||(ii==0)){
		readFileCommand[ii+sizetrack] = fileSize[ii];
		ii++;
	}

	sizetrack = sizetrack+ii;

	readFileCommand[sizetrack+1]=13;
	readFileCommand[sizetrack+2]=10;

	sizetrack = sizetrack+3;
	uint8_t realReadFileCommand[sizetrack];
	for(int jj = 0; jj < sizetrack; jj++){
		realReadFileCommand[jj] = readFileCommand[jj];
	}

	// reset the wifi buffer
	input_pos_wifi = 0;
	for(uint32_t ii = 0 ;ii < MAX_INPUT_WIFI; ii++){
		buffer_wifi[ii] = 0;
	}
	// Get the file
	write_wifi_command(realReadFileCommand,3);
	
	if(strstr(rawRecievedMessage,"Petras")){
		return 1;
		
	}
	else{
		return 0;
		
	}
	




}

// Load the hex file into memory
void loadHexFile(void){
	// Load the stuff

}