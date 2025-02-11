/*
Port	Pin	Name
Switch 1	1	4	P1.4
Switch 2	1	1	P1.1

Switch 1 Interrupt: 
	Interrupt is P1IFG.4
	Set falling edge with: 
Switch 2 Interrupt: 
	Interrupt is P1IFG.1

00h T32LOAD1 - load register
04h T32VALUE1 - current value register
08h T32CONTROL1 - control register
0Ch T32INTCLR1 - clear register

20h T32LOAD2 - load register
24h T32VALUE2 - current value register
28h T32CONTROL2 - control register
2Ch T32INTCLR2 - clear register

Control:
0000 0000 0000 0000 0000 0000 0000 0000
XXXX XXXX XXXX XXXX XXXX XXXX 101X 0110
Timer32_1->Control = 0x000000A6
Timer32_2->Control = 0x000000A6

https://faculty-web.msoe.edu/johnsontimoj/EE2920/files2920/timer_32.pdf
	*/
	
#include "msp.h"
#include "Common.h"

#define SW1 BIT1
#define SW2 BIT4
#define SWITCHES (SW1 | SW2)

void PORT1_IRQHandler(void) {
	if(P1->IFG & SW1) {
		//do something or another
	} else if(P1->IFG & SW2) {
		//do something else or another else
	}
}

void setup() {
	// Stop Watchdog Timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
	//set as GPIO
	P1->SEL0 &= ~SWITCHES;
	P1->SEL1 &= ~SWITCHES;
	//set as input
	P1->DIR &= ~SWITCHES;
	//enable falling edge triggered interrupts
	P1->IES |= SWITCHES;
	P1->IE |= SWITCHES;
	
	NVIC_EnableIRQ(PORT1_IRQn);
	
	__enable_irq();
//------------------------------------------------
	//Enable timers
	Timer32_1->Control = 0x000000A6;
	Timer32_2->Control = 0x000000A6;
}

int main(void) {
	
}