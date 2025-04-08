#include "car.h"
#include "math.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>

#define P (1.0f / 12.0f)
#define I 0.00000001f
#define D 0.001f

#define DIFF_COEF 0.01f

extern int lightCenter;

int main() {
	uint32_t i;
	float a;
	char buf[32];
	
	int accum = 0;
	int lastCenter = 0;
	
	float steering = 0;
				
	initCar();
	initCamera();
	
	P3->OUT |= (BIT6);
	P3->OUT |= (BIT7);
	
	uart0_put("Car Starting\n");
	
	for(i = 0;; i++) {		
		if(lightCenter != 400) {
			lastCenter = lightCenter;
			accum += lightCenter;
		}
		
		updateCamera();
		
		steering = lightCenter * P + accum * I + (lightCenter - lastCenter) * D;
		
		if(steering > 1.0f) {
			steering = 1.0f;
		} else if(steering < -1.0f) {
			steering = -1.0f;
		}
		
		if(lightCenter == 400) {
			setSteerAngle(0.2);
			setWheel(0, 0);
			setWheel(1, 0);
		} else {
			setSteerAngle(steering);
			setWheel(0, -0.4f + -steering * DIFF_COEF);
			setWheel(1, -0.4f + steering * DIFF_COEF);
		}

		/*if(a > 0) {
			TIMER_A0_PWM_DutyCycle(0.1, 1);
			TIMER_A0_PWM_DutyCycle(0.1, 2);
			TIMER_A0_PWM_DutyCycle(0.1, 4);
			TIMER_A0_PWM_DutyCycle(0.3, 3);
		} else {
		  TIMER_A0_PWM_DutyCycle(0.0, 4);
		}*/
	}
}
