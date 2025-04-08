#include "Common.h"

#define put(x) uart0_put(x)

void uart0_init(void);
void uart0_putchar(char character);
BYTE uart0_getchar(void);
void uart0_put(char*);
BOOLEAN uart0_dataAvailable(void);

void uart2_init(void);
void uart2_putchar(char character);
BYTE uart2_getchar(void);
void uart2_put(char*);
BOOLEAN uart2_dataAvailable(void);

void LED1_Init(void);
void LED2_Init(void);
