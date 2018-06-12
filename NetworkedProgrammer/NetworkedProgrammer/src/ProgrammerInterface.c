// Include Libraries
#include "ProgrammerInterface.h"
//#include "hardcodedprogram.h"

// Global Variables
volatile uint8_t buffer_program[MAX_PROGRAM_SIZE] = hardprogram;

//////////////////////////////////////////////////////////
// Functions /////////////////////////////////////////////
//////////////////////////////////////////////////////////

// USART Functions ///////////////////////////////////////

// Helper Functions //////////////////////////////////////
// Convert from ASCII to number (supports up to base 16)
uint8_t ASCII_to_Num(uint8_t inputChar){
	switch(inputChar){
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'A':
			return 10;
		case 'B':
			return 11;
		case 'C':
			return 12;
		case 'D':
			return 13;
		case 'E':
			return 14;
		case 'F':
			return 15;
	}
}

// Take in hex number and output the appropriate ASCII Character
uint8_t Num_to_ASCII(uint8_t inputNum){
	switch(inputNum){
		case 0x00:
			return '0';
		case 0x01:
			return '1';
		case 0x02:
			return '2';
		case 0x03:
			return '3';
		case 0x04:
			return '4';
		case 0x05:
			return '5';
		case 0x06:
			return '6';
		case 0x07:
			return '7';
		case 0x08:
			return '8';
		case 0x09:
			return '9';
		case 0x0A:
			return 'A';
		case 0x0B:
			return 'B';
		case 'C':
			return 'C';
		case 0x0D:
			return 'D';
		case 0x0E:
			return 'E';
		case 0x0F:
			return 'F';
	}
}

// Programmer Functions ///////////////////////////////////
// Interpret the given program
void Write_Program(void){// eventually put input file here?
	uint8_t EOF_reached = 0;
	uint32_t current_character = 0;
	uint32_t current_line = 0;

	// A few initialization writes
	uint8_t initWrites[5];
	sprintf(initWrites,"T#");
	write_prog_command(&initWrites,0,0);
	delay_ms(20);

	int aaaa = 0;
	
	// Go through entire program
	while(EOF_reached!=1){
		
		// Check if I am looking at the start of the line
		if(buffer_program[current_character]==':'){

			if(current_line>300){
				aaaa++;
			}

			// Check the byte count
			uint32_t byteCount = ASCII_to_Num(buffer_program[current_character+1])*16+ASCII_to_Num(buffer_program[current_character+2]);
			
			uint32_t addressValue = 0;
			uint8_t byteToWrite = 0;

			// Check the record type
			switch(ASCII_to_Num(buffer_program[current_character+OFFSET_RECORDTYPE+1])){
				case HEX_DATA:
					// Reconstruct the value of the address
					addressValue = PROGRAM_MEMORY_START + 
										ASCII_to_Num(buffer_program[current_character+OFFSET_ADDRESS+0])*0x1000 +
										ASCII_to_Num(buffer_program[current_character+OFFSET_ADDRESS+1])*0x100 +
										ASCII_to_Num(buffer_program[current_character+OFFSET_ADDRESS+2])*0x10 +
										ASCII_to_Num(buffer_program[current_character+OFFSET_ADDRESS+3]);

					// Loop through for each byte to send and send it
					for(uint32_t jj = 0; jj < byteCount; jj++){
						// Read in the byte to write
						byteToWrite = ASCII_to_Num(buffer_program[current_character+OFFSET_DATA+jj*2])*0x10 +
										ASCII_to_Num(buffer_program[current_character+OFFSET_DATA+jj*2+1]);
						
						// Create the string to write
						// Format for writing a byte: 'O'AAAAAA','CC'#'
						uint8_t boot_write_string[11];
						sprintf(boot_write_string, "O%06x,%02x#",addressValue+jj,byteToWrite);

						// Write the string to the attached chip
						write_prog_command(&boot_write_string, 1, 1);
					}
					break;
				case HEX_EOF:
					EOF_reached = 1;
					break;
				case HEX_EXTENDED_SEGMENT_ADDRESS:
					// Not implemented. The data field contains a 16-bit segment base address (thus byte count is 02) compatible with 80x86 real mode addressing. The address field (typically 0000) is ignored. The segment address from the most recent 02 record is multiplied by 16 and added to each subsequent data record address to form the physical starting address for the data. This allows addressing up to one megabyte of address space.
					break;
				case HEX_START_SEGMENT_ADDRESS:
					// Not implemented. For 80x86 processors, specifies the initial content of the CS:IP registers. The address field is 0000, the byte count is 04, the first two bytes are the CS value, the latter two are the IP value.
					break;
				case HEX_EXTENDED_LINEAR_ADDRESS:
					// Not implemented. Allows for 32 bit addressing (up to 4GiB). The address field is ignored (typically 0000) and the byte count is always 02. The two encoded, big endian data bytes specify the upper 16 bits of the 32 bit absolute address for all subsequent type 00 records; these upper address bits apply until the next 04 record. If no type 04 record precedes a 00 record, the upper 16 address bits default to 0000. The absolute address for a type 00 record is formed by combining the upper 16 address bits of the most recent 04 record with the low 16 address bits of the 00 record.
					break;
				case HEX_START_LINEAR_ADDRESS:
					// Not implemented. The address field is 0000 (not used) and the byte count is 04. The four data bytes represent the 32-bit value loaded into the EIP register of the 80386 and higher CPU.
					break;
			}
			
			// Jump to next colon
			current_character = current_character+OFFSET_DATA+2+byteCount;
			current_line++;
			
		}
		else{
			// Otherwise increment what character I'm looking at
			current_character += 1;
		}
	}
	Reset_Target();
	return 1;
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

// Reset the target device
void Reset_Target(void){
	ioport_set_pin_level(FORCERST_PIN,LOW);
	delay_ms(DURATION_CLEAR);
	ioport_set_pin_level(FORCERST_PIN,HIGH);
}
