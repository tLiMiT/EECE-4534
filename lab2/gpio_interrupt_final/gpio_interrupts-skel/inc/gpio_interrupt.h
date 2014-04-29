/**
 *@file gpio_interrupt.h
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *	- GPIO Pushbutton example
 *
 *
 * Target:   TLL6527v1-1
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 *@author    Rohan Kangralkar, ECE, Northeastern University
 *@date      07/08/2010
 *
 * LastChange:
 * $Id$
 *
 *******************************************************************************/
#ifndef GPIO_INTERRUPT_H_
#define GPIO_INTERRUPT_H_

extern int bSW2Pressed;
extern int bSW3Pressed;

/**
 *
 * Initialization of PORTFIO. This PORT is used as GPIO.
 * The output and input direction can be set using the MACROS
 *
 * Parameters:
 *
 * @return void
 */
void gpio_init(void);


#endif /* GPIO_INTERRUPT_H_ */
