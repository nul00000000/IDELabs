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

#define ADC_MAX 16383
#define LOW_TRSH 1300
#define HIGH_TRSH (LOW_TRSH * 2.5)
// The sprintf function seemed to cause a hange in the interrupt service routine.
// I think if we increase the HEAP size, it will work
// change to Heap_Size       EQU     0x00000200 in startup_msp432p401r_uvision.s


BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;
char hex[12];
char temp[64];
char temp2[64];
int bpmcounter = 0;
unsigned long lastBeat;
char lastHigh = 0;
char high = 0;
#define SW2 BIT4

int bpm;
int bpmtotal;
unsigned long startTime = 0;
float peak2peak = 0;
unsigned long MillisecondCounter = 0;

// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
	unsigned int analog = ADC_In();
		//sprintf(temp, "%d\n\r", analog);
			//uart0_put(temp);
	lastHigh = high;
	if(analog < LOW_TRSH) {
		high = 0;
	} else if(analog > HIGH_TRSH) {
		high = 1;
	}
	
	if(high && !lastHigh) {
		peak2peak = MillisecondCounter - lastBeat;
		bpm = 60000/peak2peak;
		bpmtotal += bpm;
		bpmcounter ++;
		if (bpmcounter == 3) {
			bpmtotal /= 3;
			sprintf(temp, "Current BPM: %d\r\n", bpmtotal);
			uart0_put(temp);
			bpmcounter = 0;
			bpmtotal = 0;
		}
		lastBeat = MillisecondCounter;
	}
}

void Timer32_2_ISR(void)
{
	MillisecondCounter++;
}

// main
int main(void)
{
	//initializations
	uart0_init();
	uart0_put("\r\nLab8 ADC demo\r\n");
	// Set the Timer32-1 to 2Hz (0.5 sec between interrupts)
	Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/128, T32DIV1); // initialize Timer A32-1;
	Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock/1000, T32DIV1); // initialize Timer A32-2;

	ADC0_InitSWTriggerCh6();
	EnableInterrupts();
  while(1)
	{
		WaitForInterrupt();
  }
}

