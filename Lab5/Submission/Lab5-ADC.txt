/*
* Rochester Institute of Technology
* Department of Computer Engineering
* CMPE 460  Interfacing Digital Electronics
* LJBeato
* 1/14/2021
*
* Filename: main_timer_template.c
*/
#include "ADC14.h"

#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "leds.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
// The sprintf function seemed to cause a hange in the interrupt service routine.
// I think if we increase the HEAP size, it will work
// change to Heap_Size       EQU     0x00000200 in startup_msp432p401r_uvision.s


BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;
char hex[12];
char temp[64];
unsigned int analogIn = 0;
#define SW2 BIT4

unsigned long MillisecondCounter = 0;

// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
		analogIn = ADC_In();
    //sprintf(hex, "%x", analogIn);
	  float celcius = ((int) analogIn) / 142.46f - 10;		
	  float farenheit = celcius * (9.0f/5.0f) + 32.0f;
		sprintf(temp, "Temp Reading: Celcius %3.1f, Farenheit %3.1f\r\n", celcius, farenheit);
	  //sprintf(temp, "ADC Reading: Decimal %d, Hex %s\r\n", analogIn, hex);
		uart0_put(temp);
}

// main
int main(void)
{
	//initializations
	uart0_init();
	uart0_put("\r\nLab5 ADC demo\r\n");
	// Set the Timer32-1 to 2Hz (0.5 sec between interrupts)
	Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/2, T32DIV1); // initialize Timer A32-1;

	//LED1_Init();
	//LED2_Init();
	//Switch2_Interrupt_Init();
	ADC0_InitSWTriggerCh6();
	EnableInterrupts();
  while(1)
	{
		WaitForInterrupt();
  }
}

