#include "car.h"
#include "math.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>

//#define P (0.10f)
#define P (0.11f)
#define I (1.25f)
#define D (7.69f)

//original values: P=0.11, I=0.0, D=10.0
//try cranking D way up
#define DIFF_COEF 0.4f

#define NUM_MEM 20

extern float lightCenter;
extern int num;

//current measured lap speed on milestone 3: 34.57s over 4 laps = 8.89s (wowzers)

int main() {
	uint32_t i;
	char buf[32];
	
	int currIndex = 0;
	float lastCenters[NUM_MEM];
	
	float dval = 0;
	float dDrop = 0;
	
	float steering = 0;
	float speed = 0;
	
	char carpet = 0;
	
	for(i = 0; i < NUM_MEM; i++) {
		lastCenters[i] = 0.0f;
	}
	
	initCar();
	initCamera();
	
	P3->OUT |= (BIT6);
	P3->OUT |= (BIT7);
	
	uart0_put("Car Started\n");
	
	for(i = 0;; i++) {
		if(!carpet) {			
			lastCenters[currIndex] = lightCenter;
			currIndex++;
			if(currIndex > NUM_MEM) {
				currIndex = 0;
			}
		}
		
		carpet = updateCamera();
		
		//steering = (lightCenter * P) + ((lightCenter + lastCenter) * I * 0.001) + ((lightCenter - 2 * lastCenter + lastLastCenter) * D) + 0.2;
		dval = (lightCenter - lastCenters[currIndex]);
		//dDrop = fabs(dval) > fabs(dDrop) ? dval : (dval * 0.5f + dDrop * 0.5f);
		steering = (lightCenter * P) + (dval * D) + 0.17f;
		speed = num / 128.0f * 0.38f + 0.63f;
		
		//sprintf(buf, "wow %d\n", (int) lightCenter);
		//uart0_put(buf);
		
		if(steering > 1.0f) {
			steering = 1.0f;
		} else if(steering < -1.0f) {
			steering = -1.0f;
		}
		
		if(carpet) {
			setSteerAngle(0.17);
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
	}
}
