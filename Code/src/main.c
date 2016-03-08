#include <stdio.h>
#include "LPC11xx.h"
#include "GPIO.h"


int main()
{
	const GPIO_pin ledPin = PIO2_1;
	const GPIO_pin buttonPin = PIO1_9;
	configureGPIO(ledPin, OUTPUT);
	configureGPIO(buttonPin, INPUT);
	while (1)
	{
		GPIO_Write(ledPin,GPIO_Read(buttonPin));
	}
}
