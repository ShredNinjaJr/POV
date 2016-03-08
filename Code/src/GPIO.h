#ifndef __GPIO_H__
#define __GPIO_H__

#include "LPC11xx.h"

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#include "pin_label.h"

/* Function Prototypes */
void configureGPIO(GPIO_pin pin, uint8_t dir);
void GPIO_Write(GPIO_pin pin, uint8_t val);
uint8_t GPIO_Read(GPIO_pin pin);

/* configureGPIO
 * Configures the given pin as either an INPUT or OUTPUT
 * Inputs: pin -- name of the GPIO pin to configure
 *         dir -- Direction to configur; either INPUT or OUTPUT
 */
 
void configureGPIO(GPIO_pin pin, uint8_t dir)
{
	//enable clocks to GPIO block
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  16);

	/* Depending on GPIO number, write to the correct port */
	if(pin < PIO1_0)
		LPC_GPIO0->DIR |= (dir << pin);
	else if(pin < PIO2_0)
		LPC_GPIO1->DIR |= (dir << (pin - PIO1_0));
	else if(pin < PIO3_0)
		LPC_GPIO2->DIR |= (dir << (pin - PIO2_0));
	else
		LPC_GPIO3->DIR |= (dir << (pin - PIO3_0));
}

/* GPIO_Write
 * Writes a Digital HIGH or LOW to the specified GPIO Pin
 * Inputs: pin -- Pin to change output on
 *         val -- Value to change output to
 */
void GPIO_Write(GPIO_pin pin, uint8_t val)
{						 
	if(val)
	{
		if(pin < PIO1_0)
			LPC_GPIO0->DATA |= (1 << pin);
		else if(pin < PIO2_0)
			LPC_GPIO1->DATA |= (1 << (pin - PIO1_0));
		else if(pin < PIO3_0)
			LPC_GPIO2->DATA |= (1 << (pin - PIO2_0));
		else
			LPC_GPIO3->DATA |= (1 << (pin - PIO3_0));	
	}
	else
	{
		if(pin < PIO1_0)
			LPC_GPIO0->DATA &= ~(1 << pin);
		else if(pin < PIO2_0)
			LPC_GPIO1->DATA &= ~(1 << (pin - PIO1_0));
		else if(pin < PIO3_0)
			LPC_GPIO2->DATA &= ~(1 << (pin - PIO2_0));
		else
			LPC_GPIO3->DATA &= ~(1 << (pin - PIO3_0));	
	}
}

/* GPIO_Write
 * Writes a Digital HIGH or LOW to the specified GPIO Pin
 * Inputs: pin -- Pin to change output on
 * Return Value: HIGH or LOW based on Pin reading
 */
uint8_t GPIO_Read(GPIO_pin pin)
{
		if(pin < PIO1_0)
			return (LPC_GPIO0->DATA & (1 << pin)) != 0;
		else if(pin < PIO2_0)
			return (LPC_GPIO1->DATA & (1 << (pin - PIO1_0))) != 0;
		else if(pin < PIO3_0)
			return (LPC_GPIO2->DATA & (1 << (pin - PIO2_0))) != 0;
		else
			return (LPC_GPIO3->DATA & (1 << (pin - PIO3_0))) != 0;
}




#endif
