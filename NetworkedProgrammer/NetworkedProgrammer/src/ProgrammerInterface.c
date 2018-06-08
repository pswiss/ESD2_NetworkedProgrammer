// Include Libraries
#include "ProgrammerInterface.h"
#include "hardcodedprogram.h"

// Global Variables
volatile uint8_t buffer_program[MAX_PROGRAM_SIZE] = hardprogram;
volatile uint8_t program_addresses[PROGRAM_ADDRESS_SIZE][PROGRAM_LINE_LENGTH];
volatile uint8_t program_data[PROGRAM_DATA_SIZE][PROGRAM_LINE_LENGTH];


// Functions /////////////////////////////////////////////
// Helper functions:
// Convert from ASCII to number (supports up to base 16)
uint8_t ASCII_to_Num(uint8_t inputChar){
	switch(inputChar){
		case "0":
			return 0;
		case "1":
			return 1;
		case "2":
			return 2;
		case "3":
			return 3;
		case "4":
			return 4;
		case "5":
			return 5;
		case "6":
			return 6;
		case "7":
			return 7;
		case "8":
			return 8;
		case "9":
			return 9;
		case "A":
			return 10;
		case "B":
			return 11;
		case "C":
			return 12;
		case "D":
			return 13;
		case "E":
			return 14;
		case "F":
			return 15;
	}
}


///////////////////////////
// Interpret the given program
void Parse_Program(void){// eventually put input file here?
	uint32_t EOF_reached = 0;
	uint32_t current_character = 0;
	
	// Go through entire program
	while(EOF_reached!=1){
		// Check if I am looking at the start of the line
		if(buffer_program[current_character]==":"){
			// Check the byte count
			uint32_t byteCount = ASCII_to_Num(buffer_program[current_character+1])*16+ASCII_to_Num(buffer_program[current_character+2]);
			
			// Check the record type
			switch(buffer_program[current_character+OFFSET_RECORDTYPE]){
				case HEX_DATA:
					// Check the address, put it into the program_address array
					
					// Gather the data, put it into the program_data array
					break;
				case HEX_EOF:
					break;
				case HEX_EXTENDED_SEGMENT_ADDRESS:
					break;
				case HEX_START_SEGMENT_ADDRESS:
					break;
				case HEX_EXTENDED_LINEAR_ADDRESS:
					break;
				case HEX_START_LINEAR_ADDRESS:
					break;

			}
			
			
			
		}
		else{
			// Otherwise increment what character I'm looking at
			current_character += 1;
		}
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
