/************************************************
 *	 File Name: External_Interrupts.h			*
 * Description: 								*
 *  Created on: Oct 1, 2018						*
 *      Author: Khalid Tarek					*
 ************************************************/
#ifndef EXTERNAL_INTERRUPTS_H_
#define EXTERNAL_INTERRUPTS_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	Interrupt0, Interrupt1, Interrupt2
}Interrupt_Select;

typedef enum
{
	LOW_LEVEL, LOGICAL_CHANGE, FALLING_EDGE, RISING_EDGE
}Interrupt_Mode;

typedef enum
{
	FALLING_EDGE_2, RISING_EDGE_2
}Interrupt_2_Mode;

typedef struct
{
	Interrupt_Select interrupt;
	Interrupt_Mode mode;
	Interrupt_2_Mode INT2_mode;

}Interrupt_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the Interrupt driver
 * 	1. Select required interrupt
 * 	2. Select required mode
 */
void Interrupt_init(const Interrupt_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void Interrupt_setCallBack(Interrupt_Select Interrupt, void(*a_ptr)(void));

/*
 * Description: Function to disable the Interrupt
 */
void Interrupt_DeInit(Interrupt_Select Interrupt);


#endif /* EXTERNAL_INTERRUPTS_H_ */
