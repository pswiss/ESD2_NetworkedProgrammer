// Include Libraries
#include "ProgrammerInterface.h"

// Global Variables


// Functions /////////////////////////////////////////////
// Setup Pins
void setupSWDPins(void){
	// Set all pins as output, low
	ioport_set_pin_dir(SWCLK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(SWCLK_PIN, LOW);
	
	ioport_set_pin_dir(SWIO_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(SWIO_PIN, LOW);
	
	ioport_set_pin_dir(MEMCLR_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(MEMCLR_PIN, LOW);
	
	ioport_set_pin_dir(FORCERST_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(FORCERST_PIN, HIGH);
}

// Configure SWD pins to be inputs
void configSWDPinsInput(void){
	// Communication Pins Input: SWIO
	ioport_set_pin_dir(SWIO_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(SWIO_PIN, IOPORT_MODE_PULLUP);
	
}

// Configure SWD pins to be outputs
void configSWDPinsOutput(void){
	// Communication Pins Output: SWIO
	ioport_set_pin_dir(SWCLK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(SWCLK_PIN, LOW);
}

// Bit-Bang Functions //////////////////////////////////////
// Bitbang: Write Bit over SWD
void SWD_bitOut(Bool outBit){
	ioport_set_pin_level(SWIO_PIN,outBit);
	ioport_set_pin_level(SWCLK_PIN,HIGH);
	delay_us(DURATION_SWCLK_HIGH);
	
	ioport_set_pin_level(SWCLK_PIN,LOW);
	delay_us(DURATION_SWCLK_LOW);
}

// Bitbang: Read Bit over SWD
uint8_t SWD_bitIn(void){
	ioport_set_pin_level(SWCLK_PIN,HIGH);
	delay_us(DURATION_SWCLK_HIGH);

	
	
	uint8_t reportLevel = ioport_get_pin_level(SWIO_PIN);
	
	ioport_set_pin_level(SWCLK_PIN,LOW);
	delay_us(DURATION_SWCLK_LOW);
	
	return reportLevel;
}

// Bitbang: Turnaround Bit
void SWD_bitTurn(void){
	SWD_bitOut(HIGH);
}

// Return the level of the ith bit of a given value
Bool ithBitLevel(uint32_t checkByte, uint8_t ii){
	// Bit shift the checkByte right, then use bit mask
	return (((checkByte >> ii)&&MASK_32BIT_1)!=0);
}

// Bitbang: SWD Request
void SWD_sendRequest(uint8_t requestByte){
	// Set all pins to output
	configSWDPinsOutput();
	
	// Send the bits in sequence, LSB first
	for(int ii = 0; ii<LENGTH_SWD_REQUEST;ii++){
		Bool sendBit = ithBitLevel(requestByte,LENGTH_SWD_REQUEST-1-ii);
		SWD_bitOut(sendBit);
	}
}

// Bitbang: SWD Ack Read
uint8_t SWD_AckGet(void){
	uint8_t messageIn = 0;
	
	// Take in the three Ack Bits. Data comes in LSB first
	for(uint8_t ii = 0; ii<LENGTH_SWD_ACK; ii++){
		messageIn = messageIn||((SWD_bitIn()&&MASK_8BIT_1)<<ii);
	}
	
	// Behavior changes due to Ack message processed in higher function
	return messageIn;
	
}

// Bitbang: Write Data
void SWD_DataWrite(uint32_t dataToSend){
	// Set all pins to output
	configSWDPinsOutput();
	
	uint8_t checksumValue = 0;
	
	// Send the bits in sequence, LSB first. Track checksum
	for(int ii = 0; ii<LENGTH_SWD_DATA;ii++){
		Bool sendBit = ithBitLevel(dataToSend,LENGTH_SWD_DATA-1-ii);
		SWD_bitOut(sendBit);
		checksumValue+=sendBit;
	}
	
	// Implement the checksum bit: if odd, send 1, if even, send 0
	if(checksumValue&&MASK_8BIT_1){
		SWD_bitOut(1);
	}
	else{
		SWD_bitOut(0);
	}
}

//Bitbang: Read Data
uint32_t SWD_DataRead(void){
	uint32_t dataIn = 0;
	
	// Take in data Bits. Data comes in LSB first
	for(uint8_t ii = 0; ii<LENGTH_SWD_DATA; ii++){
		dataIn = dataIn||((SWD_bitIn()&&MASK_32BIT_1)<<ii);
	}
	
	// Read one more bit for the parity bit (assume that everything went OK)
	SWD_bitIn();
	
	//return the data
	return dataIn;
}


// Generic function to perform an SWD message
uint32_t SWD_Comm(uint8_t command, uint32_t data){
	// Determine if Read or Write
	if((command>>5)&&MASK_8BIT_1){
		// If 1, Read
		uint32_t okFlag = 0;
		
		while(!okFlag){
			configSWDPinsOutput();
			SWD_sendRequest(command);
			
			SWD_bitTurn();
			configSWDPinsInput();
			
			// Will break out if get Ack ok otherwise will have to repeat
			if(SWD_AckGet()==MSG_ACK_OK){
				okFlag = 1;
			}
			else{
				delay_us(DURATION_ACKWAIT);
			}
		}
		// No turn for read
		return SWD_DataRead();
	}
	else{
		// If 0, Write
		uint32_t okFlag = 0;
		
		while(!okFlag){
			configSWDPinsOutput();
			SWD_sendRequest(command);
			
			SWD_bitTurn();
			configSWDPinsInput();
			
			// Will break out if get Ack ok otherwise will have to repeat
			if(SWD_AckGet()==MSG_ACK_OK){
				okFlag = 1;
			}
			else{
				delay_us(DURATION_ACKWAIT);
			}
		}
		configSWDPinsOutput();
		SWD_bitTurn();
		SWD_DataWrite(data);
		return 1;
	}
}

// Clear the target device
void Clear_Target(void){
	ioport_set_pin_level(FORCERST_PIN,LOW);
	delay_ms(DURATION_CLEAR);
	
	ioport_set_pin_level(FORCERST_PIN,HIGH);
	ioport_set_pin_level(MEMCLR_PIN,HIGH);
	delay_ms(DURATION_CLEAR);
	
	ioport_set_pin_level(MEMCLR_PIN,LOW);
	delay_ms(DURATION_CLEAR);
}

//High Level SWD Commands /////////////////////////////////////////////////
// Perform all start configurations for SWD
void SWD_Start(void ){
	//////////////////////////////////////////////////////////////////
	// Purely an output operation
	configSWDPinsOutput();
	
	// Go from JTAG to SWD
	// Write the first reset message
	for(uint32_t ii = 0; ii < STARTUP_HIGH_1;ii++){
		SWD_bitOut(HIGH);
	}
	// Write the first message
	for(uint32_t ii = 0; ii < STARTUP_MSGLEN_1;ii++){
		SWD_bitOut(MASK_32BIT_1&&(STARTUP_MSG_1>>(STARTUP_MSGLEN_1-ii)));
	}
	
	// Write the second reset message
	for(uint32_t ii = 0; ii < STARTUP_HIGH_2;ii++){
		SWD_bitOut(HIGH);
	}
	// Write the second message
	for(uint32_t ii = 0; ii < STARTUP_MSGLEN_2;ii++){
		SWD_bitOut(MASK_32BIT_1&&(STARTUP_MSG_2>>(STARTUP_MSGLEN_2-ii)));
	}
	
	// Write the third reset message
	for(uint32_t ii = 0; ii < STARTUP_HIGH_3;ii++){
		SWD_bitOut(HIGH);
	}
	// Write the third message
	for(uint32_t ii = 0; ii < STARTUP_MSGLEN_3;ii++){
		SWD_bitOut(MASK_32BIT_1&&(STARTUP_MSG_3>>(STARTUP_MSGLEN_3-ii)));
	}
	//////////////////////////////////////////////////////////////////
	// Request device ID
	uint32_t deviceTag = SWD_Comm(RQ_DP_READ_IDCODE, MSG_NULL);

	deviceTag=0;

}


// Program the hex file
void SWD_Program(void ){

}

// Perform all end configurations for SWD
void SWD_Cleanup(void ){

}
