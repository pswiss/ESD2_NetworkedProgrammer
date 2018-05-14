// Include Libraries
#include ProgrammerInterface.h

// Functions /////////////////////////////////////////////
// Setup Pins
void setupSWDPins(void){

}

// Configure SWD pins to be inputs
void configSWDPinsInput(void){
	// Communication Pins Input
}

// Configure SWD pins to be outputs
void configSWDPinsOutput(void){
	// Communication Pins Output
}

// Clear the target device
void Clear_Target(void){

}

// Perform all start configurations for SWD
void SWD_Start(void ){

}

// Program the hex file
void SWD_Program(void ){

}

// Perform all end configurations for SWD
void SWD_Cleanup(void ){

}

// Bit-Bang Functions //////////////////////////////////////

// Bitbang: Write Bit over SWD
void SWD_bitOut(Bool outBit){
	
	
}


// Bitbang: Read Bit over SWD
uint8_t SWD_bitIn(void){
	
}

// Bitbang: Turnaround Bit
void SWD_bitTurn(void){
	
}

// Return the level of the ith bit of a given value
Bool ithBitLevel(uint32_t checkByte, uint8_t ii){
	// Bit shift the checkByte right, then use bit mask
	return (((checkByte >> ii)&&MASK_32BIT_1)!=0)
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