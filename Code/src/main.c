#include <stdio.h>
#include "LPC11xx.h"
#include "GPIO.h"
#include "font_table.h"

/* Header for pin numbers for LPC1114FBD48/302 */
#include "LPC1114FBD48_302_label.h"


int main()
{
	const GPIO_pin ledPin = PIN_23;
	const GPIO_pin buttonPin = PIO1_9;
	const GPIO_pin hall_effect_pin = PIN_13;
	configureGPIO(ledPin, OUTPUT);
	configureGPIO(buttonPin, INPUT);
	configureGPIO(hall_effect_pin, INPUT);
	while (1)
	{
		GPIO_Write(ledPin, GPIO_Read(hall_effect_pin)^ GPIO_Read(buttonPin));
	}
}


/* 

font_table[char][height][width]
for each char in the string: 
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
				led[j] = char[j][i];
		}
		delay(x);
	}


*/
