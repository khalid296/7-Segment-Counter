/************************************************
 *	 File Name: External_Interrupts.c			*
 * Description: 								*
 *  Created on: Oct 1, 2018						*
 *      Author: Khalid Tarek					*
 ************************************************/

#include "External_Interrupts.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variable array of pointer to function to hold
 * the addresses of the call back functions in the application */
static volatile void (*g_callBackPtr[3])(void) = {NULL_PTR, NULL_PTR, NULL_PTR};

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(INT0_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[0] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[0])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(INT1_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[1] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[1])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(INT2_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[2] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[2])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the Interrupt driver
 * 	1. Set the required clock.
 * 	2. Set the required Interrupt.
 * 	3. Enable the Interrupt Interrupt.
 * 	4. Initialize Interrupt Registers.
 */
void Interrupt_init(const Interrupt_ConfigType * Config_Ptr)
{
	CLEAR_BIT(SREG,7);      // Disable interrupts by clearing I-bit

	switch(Config_Ptr->interrupt){
		case Interrupt0:
			CLEAR_BIT(DDRD,PD2);               // Configure INT0/PD2 as input pin
			SET_BIT(GICR,INT0);                 // Enable external interrupt pin INT0
			// Trigger INT0 with the required mode
			MCUCR = (MCUCR & 0xFC) | ((Config_Ptr->mode & 0x03));
			break;
		case Interrupt1:
			CLEAR_BIT(DDRD,PD3);               // Configure INT1/PD3 as input pin
			SET_BIT(GICR,INT1);                 // Enable external interrupt pin INT1
			// Trigger INT1 with the required mode
			MCUCR = (MCUCR & 0xF3) | ((Config_Ptr->mode & 0x03)<<ISC10);
			break;
		case Interrupt2:
			CLEAR_BIT(DDRB,PB2);               // Configure INT2/PB2 as input pin
			SET_BIT(GICR,INT2);                 // Enable external interrupt pin INT2
			// Trigger INT2 with the required mode
			MCUCSR = (MCUCSR & 0xBF) | ((Config_Ptr->INT2_mode & 0x01)<<ISC2);
			break;
	}

	SET_BIT(SREG,7);       // Enable interrupts by setting I-bit
}

/*
 * Description: Function to set the Call Back function address.
 */
void Interrupt_setCallBack(Interrupt_Select Interrupt, void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr[Interrupt] = a_ptr;
}

/*
 * Description: Function to disable the Interrupt1 to stop the ICU Driver
 */
void Icu_DeInit(Interrupt_Select Interrupt)
{
	switch(Interrupt){
		case Interrupt0:
			CLEAR_BIT(GICR,INT0);       // Disable external interrupt pin INT0
			MCUCR = (MCUCR & 0xFC);		// Clear mode bits
			break;
		case Interrupt1:
			CLEAR_BIT(GICR,INT1);       // Disable external interrupt pin INT0
			MCUCR = (MCUCR & 0xF3);		// Clear mode bits
			break;
		case Interrupt2:
			SET_BIT(GICR,INT0);                 // Enable external interrupt pin INT2
			MCUCSR = (MCUCSR & 0xFE);	// Clear mode bit
			break;
	}
}
