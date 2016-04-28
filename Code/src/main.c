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

/* Interrupt handler prototypes*/
void PIOINT2_IRQHandler(void);
void SysTick_Handler (void);

/* Delay for time milliseconds */
void delay(int time);

/* GPIO pin layout of the LEDs*/
const GPIO_pin ledPins[] = {PIN_13, PIN_14, PIN_15, PIN_16, 
														PIN_17, PIN_18, PIN_19, PIN_20,
											      PIN_21, PIN_22, PIN_23, PIN_24
													 };

/* Keeps track of milliseconds between time 
 * the hall effect sensor interrupts */
volatile int counter = 0;	
/* counter used for the delay function to keep track of time*/
volatile int delay_counter = 0;
/*Interrupt flag for the hall effect sensor. Set High on interrupt*/
volatile int intr_flag = 0;						 
													
int height = 8, width = 8;
char input[] = "H";//ello!";												
int DELAY = 3000;

/* Circular array to keep track of the time between each
 * Hall effect interrupt. This array is used to calculate
 * the running average of the time between revolutions
 */													 
#define QUEUE_SIZE 10
struct queue
{
		int data[QUEUE_SIZE];
		int write_idx;
} timer_queue;
						

/* Interrupt handler for GPIO2 */
void PIOINT2_IRQHandler(void)
{
		LPC_GPIO2->IC |= (1 << 8);

		timer_queue.data[timer_queue.write_idx] = counter;
		timer_queue.write_idx = (timer_queue.write_idx + 1) % QUEUE_SIZE;
		printf("HALL\r\n");
		counter = 0;
		intr_flag = 1;
}

/* Interrupt handler for system Timer */
void SysTick_Handler (void)
{
	counter++;
	delay_counter++;
}


/* delay
 * Inputs: time -- time to delay in milliseconds
 * Return value: Returns after the specified time
 */
void delay(int time)
{
		delay_counter = 0;
		
		while(delay_counter < time);
	
		return;
}


/* InitializeSysTickTimer
 * Inputs: frequency -- The frequency in Hz to initialize the timer to.
 * Side Effects: Sets up the system registers to initialize the system timer
 */
void InitializeSysTickTimer(int frequency)
{
	/* Program the SYST_RVR register with the reload value RELOAD to obtain the desired
time interval.*/
	SysTick->LOAD = (PROCESSOR_FREQ / frequency) - 1;	/* INITIALIZED TO 1 millisecond*/
	/*Clear the SYST_CVR register*/
	SysTick->VAL = 0;
	/* Program the SYST_SCR register with the value 0x7 which enables the SysTick timer
and the SysTick timer interrupt.*/
	SysTick->CTRL |= 0x7;
	
}
													 
int main()
{
	
	const GPIO_pin hall_effect_pin = PIN_12;
	int i, j, k, l;	/* Loop variables */
	int MSPR = 0;	/* Milliseconds per revolution */
	SER_init();	/* Initialize serial IO*/
	
	/* Configure the GPIO pins */
	for( i = 0; i < 12; i++)
		configureGPIO(ledPins[i], OUTPUT);
	
	for( i = 0; i < 12; i++)
		GPIO_Write(ledPins[i], 0);

	configureGPIO(hall_effect_pin, INPUT);
	
	/* Initialize the system Timer and queue*/
	InitializeSysTickTimer(1000);
	/* Initialize timer_queue */
	for(i = 0; i < QUEUE_SIZE; i++)
	{
			timer_queue.data[i] = 0;
			
	}
	timer_queue.write_idx = 0;
	
	
	/* Enable interrupts on PIO2_1 */
	LPC_GPIO2->IE |= (1 << 8); /* Unmask the interrupt */
	LPC_GPIO2->IS &= ~(1 << 8); /* Unmask the interrupt */
	NVIC_EnableIRQ(EINT2_IRQn); /* Enable the interrupt in the NVIC*/

	
	/* Main loop */
	while (1)
	{
		
		/* Clear all the LEDs and wait for the hall effect interrupt */
		for(k = 0; k < height; k++)
				GPIO_Write(ledPins[k],0);
		
		intr_flag = 0;
		while(!intr_flag);
		
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
					
			}
			/* Calculate the Rotation time */
			MSPR = 0;
			for(l = 0; l < QUEUE_SIZE; l++)
			{
					MSPR +=	timer_queue.data[l];
			}
			MSPR /= QUEUE_SIZE;
			printf("MS per REV = %d\r\n", MSPR);
			delay(MSPR/2);
			
		}
	}
	

	
}

