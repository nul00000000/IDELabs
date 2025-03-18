// TimerA.c

/*  LJBeato
    2021
    TimerA functionality to drive DC motor
    and Servo Motor
 */


#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include "TimerA.h"
#include "uart.h"

// Make these arrays 5 deep, since we are using indexes 1-4 for the pins
static uint32_t DEFAULT_PERIOD_A0[5] = {0,0,0,0,0};
static uint32_t DEFAULT_PERIOD_A2[5] = {0,0,0,0,0};
//***************************PWM_Init*******************************
// PWM output on P2.4, P2.5, P2.6, P2.7
// Inputs:  freq of P2.4...P2.7 is frequency of the cycles
//          percentDutyCycle (0 -> 1.0)
//          pin number (1,2,3,4)
// Outputs: none


int TIMER_A0_PWM_Init(uint16_t freq, double percentDutyCycle, uint16_t pin)
{
	//stop PWM
	uint16_t dutyCycle;
	TIMER_A0->CTL &= ~(BIT4 | BIT5);
	// Timer A0.1
	if (pin >= 1 || pin <= 4)
	{
		P2->SEL0 |= (BIT3 << pin);
		P2->SEL1 &= ~(BIT3 << pin);
		P2->DIR |= (BIT3 << pin);
		P2->DS |= (BIT3 << pin);
	}
	else return -2;
	// save the period for this timer instance
	// DEFAULT_PERIOD_A0[pin] where pin is the pin number
	DEFAULT_PERIOD_A0[pin] = SystemCoreClock/freq;
	TIMER_A0->CCR[0] = SystemCoreClock/freq;
	//TIMER_A0->CCR[0] = (uint16_t) (period * percentDutyCycle);
	
	// TIMER_A0->CCTL[pin]
	//start PWM
  TIMER_A0->CCTL[pin] |= (BIT7 | BIT6 | BIT5);
	
	// set the duty cycle
	dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
    //TIMER_A0->CCR[pin]
  TIMER_A0->CCR[pin] = dutyCycle;
	
	// Timer CONTROL register
	// TIMER_A0->CTL
	//set clock source
	TIMER_A0->CTL &= ~(BIT8 | BIT9); 
	TIMER_A0->CTL |= BIT9;
	//set clock divider
	TIMER_A0->CTL &= ~(BIT6 | BIT7);
	//clear timer
	TIMER_A0->CTL |= BIT2;
	//start up counting
	TIMER_A0->CTL |= BIT4;
	return 0;
}
//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on pin
// Inputs:  dutycycle, pin
// Outputs: none
// percentDutyCycle is a number between 0 and 1  (ie. 0.5 = 50%)
void TIMER_A0_PWM_DutyCycle(double percentDutyCycle, uint16_t pin)
{
	uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);
	TIMER_A0->CTL &= ~(BIT4 | BIT5);
	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
    //TIMER_A0->CCR[pin]
  TIMER_A0->CCR[pin] = dutyCycle;
	//start up counting
	TIMER_A0->CTL |= BIT4;
}

//***************************PWM_Init*******************************
// PWM output on P5.6
// Inputs:  period of P5.6 is number of counts before output changes state
//          percentDutyCycle (0 -> 1.0)//          duty cycle
//          pin number (1,2,3,4), but always 1
// Outputs: none
int TIMER_A2_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin)
{

	//stop PWM
	uint16_t dutyCycle;
	TIMER_A0->CTL &= ~(BIT4 | BIT5);
	// Timer A0.1
	if (pin == 1)
	{
		P2->SEL0 |= BIT4;
		P2->SEL1 &= ~BIT4;
		P2->DIR |= BIT4;
		P2->DS |= BIT4;
	}
	else return -2;



	
	// save the period for this timer instance
	// DEFAULT_PERIOD_A0[pin] where pin is the pin number
	DEFAULT_PERIOD_A0[pin] = period;
	// TIMER_A0->CCR[0]
	//TIMER_A0->CCR[0] = (uint16_t) (period * percentDutyCycle);
	
	

	// TIMER_A0->CCTL[pin]
	//start PWM
  TIMER_A0->CCTL[pin] |= BIT6;
	
	// set the duty cycle
	dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
    //TIMER_A0->CCR[pin]
  TIMER_A0->CCR[pin] = dutyCycle;
	
	// Timer CONTROL register
	// TIMER_A0->CTL
	//set clock source
	TIMER_A0->CTL &= ~(BIT8 | BIT9); 
	//set clock divider
	TIMER_A0->CTL |= (BIT6 | BIT7);
	//clear timer
	TIMER_A0->CTL |= BIT2;
	
	//start up counting
	TIMER_A0->CTL |= BIT4;
	return 0;
}
//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on P5.6
// Inputs:  percentDutyCycle, pin  (should always be 1 for TimerA2.1)
//         
// Outputs: none
// 
void TIMER_A2_PWM_DutyCycle(double percentDutyCycle, uint16_t pin)
{

}

