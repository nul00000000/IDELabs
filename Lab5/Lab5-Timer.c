/*
* Rochester Institute of Technology
* Department of Computer Engineering
* CMPE 460  Interfacing Digital Electronics
* LJBeato
* 1/14/2021
*
* Filename: main_timer_template.c
*/
#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "leds.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
extern uint32_t SystemCoreClock;

// these are not used by the timer
BOOLEAN g_sendData = FALSE;
uint16_t line[128];

int colorIndex = 0;
BYTE colors[7] = { RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE };

BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;

#define SW1 BIT1
#define SW2 BIT4
#define SWITCHES (SW1 | SW2)

unsigned long MillisecondCounter = 0;
//

//  I/O interrupt pin setup
//
// DIR     SEL0/SEL1    IE    IES     Port Mode
//  0          00        0     0       Input, rising edge trigger
//  0          00        0     1       Input, falling edge trigger, interrupt
//  0          00        1     0       Input, rising edge trigger, interrupt
//  0          00        1     1       Input, falling edge trigger, interrupt
//

void Switch1_Interrupt_Init(void)
{
	// disable interrupts
	DisableInterrupts();
	
	//WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
	//set as GPIO
	P1->SEL0 &= ~SW1;
	P1->SEL1 &= ~SW1;
	//set as input
	P1->DIR &= ~SW1;
	//enable falling edge triggered interrupts
	P1->IES |= SW1;
	P1->IE |= SW1;
	
	//clear flag
	P1->IFG &= SW2;
	
	// now set the pin to cause falling edge interrupt event
  NVIC_IPR8 = (NVIC_IPR8 & 0x00FFFFFF)|0x40000000; // priority 2
	
	// enable Port 1 - interrupt 35 in NVIC	
  NVIC_ISER1 = 0x00000008;  
	
	// enable interrupts  (// clear the I bit	)
  EnableInterrupts();              
	
}
void Switch2_Interrupt_Init(void)
{
	// disable interrupts
	DisableInterrupts();
	
	//WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
	//set as GPIO
	P1->SEL0 &= ~SW2;
	P1->SEL1 &= ~SW2;
	//set as input
	P1->DIR &= ~SW2;
	//enable falling edge triggered interrupts
	P1->IES |= SW2;
	P1->IE |= SW2;
	
	// now set the pin to cause falling edge interrupt event
	// P1.4 is falling edge event
  ;
  
	// clear flag4 (reduce possibility of extra interrupt)
  P1->IFG &= SW2; 
  
	// arm interrupt on P1.4 
  ;     
	
	// now set the pin to cause falling edge interrupt event
  NVIC_IPR8 = (NVIC_IPR8&0x00FFFFFF)|0x40000000; // priority 2
	
	// enable Port 1 - interrupt 35 in NVIC
  NVIC_ISER1 = 0x00000008;         
	
	// enable interrupts  (// clear the I bit	)
  EnableInterrupts();              
	
}
// PORT 1 IRQ Handler
// LJBeato
// Will be triggered if any pin on the port causes interrupt
//
// Derived From: Jonathan Valvano


void PORT1_IRQHandler(void)
{
	float numSeconds = 0.0;
	char temp[32];

	// First we check if it came from Switch1 ?
  if(P1->IFG & SW1) { // we start a timer to toggle the LED1 1 second ON and 1 second OFF
		// acknowledge P1.1 is pressed, by setting BIT1 to zero - remember P1.1 is switch 1
		if(!(P1->IN & SW1)) {
			Timer1RunningFlag = !Timer1RunningFlag;
			if(!Timer1RunningFlag) {
				LED1_Off();
			}
		}
    P1->IFG &= ~SW1;
  }
	// Now check to see if it came from Switch2 ?
  if(P1->IFG & SW2) {
		if(!(P1->IN & SW2)) {
			Timer2RunningFlag = TRUE;
		} else {
			Timer2RunningFlag = FALSE;
			//print millisecond counter to UART
			uart0_put("");
		}
		// acknowledge P1.4 is pressed, by setting BIT4 to zero - remember P1.4 is switch 2
		P1->IFG &= ~SW2;
  }
}

//
// Interrupt Service Routine for Timer32-1
//
void Timer32_1_ISR(void) {
	if(Timer1RunningFlag) {
		if (LED1_State() == FALSE ) {
			LED1_On();
		}
		else LED1_Off();
	}
}

//
// Interrupt Service Routine
//
void Timer32_2_ISR(void) {
	if(Timer2RunningFlag) {
		MillisecondCounter++;
	}
}


//
// main
//
int main(void){
	//initializations
	uart0_init();
	uart0_put("\r\nLab5 Timer demo\r\n");
	// Set the Timer32-2 to 2Hz (0.5 sec between interrupts)
	Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/2, T32DIV1); // initialize Timer A32-1;
        
	// Setup Timer32-2 with a .001 second timeout.
	// So use DEFAULT_CLOCK_SPEED/(1/0.001) = SystemCoreClock/1000
	Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock/1000, T32DIV1); // initialize Timer A32-1;
    
	Switch1_Interrupt_Init();
	Switch2_Interrupt_Init();
	LED1_Init();
	LED2_Init();
	EnableInterrupts();
  while(1)
	{
    WaitForInterrupt();
  }
}
