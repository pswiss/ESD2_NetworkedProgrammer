#ifndef PROGRAMMERINTERFACE_H_
#define PROGRAMMERINTERFACE_H_

// Boilerplate Include /////////////////////////////////////////////////
#include <asf.h>

// Include necessary libraries /////////////////////////////////////////

// Constants ///////////////////////////////////////////////////////////
// Pin Definitions

// Communication Constants

// Commonly-Used Communication Messages

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

#endif /* PROGRAMMERINTERFACE_H_ */