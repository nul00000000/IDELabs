#include "car.h"
#include "math.h"

#define STEER_COEF (1.0/1.0)

uint16_t lineData[128];
uint16_t procData[128];
char g_sendData;

float lightCenter = 0.0f;
int num = 0;

int numValley = 0;

void initCar(void) {
	//initialize UART
	uart0_init();
	
	put("UART0 Initialized\n\n");
	
	//initialize PWM for motors
	TIMER_A0_PWM_Init(10000, 0.0, 3);
	TIMER_A0_PWM_Init(10000, 0.0, 4);
	TIMER_A0_PWM_Init(10000, 0.0, 1);
	TIMER_A0_PWM_Init(10000, 0.0, 2);
	
	put("DC Motors Initialized\n");
	
	//initialize PWM for servo
	TIMER_A2_PWM_Init(50, 0.0, 1);
	
	put("Steering Servo Initialized\n\n");
	
	//initialize enable pins
	P3->SEL0 &= ~(BIT6 | BIT7);
	P3->SEL1 &= ~(BIT6 | BIT7);
	P3->DIR |= (BIT6 | BIT7);
	P3->DS |= (BIT6 | BIT7);
	P3->OUT &= ~(BIT6 | BIT7);
	
	put("Enable pins Initialized\n\n");
}

void initCamera(void) {
	int i;
	
	DisableInterrupts();
	g_sendData = 0;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
	EnableInterrupts();
	
	put("Camera Initialized\n");
	
	/*OLED_Init();
	OLED_display_on();
	OLED_display_clear();
	OLED_display_on();
	for(i = 0; i < 128; i++) {
		lineData[i] = (uint16_t) (8000 * sin(i * 0.05) + 8196);
	}
	OLED_DisplayCameraData(lineData);
	put("OLED Initialized\n\n");*/
}

char updateCamera(void) {
	int i;
	char carpet = 1;
	int total = 0;
	int meow = 0;
	char inLight = 0;
	int der;
	num = 0;
	lightCenter = 0.0f;
	numValley = 0;
	//loop through almost all samples
	for(i = 2; i < 126; i++) {
		//threshold data (now only for carpet stopping!)
		if(lineData[i] > 6000) {
				carpet = 0;
		}
		//take weighted average of all indexes (weight is the lineData)
		total += i * lineData[i];
		num += lineData[i];
		
		//find slope of data for sake of line stopping
		der = lineData[i + 2] - lineData[i - 2];
		//this works if the track pieces arent weird and separated
		if(!inLight && der > 250) {
			inLight = 1;
		} else if(inLight && der < -250) {
			inLight = 0;
			numValley++;
		}
	}
	
	//normalize values as necessary
	lightCenter = (float) total / (float) num;
	
	num = num / 16384;
	
	lightCenter = 64.0f - lightCenter;
	
	//OLED_DisplayCameraData(lineData);
	return carpet;
	//return 0;
}

//enable motor
void enableMotor(char right, char enabled) {
	P3->OUT &= (BIT6 << right);
	P3->OUT |= (BIT6 << right) & (-enabled);
}

//sets duty cycle of selected wheel
void setWheel(char right, double speed) {
	char wheelMod = right << 1;
	if(speed > 0) {
		TIMER_A0_PWM_DutyCycle(speed, 1 + wheelMod);
	} else {
		TIMER_A0_PWM_DutyCycle(-speed, 2 + wheelMod);
	}
}

//sets steering angle of servo
void setSteerAngle(double angle) {
	TIMER_A2_PWM_DutyCycle((angle * STEER_COEF) * 0.02 + 0.075, 1);
}
