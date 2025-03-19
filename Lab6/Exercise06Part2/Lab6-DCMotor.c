/*
 * Main module for testing the PWM Code for the K64F
 * 
 * Author:  
 * Created:  
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu> 
 * LJBeato
 * 2021
 */

#include "msp.h"
#include "uart.h"
#include "TimerA.h"

#define put(x) uart0_put(x)

/**
 * Waits for a delay (in milliseconds)
 * 
 * del - The delay in milliseconds
 */
void delay(int del){
	volatile int i;
	for (i=0; i<del*270; i++){
		;// Do nothing
	}
}

int main(void) {
	int forward = 1;
	int phase = 0;
	int pos = 0;
	
	// Part 1 - UNCOMMENT THIS
	//Generate 20% duty cycle at 10kHz
	// Configure the Signal Multiplexer for GPIO Pins
	// Configure the GPIO Pins for Output
	P4->SEL0 &= ~(BIT1 | BIT2 | BIT3 | BIT4);
	P4->SEL1 &= ~(BIT1 | BIT2 | BIT3 | BIT4);
	P4->DIR |= (BIT1 | BIT2 | BIT3 | BIT4);
	P4->DS |= (BIT1 | BIT2 | BIT3 | BIT4);
	while ( 1 ){
		// Turn off all coils , Set GPIO pins to 0
		P4->OUT &= ~(BIT1 | BIT2 | BIT3 | BIT4);
		// Set one pin high at a time
		if ( forward ) {
			if ( phase == 0) { P4->OUT |= BIT1; P4->OUT &= ~BIT4; phase ++;} // A ,1 a
			else if ( phase == 1) { P4->OUT |= BIT2; P4->OUT &= ~BIT1; phase ++;} // B ,2 a
			else if ( phase == 2) { P4->OUT |= BIT3; P4->OUT &= ~BIT2; phase ++;} // C ,1 b
			else { P4->OUT |= BIT4; P4->OUT &= ~BIT3; phase =0;} // D ,2 b
			pos++;
			if(pos == 20) {
				forward = 0;
			}
		}
		else { // reverse
			if ( phase == 0) { P4->OUT |= BIT4; P4->OUT &= ~BIT1; phase ++;} // D ,2 b
			else if ( phase == 1) { P4->OUT |= BIT3; P4->OUT &= ~BIT4; phase ++;} // C ,1 b
			else if ( phase == 2) { P4->OUT |= BIT2; P4->OUT &= ~BIT3; phase ++;} // B ,2 a
			else { P4->OUT |= BIT1; P4->OUT &= ~BIT2; phase =0;} // A ,1 a
			pos--;
			if(pos == 0) {
				forward = 1;
			}
		}
		// Note - you need to write your own delay function
		delay (pos * 5 + 50); // smaller values = faster speed
	}
	return 0;
}



