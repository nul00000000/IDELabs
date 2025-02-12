#include "msp.h"
#include "common.h"
#include "leds.h"

#define RED 0b0001
#define GREEN 0b0010
#define BLUE 0b0100
#define CYAN 0b0110
#define MAGENTA 0b0101
#define YELLOW 0b0011
#define WHITE 0b0111

void LED1_Init(void)
{
	// configure PortPin for LED1 as port I/O 
	P1->SEL0 &= ~BIT0; // select Port1 Bit0 for use as port I/O
	P1->SEL1 &= ~BIT0; // select Port1 Bit0 for use as port I/O

	// make built-in LED1 LED high drive strength
	P1->DS |= BIT0; //set high drive stregth

	// make built-in LED1 out	 
	P1->DIR |= BIT0;  // set BIT 0 as output

	// turn off LED
	P1->OUT &= ~BIT0; // set P1.0 to '0'
}

void LED2_Init(void)
{
	// configure PortPin for LED2 as port I/O 
	P2->SEL0 &= ~BIT0; // select Port2 Bit0 for use as port I/O
	P2->SEL1 &= ~BIT0; // select Port2 Bit0 for use as port I/O
	
	P2->SEL0 &= ~BIT1; // select Port2 Bit1 for use as port I/O
	P2->SEL1 &= ~BIT1; // select Port2 Bit1 for use as port I/O
	
	P2->SEL0 &= ~BIT2; // select Port2 Bit2 for use as port I/O
	P2->SEL1 &= ~BIT2; // select Port2 Bit2 for use as port I/O

	// make built-in LED2 LEDs high drive strength
	P2->DS |= BIT0; //set high drive stregth
	P2->DS |= BIT1; //set high drive stregth
	P2->DS |= BIT2; //set high drive stregth

	// make built-in LED2 out	 
	P2->DIR |= BIT0;  // set BIT 0 as output
	P2->DIR |= BIT1;  // set BIT 1 as output
	P2->DIR |= BIT2;  // set BIT 2 as output

	// turn off LED
	P2->OUT &= ~BIT0; // set P2.0 to '0'
	P2->OUT &= ~BIT1; // set P2.1 to '0'
	P2->OUT &= ~BIT2; // set P2.2 to '0'
}

BOOLEAN LED1_State(void)
{
	return (P1->OUT & BIT0) != 0;
}

void LED1_On(void)
{
	P1->OUT |= BIT0; // set P1.0 to '1'
}

void LED1_Off(void)
{
	P1->OUT &= ~BIT0; // set P1.0 to '0'
}
