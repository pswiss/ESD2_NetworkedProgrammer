#ifndef PROGRAMMERINTERFACE_H_
#define PROGRAMMERINTERFACE_H_

// Boilerplate Include /////////////////////////////////////////////////
#include <asf.h>

// Include necessary libraries /////////////////////////////////////////

// Constants ///////////////////////////////////////////////////////////
// Pin Definitions ALL THIS NEEDS TO BE UPDATED
// SW CLK Definitions		OUTPUT
#define SWCLK_ID            ID_PIOB
#define SWCLK_PIO           PIOB
#define SWCLK_PIN_MSK		PIO_PB4
#define SWCLK_PIN			PIO_PB4_IDX

// SW IO Definitions		INPUT / OUTPUT
#define SWIO_ID				ID_PIOB
#define SWIO_PIO			PIOB
#define SWIO_PIN_MSK		PIO_PB5
#define SWIO_PIN			PIO_PB5_IDX

// Force Clear Memory Pin	OUTPUT
#define MEMCLR_ID			ID_PIOB
#define MEMCLR_PIO			PIOB
#define MEMCLR_PIN_MSK		PIO_PB6
#define MEMCLR_PIN			PIO_PB6_IDX

// Force Reset Pin			OUTPUT
#define FORCERST_ID			ID_PIOB
#define FORCERST_PIO		PIOB
#define FORCERST_PIN_MSK	PIO_PB7
#define FORCERST_PIN		PIO_PB7_IDX

// Communication Constants
// Communication phase bit lengths
#define LENGTH_SWD_REQUEST	8
#define LENGTH_SWD_ACK		3
#define LENGTH_SWD_DATA		32

// Timings
//us
#define DURATION_SWCLK_HIGH	10
#define DURATION_SWCLK_LOW	90

#define DURATION_ACKWAIT	1000

//ms
#define DURATION_CLEAR		100
#define DURATION_AFTER_WAIT	5

// Commonly-Used Communication Messages
// ACK MESSAGES
#define MSG_ACK_OK			0b00000001
#define MSG_ACK_WAIT		0b00000010
#define MSG_ACK_FAULT		0b00000100

// SWD Requests
#define RQ_AP_READ_BASE		0b11101101
#define RQ_AP_READ_BD0		0b11100001
#define RQ_AP_READ_BD1		0b11110001
#define RQ_AP_READ_BD2		0b11101101
#define RQ_AP_READ_BD3		0b11111001
#define RQ_AP_READ_DRW		0b11111001

#define RQ_AP_WRITE_BD0		0b11000101
#define RQ_AP_WRITE_BD1		0b11010001
#define RQ_AP_WRITE_BD2		0b11001001
#define RQ_AP_WRITE_BD3		0b11011101
#define RQ_AP_WRITE_CSW		0b11000101
#define RQ_AP_WRITE_DRW		0b11011101
#define RQ_AP_WRITE_TAR		0b11010001

#define RQ_DP_READ_CTRLSTAT	0b10110001
#define RQ_DP_READ_IDCODE	0b10100101
#define RQ_DP_READ_RDBUFF	0b10111101

#define RQ_DP_WRITE_ABORT	0b10000001
#define RQ_DP_WRITE_CTRLSTAT	0b10010101
#define RQ_DP_WRITE_SELECT	0b10001101

// STARTUP
#define STARTUP_HIGH_1		56
#define STARTUP_MSG_1		0b111100111100
#define STARTUP_MSGLEN_1	12

#define STARTUP_HIGH_2		59
#define STARTUP_MSG_2		0b0110110110110
#define STARTUP_MSGLEN_2	13

#define STARTUP_HIGH_3		59
#define STARTUP_MSG_3		0b0000000000000000
#define STARTUP_MSGLEN_3	16

// Bit Masks
#define MASK_32BIT_1		0x00000001
#define MASK_8BIT_1			0b00000001

// Generic Messages
#define MSG_NULL			0

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