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
	int i;
	// Initialize UART and PWM
	uart0_init();
	TIMER_A0_PWM_Init(10000, 0.0, 3);
	TIMER_A0_PWM_Init(10000, 0.0, 4);

	// Print welcome over serial
	put("Running... \r\n");
	
	TIMER_A0_PWM_DutyCycle(0.2, 4);
	
	// Part 1 - UNCOMMENT THIS
	//Generate 20% duty cycle at 10kHz
	
	for(;;) {
		for(i = 0; i < 100; i++) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 4);
			delay(10);
		}
		for(i = 100; i > 0; i--) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 4);
			delay(10);
		}
		TIMER_A0_PWM_DutyCycle(0, 4);
		for(i = 0; i < 100; i++) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 3);
			delay(10);
		}
		for(i = 100; i > 0; i--) {
			TIMER_A0_PWM_DutyCycle((double) i / 100.0, 3);
			delay(10);
		}
		TIMER_A0_PWM_DutyCycle(0, 3);
	}
	
	
  //Part 2 - UNCOMMENT THIS
	/*
	for(;;)  //loop forever
	{
		uint16_t dc = 0;
		uint16_t freq = 10000; // Frequency = 10 kHz 
		uint16_t dir = 0;
		char c = 48;
		int i=0;
		
		// 0 to 100% duty cycle in forward direction
		for (i=0; i<100; i++) {
			TIMER_A0_PWM_DutyCycle(i/100, 1);
			TIMER_A0_PWM_DutyCycle(0, 2);
			TIMER_A0_PWM_DutyCycle(i/100, 3);
			TIMER_A0_PWM_DutyCycle(0, 4);
			delay(10);
		}
		
		// 100% down to 0% duty cycle in the forward direction
		for (i=100; i>=0; i--) {
			TIMER_A0_PWM_DutyCycle(i/100, 1);
			TIMER_A0_PWM_DutyCycle(0, 2);
			TIMER_A0_PWM_DutyCycle(i/100, 3);
			TIMER_A0_PWM_DutyCycle(0, 4);
			delay(10);
		}
		
		// 0 to 100% duty cycle in reverse direction
		for (i=0; i<100; i++) {
		  TIMER_A0_PWM_DutyCycle(0, 1);
			TIMER_A0_PWM_DutyCycle(i/100, 2);
			TIMER_A0_PWM_DutyCycle(0, 3);
			TIMER_A0_PWM_DutyCycle(i/100, 4);
			delay(10);
		}
		
		// 100% down to 0% duty cycle in the reverse direction
		for (i=100; i>=0; i--) {
		  TIMER_A0_PWM_DutyCycle(0, 1);
			TIMER_A0_PWM_DutyCycle(i/100, 2);
			TIMER_A0_PWM_DutyCycle(0, 3);
			TIMER_A0_PWM_DutyCycle(i/100, 4);
			delay(10);
		}

	}*/
	return 0;
}



