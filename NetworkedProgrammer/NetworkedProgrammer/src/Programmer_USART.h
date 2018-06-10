// Author: Petras Swissler
////////////////////////////////////////////////////////////////////////////////////////////////////
/* This file contains all functions / pin definitions etc for the programemer USART communications
////////////////////////////////////////////////////////////////////////////////////////////////////
*/

#ifndef PROGRAMMER_USART_H_
#define PROGRAMMER_USART_H_

// boilerplate include
#include <asf.h>

// Include necessary libraries
#include <string.h>
#include <stdio.h>

// Pin Definitions: USART
/** USART1 pin RX */
#define PIN_USART1_RXD    {PIO_PA21A_RXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_RXD_IDX        (PIO_PA21_IDX)
#define PIN_USART1_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin TX */
#define PIN_USART1_TXD    {PIO_PA22A_TXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_TXD_IDX        (PIO_PA22_IDX)
#define PIN_USART1_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
// CTS and RTS not supported

// Communication Constants
#define BOARD_ID_USART_PROG			ID_USART1
#define BOARD_USART_PROG			USART1
#define BOARD_USART_PROG_BAUDRATE	115200
#define PROG_USART_HANDLER			USART1_Handler
#define USART_IRQn_PROG				USART1_IRQn
#define ALL_INTERRUPT_MASK			0xffffffff
#define MAX_INPUT_PROG				50
#define PROG_RX_BUFF_SIZE			50


////////////////////////////////////////////////////////////////////////////////////////////////////
// Variables
volatile uint32_t received_byte_prog;
volatile bool new_rx_prog;
volatile uint8_t buffer_prog[MAX_INPUT_PROG];
volatile uint32_t input_pos_prog;

// Variables: interrupt flag
volatile bool prog_setup_flag;

// Messages
#define maxProgMessage				50
#define NO_MESSAGE					0

#define NOT_CONNECTED				1
#define msg_NOT_CONNECTED			"Client not connected"

#define START_TRANSFER				2
#define msg_START_TRANSFER			"Start transfer"

#define TRANSFER_COMPLETE			3
#define msg_COMPLETE				"Complete"

#define CLIENT_NOT_CONNECTED		4
#define msg_CLIENT_NOT_CONNECTED	"Client not connected"

#define COMMAND_FAILED				5
#define msg_COMMAND_FAILED			"Command failed"

#define RECIEVE_NONE				6
#define msg_RECIEVE_NONE			"None"

#define msg_WEBSOCKET_DISCONNECTED	"Websocket disconnected"
#define msg_UNKNOWN_COMMAND			"Unknown command"

#define DEFAULT						4000

volatile uint32_t uintreceivedMessage_prog_prog;
volatile uint8_t rawRecievedMessage_prog[maxProgMessage];

////////////////////////////////////////////////////////////////////////////////////////////////////
// General Function Prototypes
void PROG_USART_HANDLER(void);
void process_incoming_byte_prog(uint8_t);
void prog_command_response_handler(uint32_t, uint32_t);
void process_data_prog (void);
void prog_web_setup_handler(uint32_t, uint32_t);
void configure_usart_prog(void);
void configure_prog_comm_pin(void);
void configure_prog_web_setup_pin(void);
void write_prog_command(char* , uint8_t);


#endif /* PROGRAMMER_USART_H_ */