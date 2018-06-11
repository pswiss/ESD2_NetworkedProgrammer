/*///////////////////////////////////////////////////////////////////
Author: Petras Swissler
//////////////////////////////////////////////////////////////////*/
#include "Programmer_USART.h"

// Variables for prog communications
volatile uint32_t received_byte_prog = 0;
volatile bool new_rx_prog = false;
volatile uint8_t buffer_prog[MAX_INPUT_PROG];
volatile uint32_t input_pos_prog = 0;

volatile bool prog_setup_flag = false;
volatile uint32_t receivedMessage_prog = NO_MESSAGE;
volatile uint8_t rawRecievedMessage_prog[maxProgMessage];

// Variables for Hex Files
volatile uint32_t linesInHexFile;

/////////////////////////////////////////////////////////////////////
/*
Handler for incoming data from the prog. Should call
process incoming byte prog when a new byte arrives
*/
void PROG_USART_HANDLER(void) 
{
	uint32_t ul_status;
	
	/* Read USART status. */
	ul_status = usart_get_status(BOARD_USART_PROG);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(BOARD_USART_PROG, &received_byte_prog);
		new_rx_prog = true;
		process_incoming_byte_prog((uint8_t)received_byte_prog);
	}
}

/*
Stores every incoming byte (in byte) from the AMW136 in a buffer.
*/
void process_incoming_byte_prog(uint8_t in_byte) 
{
	buffer_prog[input_pos_prog] = in_byte;
	input_pos_prog++;
}

/*
Handler for command complete rising-edge interrupt from AMW136.
When this is triggered, it is time to process the response of the AMW136.
*/
void prog_command_response_handler(uint32_t ul_id, uint32_t ul_mask) 
{
	unused(ul_id);
	unused(ul_mask);

	process_data_prog();
	
	// reset the buffer
	input_pos_prog = 0;
	for(uint32_t ii = 0 ;ii < MAX_INPUT_PROG; ii++){
		buffer_prog[ii] = 0;
	}
}

/*
Processes the response of the AMW136, which should be stored in the buffer
filled by process incoming byte prog. This processing should be looking for certain
responses that the AMW136 should give, such as start transfer when it is ready to
receive the image.
*/
void process_data_prog (void) 
{
	receivedMessage_prog = DEFAULT;
}

/*
Configuration of USART port used to communicate with the AMW136.
*/
void configure_usart_prog(void) 
{
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		BOARD_USART_PROG_BAUDRATE,
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
	sysclk_enable_peripheral_clock(BOARD_ID_USART_PROG);

	/* Configure USART. */
	usart_init_hw_handshaking(BOARD_USART_PROG, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(BOARD_USART_PROG, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(BOARD_USART_PROG);
	usart_enable_rx(BOARD_USART_PROG);

	usart_enable_interrupt(BOARD_USART_PROG, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(USART_IRQn_PROG);

	// Configure the RX / TX pins
	/* Configure USART RXD pin */
	gpio_configure_pin(PIN_USART1_RXD_IDX, PIN_USART1_RXD_FLAGS);
	/* Configure USART TXD pin */
	gpio_configure_pin(PIN_USART1_TXD_IDX, PIN_USART1_TXD_FLAGS);

	/* Configure USART CTS pin */
	//gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	ioport_set_pin_dir(PIN_USART1_CTS_IDX,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_USART1_CTS_IDX,false);
	/* Configure USART RTS pin */
	gpio_configure_pin(PIN_USART1_RTS_IDX, PIN_USART1_RTS_FLAGS);

}

/*
Writes a command (comm) and waits either for an acknowledgment
or a timeout. The timeout can be created by setting the global variable counts
to zero, which will automatically increment every second, and waiting while counts < cnt.
*/
void write_prog_command(char* comm, uint8_t cnt) 
{

	// send a message via USART:
	//usart_write_line(BOARD_USART_PROG, "string to write\r\n");
	usart_write_line(BOARD_USART_PROG, comm);
	
	delay_ms(5);

}
