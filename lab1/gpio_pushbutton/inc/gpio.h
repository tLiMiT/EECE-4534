
/**
 *@file gpio.h
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *	- GPIO skeleton
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
#ifndef GPIO_H_
#define GPIO_H_


/** gpio_init
 *
 * Initialization of PORTFIO. This PORT is used as GPIO.
 * The output and input direction can be set using the MACROS
 *
 * Parameters:
 *
 * @return void
 */
void gpio_init(void);

/** gpio_init
 *
 * The main command loop. Write all the control commands in this function
 *
 * Parameters:
 *
 * @return void
 */
void gpio_run(void);

void fpga_gpio_button(void);

#endif /* GPIO_H_ */
