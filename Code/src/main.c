#include <stdio.h>
#include <string.h>
#include "LPC11xx.h"
#include "GPIO.h"
#include "font_table.h"
#include "time.h"

/* Header for pin numbers for LPC1114FBD48/302 */
#include "LPC1114FBD48_302_label.h"

#define PROCESSOR_FREQ   50000000
/* Import external functions from Serial.c file                               */
extern void SER_init (void);

const GPIO_pin ledPins[] = {PIN_13, PIN_14, PIN_15, PIN_16, 
														PIN_17, PIN_18, PIN_19, PIN_20,
											      PIN_21, PIN_22, PIN_23, PIN_24
													 };

void PIOINT2_IRQHandler(void);
volatile int intr = 0;

volatile int counter = 0;
volatile int delay_counter = 0;
													 
int height = 8, width = 8;
char input[] = "Hello!";												
int DELAY = 3000;
													 
#define QUEUE_SIZE 10
struct queue
{
		int data[QUEUE_SIZE];
		int write_idx;
} timer_queue;
						

void PIOINT2_IRQHandler(void)
{
		LPC_GPIO2->IC |= (1 << 8);
		
		timer_queue.data[timer_queue.write_idx] = counter;
		timer_queue.write_idx = (timer_queue.write_idx + 1) % QUEUE_SIZE;
		counter = 0;
}


void SysTick_Handler (void)
{
	counter++;
	delay_counter++;
}

void delay(int time)
{
		delay_counter = 0;
		
		while(delay_counter < time);
	
		return;
}

void InitializeSysTickTimer(int frequency)
{
	/* Program the SYST_RVR register with the reload value RELOAD to obtain the desired
time interval.*/
	SysTick->LOAD = (PROCESSOR_FREQ / frequency) - 1;	/* INITIALIZED TO 1 millisecond*/
	printf("SYSTick counter = %d\r\n",(PROCESSOR_FREQ / frequency) - 1);
	/*Clear the SYST_CVR register*/
	SysTick->VAL = 0;
	/* Program the SYST_SCR register with the value 0x7 which enables the SysTick timer
and the SysTick timer interrupt.*/
	SysTick->CTRL |= 0x7;
	
}
													 
int main()
{
	//const GPIO_pin ledPin = PIN_13;
	
	const GPIO_pin buttonPin = PIN_25;
	const GPIO_pin hall_effect_pin = PIN_12;
	int i, j, k, l;
	int MSPR = 0;
	SER_init();
	for( i = 0; i < 12; i++)
		configureGPIO(ledPins[i], OUTPUT);
	
	for( i = 0; i < 12; i++)
		GPIO_Write(ledPins[i], 0);
	
	configureGPIO(buttonPin, INPUT);
	configureGPIO(hall_effect_pin, INPUT);
	InitializeSysTickTimer(1000);
	/* Initialize timer_queue */
	for(i = 0; i < QUEUE_SIZE; i++)
	{
			timer_queue.data[i] = 0;
	}
	timer_queue.write_idx = 0;
	
	
	#ifndef INTERRUPT
	/* Enable interrupts on PIO2_1 */
	
	/* Unmask the interrupt */
	LPC_GPIO2->IE |= (1 << 8);
	LPC_GPIO2->IS &= ~(1 << 8);
	NVIC_EnableIRQ(EINT2_IRQn);
		
	#endif
	
	
	#ifndef INTERRUPT
	while (1)
	{
		
		for(k = 0; k < height; k++)
				GPIO_Write(ledPins[k],0);
		
		while(LPC_GPIO2->DATA & (1 << 8));
		for(i =0; i < strlen(input); i++)
		{
			for(j = 0; j < width; j++)
			{
					for( k = 0; k < height; k++)
					{
							char temp = font_table[input[i] - ' '][k];
							GPIO_Write(ledPins[k],1);//(temp & (0x80 >> j)));
						//printf("%d\r\n", (temp & (0x80 >> j))? 1 : 0);
					}
					/* Calculate the Rotation time */
					MSPR = 0;
					for(l = 0; l < QUEUE_SIZE; l++)
					{
							MSPR +=	timer_queue.data[i];
					}
					MSPR /= QUEUE_SIZE;
					delay(MSPR/2);
			}
			
		}
	}
	#endif
	

	
}


/* 

font_table[char][height][width]
for each char in the string: 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
				led[j] = font_table[char - ' '][j][i];
		}
		delay(x);		
	}


*/
