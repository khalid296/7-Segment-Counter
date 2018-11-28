/************************************************
 *	 File Name: timer.c							*
 * Description: 								*
 *  Created on: Sep 29, 2018					*
 *      Author: Khalid Tarek					*
 ************************************************/

#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variable array of pointer to function to hold
 * the addresses of the call back functions in the application */
static volatile void (*g_callBackPtr[4])(void) = {NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR};

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_OVF_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[0] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[0])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[0] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[0])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[1] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[1])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
	/*check if function is passed to the pointer*/
	else if(g_callBackPtr[2] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[2])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[1] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[1])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPB_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[2] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[2])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[3] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[3])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	/*check if function is passed to the pointer*/
	if(g_callBackPtr[3] != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr[3])();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer driver
 * 	1. Set the required clock.
 * 	2. Set the required Timer.
 * 	3. Enable the Timer Interrupt.
 * 	4. Initialize Timer Registers.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	switch(Config_Ptr->timer){
		case TIMER0:
			TCNT0 = 0; //timer initial value
			OCR0  = Config_Ptr->compare_duty; //compare value/pwm duty value
			/* Check if mode is PWM or fast PWM */
			if (Config_Ptr->mode==PWM || Config_Ptr->mode==FAST_PWM){
				/* Set OC0 pin (PB3) as output pin */
				DDRB  = DDRB | (1<<PB3);
				/* Clear Force Output Compare bit */
				CLEAR_BIT(TCCR0,FOC0);
			}else{
				/* Set Force Output Compare bit */
				SET_BIT(TCCR0,FOC0);
			}
			/* If mode is normal and interrupt on
			 * enable overflow interrupt
			 * else enable compare interrupt when CTC is selected and interrupt is on*/
			if(Config_Ptr->mode==NORMAL && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,TOIE0); //enable Overflow interrupt
			}else if(Config_Ptr->mode==CTC && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,OCIE0); //enable compare interrupt
			}
			/* Configure  TCCR register according to configuration structure values */
			TCCR0 = (TCCR0 & 0x80) \
					| (Config_Ptr->clock & 0x07) \
					| ((Config_Ptr->com_mode & 0x03)<<4)\
					| ((Config_Ptr->mode & 0x01)<<6)\
					| ((Config_Ptr->mode & 0x02)<<2);
			break;
		case TIMER1A:
			TCNT1 = 0; //timer initial value
			OCR1A  = Config_Ptr->compare_duty; //compare value
			/* Check if mode is PWM or fast PWM */
			if (Config_Ptr->t1_mode==PWM_PC_8 || Config_Ptr->t1_mode==PWM_PC_9 || Config_Ptr->t1_mode==PWM_PC_10 ||\
				Config_Ptr->t1_mode==FAST_PWM_8 || Config_Ptr->t1_mode==FAST_PWM_9 || Config_Ptr->t1_mode==FAST_PWM_10 ||\
				Config_Ptr->t1_mode==PWM_PFC_16 || Config_Ptr->t1_mode==PWM_PC_16 || Config_Ptr->t1_mode==FAST_PWM_16){
				/* Set OC1A pin (PD5) as output pin */
				SET_BIT(DDRD,PD5);
				/* Clear Force Output Compare bit */
				CLEAR_BIT(TCCR1A,FOC1A);
			}else{
				/* Set Force Output Compare bit */
				SET_BIT(TCCR1A,FOC1A);
			}
			/* If mode is normal and interrupt on
			 * enable overflow interrupt
			 * else enable compare interrupt when CTC is selected and interrupt is on*/
			if(Config_Ptr->t1_mode==NORMAL_16 && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,TOIE1); //enable Overflow interrupt
			}else if(Config_Ptr->t1_mode==CTC_16 && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,OCIE1A); //enable compare interrupt
			}
			/* Configure  TCCR registers according to configuration structure values */
			TCCR1A = (TCCR1A & 0x3C) \
					| ((Config_Ptr->com_mode & 0x03)<<6)\
					| (Config_Ptr->t1_mode & 0x03);
			TCCR1B = (TCCR1B & 0xE0) \
					| (Config_Ptr->clock & 0x07) \
					| ((Config_Ptr->t1_mode & 0x0C)<<1);
			break;
		case TIMER1B:
			TCNT1 = 0; //timer initial value
			OCR1B  = Config_Ptr->compare_duty; //compare value
			/* Check if mode is PWM or fast PWM */
			if (Config_Ptr->t1_mode==PWM_PC_8 || Config_Ptr->t1_mode==PWM_PC_9 || Config_Ptr->t1_mode==PWM_PC_10 ||\
				Config_Ptr->t1_mode==FAST_PWM_8 || Config_Ptr->t1_mode==FAST_PWM_9 || Config_Ptr->t1_mode==FAST_PWM_10 ||\
				Config_Ptr->t1_mode==PWM_PFC_16 || Config_Ptr->t1_mode==PWM_PC_16 || Config_Ptr->t1_mode==FAST_PWM_16){
				/* Set OC1A pin (PD4) as output pin */
				SET_BIT(DDRD,PD4);
				/* Clear Force Output Compare bit */
				CLEAR_BIT(TCCR1A,FOC1B);
			}else{
				/* Set Force Output Compare bit */
				SET_BIT(TCCR1A,FOC1B);
			}
			/* If mode is normal and interrupt on
			 * enable overflow interrupt
			 * else enable compare interrupt when CTC is selected and interrupt is on*/
			if(Config_Ptr->t1_mode==NORMAL_16 && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,TOIE1); //enable Overflow interrupt
			}else if(Config_Ptr->t1_mode==CTC_16 && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,OCIE1B); //enable compare interrupt
			}
			/* Configure  TCCR registers according to configuration structure values */
			TCCR1A = (TCCR1A & 0xCC) \
					| ((Config_Ptr->com_mode & 0x03)<<4)\
					| (Config_Ptr->t1_mode & 0x03);
			TCCR1B = (TCCR1B & 0xE0) \
				| (Config_Ptr->clock & 0x07) \
				| ((Config_Ptr->t1_mode & 0x0C)<<1);
			break;
		case TIMER2:
			TCNT2 = 0; //timer initial value
			OCR2  = Config_Ptr->compare_duty; //compare value/pwm duty value
			/* Check if mode is PWM or fast PWM */
			if (Config_Ptr->mode==PWM || Config_Ptr->mode==FAST_PWM){
				/* Set OC1B pin (PD4) as output pin */
				SET_BIT(DDRD,PD7);
				/* Clear Force Output Compare bit */
				CLEAR_BIT(TCCR2,FOC2);
			}else{
				/* Set Force Output Compare bit */
				SET_BIT(TCCR2,FOC2);
			}
			/* If mode is normal and interrupt on
			 * enable overflow interrupt
			 * else enable compare interrupt when CTC is selected and interrupt is on*/
			if(Config_Ptr->mode==NORMAL && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,TOIE2); //enable Overflow interrupt
			}else if(Config_Ptr->mode==CTC && Config_Ptr->interrupt==ON){
				SET_BIT(TIMSK,OCIE2); //enable compare interrupt
			}
			/* Configure  TCCR register according to configuration structure values */
			TCCR2 = (TCCR2 & 0x80) \
					| (Config_Ptr->t2_clock & 0x07) \
					| ((Config_Ptr->com_mode & 0x03)<<4)\
					| ((Config_Ptr->mode & 0x01)<<6)\
					| ((Config_Ptr->mode & 0x02)<<2);
			break;
	}
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(Timer_Select timer, void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr[timer] = a_ptr;
}

/*
 * Description: Function to clear the Timer Value to start count from ZERO
 */
void Timer_clearTimerValue(Timer_Select timer)
{
	switch(timer){
		case TIMER0:
			TCNT0 = 0; //timer initial value
			break;
		case TIMER1A:
		case TIMER1B:
			TCNT1 = 0; //timer initial value
			break;
		case TIMER2:
			TCNT2 = 0; //timer initial value
			break;
	}

}

void Timer_EditDuty(Timer_Select timer, uint16 compare_duty)
{
	switch(timer){
		case TIMER0:
			OCR0 = compare_duty; //timer new compare/duty value
			break;
		case TIMER1A:
			OCR1A = compare_duty; //timer new compare/duty value
			break;
		case TIMER1B:
			OCR1B = compare_duty; //timer new compare/duty value
			break;
		case TIMER2:
			OCR2 = compare_duty; //timer new compare/duty value
			break;
	}
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void TIMER_DeInit(Timer_Select timer)
{
	switch(timer){
		case TIMER0:
			/* Clear All Timer Registers */
			TCCR0 = 0;
			TCNT0 = 0;

			/* Disable the interrupts */
			CLEAR_BIT(TIMSK,TOIE0);
			CLEAR_BIT(TIMSK,OCIE0);
			break;
		case TIMER1A:
		case TIMER1B:
			/* Clear All Timer Registers */
			TCCR1A = 0;
			TCCR1B = 0;
			TCNT1 = 0;

			/* Disable the interrupts */
			CLEAR_BIT(TIMSK,TOIE1);
			CLEAR_BIT(TIMSK,OCIE1A);
			CLEAR_BIT(TIMSK,OCIE1B);
			break;
		case TIMER2:
			/* Clear All Timer Registers */
			TCCR2 = 0;
			TCCR2 = 0;
			TCNT2 = 0;

			/* Disable the interrupts */
			CLEAR_BIT(TIMSK,TOIE2);
			CLEAR_BIT(TIMSK,OCIE2);
			break;
	}
}

