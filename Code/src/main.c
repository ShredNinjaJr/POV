#include <stdio.h>
#include <string.h>
#include "LPC11xx.h"
#include "GPIO.h"
#include "font_table.h"
#include "time.h"

/* Header for pin numbers for LPC1114FBD48/302 */
#include "LPC1114FBD48_302_label.h"

/* Import external functions from Serial.c file                               */
extern void SER_init (void);

const GPIO_pin ledPins[] = {PIN_13, PIN_14, PIN_15, PIN_16, 
														PIN_17, PIN_18, PIN_19, PIN_20,
											      PIN_21, PIN_22, PIN_23, PIN_24
													 };

int main()
{
	const GPIO_pin ledPin = PIN_13;
	
	const GPIO_pin buttonPin = PIN_25;
	const GPIO_pin hall_effect_pin = PIN_13;
	int i, j, k, l;
	int height = 8, width = 8;
	char input[] = "Hello World!";
	SER_init();
	for( i = 0; i < 12; i++)
		configureGPIO(ledPins[i], OUTPUT);
	
	for( i = 0; i < 12; i++)
		GPIO_Write(ledPins[i], 0);
	
	configureGPIO(buttonPin, INPUT);
	configureGPIO(hall_effect_pin, INPUT);
	#define DELAY 30000

	
	
	
	
	while (1)
	{
		for(i =0; i < strlen(input); i++)
		{
			for(j = 0; j < width; j++)
			{
					for( k = 0; k < height; k++)
					{
							char temp = font_table[input[i] - ' '][k];
							GPIO_Write(ledPins[k],(temp & (0x80 >> j)));
						//printf("%d\r\n", (temp & (0x80 >> j))? 1 : 0);
					}
					
					for( l = 0; l < DELAY; l++);
			}
			
		}
	}
	
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
