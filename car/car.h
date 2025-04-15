#ifndef CAR_WOW
#define CAR_WOW

#include "TimerA.h"
#include "uart.h"
#include "msp.h"
#include "oled.h"
#include "Timer32.h"
#include "CortexM.h"
#include "ADC14.h"
#include "ControlPins.h"

void initCar(void);
void initCamera(void);
char updateCamera(void);
void enableMotor(char left, char enabled);
void setWheel(char left, double speed);
void setSteerAngle(double angle);

#endif
