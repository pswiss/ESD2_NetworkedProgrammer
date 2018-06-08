#ifndef PROGRAMMERINTERFACE_H_
#define PROGRAMMERINTERFACE_H_

// Boilerplate Include /////////////////////////////////////////////////
#include <asf.h>

// Include necessary libraries /////////////////////////////////////////
#include "hardcodedprogram.h"

#include <string.h>
#include <stdio.h>
#define <xmodem.h>

// Constants ///////////////////////////////////////////////////////////
// Pin Definitions ALL THIS NEEDS TO BE UPDATED

// Force Clear Memory Pin	OUTPUT
#define MEMCLR_ID			ID_PIOA
#define MEMCLR_PIO			PIOA
#define MEMCLR_PIN_MSK		PIO_PA1
#define MEMCLR_PIN			PIO_PA1_IDX

// Force Reset Pin			OUTPUT
#define FORCERST_ID			ID_PIOA
#define FORCERST_PIO		PIOA
#define FORCERST_PIN_MSK	PIO_PA0
#define FORCERST_PIN		PIO_PA0_IDX

// Communication Constants
// Communication sizes
#define MAX_PROGRAM_SIZE		0x40000
#define PROGRAM_ADDRESS_SIZE	4
#define PROGRAM_DATA_SIZE		16
#define PROGRAM_LINE_LENGTH		450

// Hex File Constants
#define OFFSET_BYTECOUNT		1
#define OFFSET_ADDRESS			3
#define OFFSET_RECORDTYPE		7
#define OFFSET_DATA				9

// Hex File record types
#define HEX_DATA						0
#define HEX_EOF							1
#define HEX_EXTENDED_SEGMENT_ADDRESS	2
#define HEX_START_SEGMENT_ADDRESS		3
#define HEX_EXTENDED_LINEAR_ADDRESS		4
#define HEX_START_LINEAR_ADDRESS		5

// Timings
//us

//ms
#define DURATION_CLEAR			50

// Commonly-Used Communication Messages

// Flash Memory Values
#define PROGRAM_MEMORY_START	0x00400000

// Variables ///////////////////////////////////////////////////////////
// Global Variables
volatile uint8_t buffer_program[MAX_PROGRAM_SIZE];

// Interrupt Flags

// Messages

// Function Prototypes /////////////////////////////////////////////////
void Clear_Target(void);

void configure_usart_programmer(void);
void configure_programmer_interfacePins(void);

void Load_Hex_File(void);
void Parse_Program(void);

void Write_Program(void);

void Cleanup_Program(void);

uint8_t ASCII_to_Num(uint8_t);


#endif /* PROGRAMMERINTERFACE_H_ */