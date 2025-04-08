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
#include <math.h>

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
	int i;
	// Initialize UART and PWM
	uart0_init();
	//TIMER_A0_PWM_Init(50, 0.0, 3);
	TIMER_A0_PWM_Init(50, 0.0, 4);

	// Print welcome over serial
	put("Running... \r\n");
	
	for(;;) {
		double a = sin(i * 0.001);
		i++;
		TIMER_A0_PWM_DutyCycle(a * 0.03 + 0.06, 4);
	}
	
	
	return 0;
}



