#include "car.h"
#include "math.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>

//#define P (1.0f / 5.0f)
#define P (0.10f)
#define I (0.0f)
#define D (0.0f)

#define DIFF_COEF 0.17f

extern float lightCenter;
extern int num;

//current measured lap speed on milestone 3: 34.57s over 4 laps = 8.89s (wowzers)

int main() {
	uint32_t i;
	char buf[32];
	
	float lastCenter = 0;
	float lastLastCenter = 0;
	
	float dval = 0;
	float dDrop = 0;
	
	float steering = 0;
	float speed = 0;
	
	char carpet = 0;
	
	initCar();
	initCamera();
	
	P3->OUT |= (BIT6);
	P3->OUT |= (BIT7);
	
	uart0_put("Car Started\n");
	
	for(i = 0;; i++) {
		if(!carpet) {
			lastLastCenter = lastCenter;
			lastCenter = lightCenter;
		}
		
		carpet = updateCamera();
		
		//steering = (lightCenter * P) + ((lightCenter + lastCenter) * I * 0.001) + ((lightCenter - 2 * lastCenter + lastLastCenter) * D) + 0.2;
		dval = (lightCenter - 2 * lastCenter + lastLastCenter);
		//dDrop = fabs(dval) > fabs(dDrop) ? dval : (dval * 0.5f + dDrop * 0.5f);
		steering = (lightCenter * P) + (dval * D) + 0.15f;
		speed = num / 128.0f * 0.45f + 0.55f;
		
		//sprintf(buf, "wow %d\n", (int) lightCenter);
		//uart0_put(buf);
		
		if(steering > 1.0f) {
			steering = 1.0f;
		} else if(steering < -1.0f) {
			steering = -1.0f;
		}
		
		if(carpet) {
			setSteerAngle(0.15);
			setWheel(0, 0);
			setWheel(1, 0);
		} else {
			setSteerAngle(steering);
			setWheel(0, (-0.5f + (steering > 0 ? steering * DIFF_COEF : 0)) * speed); //the polarity is now correct
			setWheel(1, (-0.5f + (steering < 0 ? -steering * DIFF_COEF : 0)) * speed);
			//setWheel(0, 0);
			//setWheel(1, 0);
			//we all live in a yellow submarine
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
