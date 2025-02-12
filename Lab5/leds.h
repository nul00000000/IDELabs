#ifndef __LEDS__
#define __LEDS__

#include "Common.h"

#define RED 0b0001
#define GREEN 0b0010
#define BLUE 0b0100
#define CYAN 0b0110
#define MAGENTA 0b0101
#define YELLOW 0b0011
#define WHITE 0b0111

void LED1_Init(void);
void LED2_Init(void);
BOOLEAN LED1_State(void);
void LED1_On(void);
void LED1_Off(void);

#endif
