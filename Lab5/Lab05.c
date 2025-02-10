/*
Port	Pin	Name
Switch 1	1	4	P1.4
Switch 2	1	1	P1.1

Switch 1 Interrupt: 
	Interrupt is P1IFG.4
	Set falling edge with: 
Switch 2 Interrupt: 
	Interrupt is P1IFG.1
	*/
	
#include "msp.h"
#include "Common.h"

#define SW1 BIT1
#define SW2 BIT4
#define SWITCHES (SW1 | SW2)

void setup() {
	//set as GPIO
	P1->SEL0 &= ~SWITCHES;
	P1->SEL1 &= ~SWITCHES;
	//set as input
	P1->DIR &= ~SWITCHES;
	//enable falling edge triggered interrupts
	P1->IES |= SWITCHES;
	P1->IE |= SWITCHES;
}

int main(void) {
	
}