/************************************************
 *	 File Name: timer.h							*
 * Description: 								*
 *  Created on: Sep 29, 2018					*
 *      Author: Khalid Tarek					*
 ************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	TIMER0, TIMER1A, TIMER1B, TIMER2
}Timer_Select;

typedef enum
{
	NORMAL, PWM, CTC, FAST_PWM
}Timer_Mode;

typedef enum
{
	DISCONNECTED, TOGGLE_OC, CLEAR_OC_NON_INVERTING, SET_OC_INVERTING
}Timer_Compare_Output_Mode;

typedef enum
{
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024, EXTERNAL_FALLING, EXTERNAL_RISING
}Timer_Clock;

typedef enum
{
	NO_CLOCK_2, F_CPU_CLOCK_2, F_CPU_8_2, F_CPU_32_2, F_CPU_64_2, F_CPU_128_2, F_CPU_256_2, F_CPU_1024_2
}Timer_2_Clock;

typedef enum
{
	OFF, ON
}Timer_Interrupt;

typedef enum
{
	NORMAL_16, PWM_PC_8, PWM_PC_9, PWM_PC_10, CTC_16, FAST_PWM_8,
	FAST_PWM_9, FAST_PWM_10, PWM_PFC_16=9, PWM_PC_16=11, FAST_PWM_16=15
}Timer_1_Mode;

typedef struct
{
	Timer_Select timer;
	Timer_Mode mode;
	Timer_Compare_Output_Mode com_mode;
	Timer_Clock clock;
	Timer_Interrupt interrupt;
	uint16 compare_duty;
	Timer_1_Mode t1_mode;
	Timer_2_Clock t2_clock;

}Timer_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer driver
 * 	1. Set the required clock.
 * 	2. Set the required Timer.
 * 	3. Enable the Timer Interrupt.
 * 	4. Initialize Timer Registers.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(Timer_Select timer, void(*a_ptr)(void));

/*
 * Description: Function to clear the Timer Value to start count from ZERO
 */
void Timer_clearTimerValue(Timer_Select timer);

/*
 * Description: Function to Edit duty/compare value
 */
void Timer_EditDuty(Timer_Select timer, uint16 compare_duty);
/*
 * Description: Function to disable the Timer
 */
void Timer_DeInit(Timer_Select timer);

#endif /* TIMER_H_ */
