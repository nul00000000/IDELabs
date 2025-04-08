#include "car.h"
#include "math.h"

#define STEER_COEF (1.0/1.0)

uint16_t lineData[128];
uint16_t procData[128];
char g_sendData;

int lightCenter = 0;

void initCar(void) {
	uart0_init();
	
	put("UART0 Initialized\n\n");
	
	TIMER_A0_PWM_Init(10000, 0.0, 3);
	TIMER_A0_PWM_Init(10000, 0.0, 4);
	TIMER_A0_PWM_Init(10000, 0.0, 1);
	TIMER_A0_PWM_Init(10000, 0.0, 2);
	
	put("DC Motors Initialized\n");
	
	TIMER_A2_PWM_Init(50, 0.0, 1);
	
	put("Steering Servo Initialized\n\n");
	
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
	OLED_DisplayCameraData(lineData);*/
	put("OLED Initialized\n\n");
}

void updateCamera(void) {
	int i;
	int num = 0;
	lightCenter = 0;
	for(i = 0; i < 128; i++) {
		procData[i] = (lineData[i] > 10000) * 8000;
		if(procData[i]) {
			lightCenter += i;
			num++;
		}
	}
	
	lightCenter /= num;
	
	lightCenter = 64 - lightCenter;
	
	if(num == 0) {
		lightCenter = 400;
	}
	
	//OLED_DisplayCameraData(procData);
}

void enableMotor(char right, char enabled) {
	P3->OUT &= (BIT6 << right);
	P3->OUT |= (BIT6 << right) & (-enabled);
}

void setWheel(char right, double speed) {
	char wheelMod = right << 1;
	if(speed > 0) {
		TIMER_A0_PWM_DutyCycle(speed, 1 + wheelMod);
	} else {
		TIMER_A0_PWM_DutyCycle(-speed, 2 + wheelMod);
	}
}

void setSteerAngle(double angle) {
	TIMER_A2_PWM_DutyCycle((angle * STEER_COEF) * 0.02 + 0.075, 1);
}
