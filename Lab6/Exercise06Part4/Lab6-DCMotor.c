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

#define puts(x) uart0_put(x)

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
	int j;
	double a;
	// Initialize UART and PWM
	uart0_init();
	TIMER_A0_PWM_Init(10000, 0.0, 3);
	TIMER_A0_PWM_Init(10000, 0.0, 4);
	TIMER_A0_PWM_Init(10000, 0.0, 1);
	TIMER_A0_PWM_Init(10000, 0.0, 2);
	TIMER_A2_PWM_Init(50, 0.0, 1);

	// Print welcome over serial
	put("Running... \r\n");
	
	// Part 1 - UNCOMMENT THIS
	//Generate 20% duty cycle at 10kHz
	
	for(;;) {
		for(i = 0; i < 100; i++) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 4);
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 2);
			delay(10);
			a = sin(j * 0.01);
			j++;
			TIMER_A2_PWM_DutyCycle(a * 0.02 + 0.075, 1);
		}
		for(i = 100; i > 0; i--) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 4);
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 2);
			delay(10);
			a = sin(j * 0.01);
			j++;
			TIMER_A2_PWM_DutyCycle(a * 0.02 + 0.075, 1);
		}
		TIMER_A0_PWM_DutyCycle(0, 4);
		TIMER_A0_PWM_DutyCycle(0, 2);
		for(i = 0; i < 100; i++) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 3);
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 1);
			delay(10);
			a = sin(j * 0.01);
			j++;
			TIMER_A2_PWM_DutyCycle(a * 0.02 + 0.075, 1);
		}
		for(i = 100; i > 0; i--) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 3);
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 1);
			delay(10);
			a = sin(j * 0.01);
			j++;
			TIMER_A2_PWM_DutyCycle(a * 0.02 + 0.075, 1);
		}
		TIMER_A0_PWM_DutyCycle(0, 3);
		TIMER_A0_PWM_DutyCycle(0, 1);
	}
	
	return 0;
}



