#include <stdio.h>
#include <string.h>
#include "LPC11xx.h"
#include "GPIO.h"
#include "font_table.h"

/* Header for pin numbers for LPC1114FBD48/302 */
#include "LPC1114FBD48_302_label.h"

/* Import external functions from Serial.c file                               */
extern void SER_init (void);

int main()
{
	const GPIO_pin ledPin = PIN_23;
	const GPIO_pin buttonPin = PIO1_9;
	const GPIO_pin hall_effect_pin = PIN_13;
	int i, j, k;
	int height = 8, width = 8;
	char input[] = "Sahil, I am your father!";
	SER_init();
	
	configureGPIO(ledPin, OUTPUT);
	configureGPIO(buttonPin, INPUT);
	configureGPIO(hall_effect_pin, INPUT);
	
	
	
	while (1)
	{
		for(i =0; i < strlen(input); i++)
		{
			for(j = 0; j < height; j++)
			{
					for( k = 0; k < width; k++)
					{
							char temp = font_table[input[i] - ' '][j];
							if(temp & (0x80 >> k))
								printf("1");
							else
								printf(" ");
					}
					printf("\r\n");
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
