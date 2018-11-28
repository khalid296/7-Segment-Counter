/************************************************
 *	 File Name: 7-Seg_Counter.c					*
 * Description: 								*
 *  Created on: Sep 29, 2018					*
 *      Author: Khalid Tarek					*
 ************************************************/

#include"micro_config.h"
#include"std_types.h"
#include"common_macros.h"
#include"timer.h"
#include"External_Interrupts.h"

#define NUMBER_OF_COMPARE_MATCHES_PER_SECOND 4

#define CONTROL_PORT_DIR DDRC
#define CONTROL_PORT_PORT PORTC

#define DATA_PORT_DIR DDRA
#define DATA_PORT_PORT PORTA

// variable to count the number of timer ticks (compare match)
uint8 g_tick = 0;

// flag will be set when the timer count 1 second
uint8 count_second_flag = 0;

// variables to hold the clock time
uint8 seconds_count = 0;
uint8 minutes_count = 0;
uint8 hours_count = 0;
uint8 days_count = 0;

/* External Interrupt Function */
void INT_function(void)
{
	/* Reset Counter */
	seconds_count = 0;
	minutes_count = 0;
	hours_count = 0;
}

/* Timer Function */
void TIMER_function(void)
{
	g_tick++;
	if(g_tick == NUMBER_OF_COMPARE_MATCHES_PER_SECOND)
	{
		count_second_flag = 1;
		g_tick = 0; //clear the tick counter again to count a new second
	}
}

void main(){

	SET_BIT(SREG,7);	//Enable Global interrupt I-bit

	// Enable and configure external INT1
	Interrupt_ConfigType  Interrupt_Config = {Interrupt1, FALLING_EDGE};
	Interrupt_setCallBack(Interrupt1,INT_function);
	Interrupt_init(&Interrupt_Config);

	// Enable and configure Timer0
	Timer_ConfigType  Timer_Config = {TIMER0, CTC, DISCONNECTED, F_CPU_1024, ON,244};
	Timer_setCallBack(TIMER0,TIMER_function);
	Timer_init(&Timer_Config);

	// enable internal pull up resistor at INT0/PD2 pin
	SET_BIT(PORTD,PD2);

	// configure first four pins in CONTROL_PORT_PORT as output pins
	DATA_PORT_DIR |= 0x0F;

	// configure All pins of DATA_PORT_PORT as output pins
	CONTROL_PORT_DIR = 0xFF;

	// initialize all the 7-segment with zero value
	DATA_PORT_PORT &= 0xF0;
	CONTROL_PORT_PORT |= 0xFF;
	while(TRUE)
	{
		if(count_second_flag == 1)
		{
			//enter here every one second
			//increment seconds count
			seconds_count++;

			if(seconds_count == 60)
			{
				seconds_count = 0;
				minutes_count++;
			}
			if(minutes_count == 60)
			{
				minutes_count = 0;
				hours_count++;
			}
			if(hours_count == 24)
			{
				hours_count = 0;
			}
			if(days_count == 30)
			{
				days_count = 0;
			}
			// reset the flag again
			count_second_flag = 0;
		}
		else
		{
			// out the number of seconds
			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x01;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (seconds_count%10);

			// make small delay to see the changes in the 7-segment
			// 2Miliseconds delay will not effect the seconds count
			_delay_ms(2);

			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x02;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (seconds_count/10);

			_delay_ms(2);

			// out the number of minutes
			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x04;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (minutes_count%10);

			_delay_ms(2);

			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x08;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (minutes_count/10);

			_delay_ms(2);

			// out the number of hours
			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x10;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (hours_count%10);

			_delay_ms(2);

			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x20;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (hours_count/10);

			_delay_ms(2);

			// out the number of days
			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x40;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (days_count%10);

			_delay_ms(2);

			CONTROL_PORT_PORT = (CONTROL_PORT_PORT & 0x00) | 0x80;
			DATA_PORT_PORT = (DATA_PORT_PORT & 0xF0) | (days_count/10);

			_delay_ms(2);
		}
	}

}
