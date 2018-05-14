#ifndef PROGRAMMERINTERFACE_H_
#define PROGRAMMERINTERFACE_H_

// Boilerplate Include /////////////////////////////////////////////////
#include <asf.h>

// Include necessary libraries /////////////////////////////////////////

// Constants ///////////////////////////////////////////////////////////
// Pin Definitions

// Communication Constants
// Communication phase bit lengths
#define LENGTH_SWD_REQUEST	8
#define LENGTH_SWD_ACK		3
#define LENGTH_SWD_DATA		32

// Commonly-Used Communication Messages
#define MSG_ACK_OK			0b00000001
#define MSG_ACK_WAIT		0b00000010
#define MSG_ACK_FAULT		0b00000100

// Bit Masks
#define MASK_32BIT_1		0x00000001
#define MASK_8BIT_1			0b00000001

// Variables ///////////////////////////////////////////////////////////
// Global Variables

// Interrupt Flags

// Messages

// Function Prototypes /////////////////////////////////////////////////
void setupSWDPins(void);
void configSWDPinsInput(void);
void Clear_Target(void);
void SWD_Start(void);
void SWD_Program(void);
void SWD_Cleanup(void);

void SWD_bitOut(Bool );
uint8_t SWD_bitIn(void );
void SWD_bitTurn(void);

#endif /* PROGRAMMERINTERFACE_H_ */