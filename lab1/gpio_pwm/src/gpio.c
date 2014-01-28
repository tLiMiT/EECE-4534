/**
 *@file gpio.c
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *  - gpio skeleton code
 * 
 *
 * Target:   TLL6527v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author    Rohan Kangralkar, ECE, Northeastern University
 * @date      07/08/2010
 *
 * LastChange:
 * $Id: gpio.c 804 2013-02-04 17:22:09Z ovaskevi $
 *
 *******************************************************************************/

#include <stdio.h>
#include "startup.h"
#include <gpio.h>
#include "ADP5588_Driver.h"

#define PORTFIO_FER_ADDR 		0xFFC03200
#define PORTFIO_DIR_ADDR 		0xFFC00730
#define PORTFIO_TOGGLE_ADDR 	0xFFC0070C
#define PORTFIO_SET_ADDR		0xFFC00708
#define PORTFIO_CLEAR_ADDR		0xFFC00704

/** gpio_init
 *
 * Initialization of PORTFIO. This PORT is used as GPIO.
 * The output and input direction can be set using the MACROS
 *
 * Parameters:
 *
 * @return void
 */
void gpio_init(void)
{
    if (fpga_outputToPortFEnable(TLLXBV1_INPUT_PINS) < 0) {
        printf("Failed to enable FPGA Output PINS\n");
        exit(-1);
    }
  
    /** Add your GPIO initialization code below this comment */
    volatile unsigned short *ppPORTFIO_FER = (unsigned short *) PORTFIO_FER_ADDR;
    *ppPORTFIO_FER &= 0x0000;
    volatile unsigned short *ppPORTFIO_DIR = (unsigned short *) PORTFIO_DIR_ADDR;
    *ppPORTFIO_DIR |= 0x00FF;
}

static inline void busyWait(unsigned int cycles) {
	while (cycles--) {
		asm("nop;");
		asm("ssync;");
	}
}

/** gpio_run
 *
 * The main command loop. Write all the control commands in this function
 *
 * Parameters:
 *
 * @return void
 */
void gpio_run(void)
{
	volatile unsigned short *ppPORTFIO_TOGGLE = (unsigned short *) PORTFIO_TOGGLE_ADDR;
	volatile unsigned short *ppPORTFIO_CLEAR = (unsigned short *) PORTFIO_CLEAR_ADDR;

	// Clear
	*ppPORTFIO_CLEAR |= 0x00FF;
	asm("ssync;");

	while (1) {
/*
		// BLINK
		*ppPORTFIO_TOGGLE |= 0x00FF;
		asm("ssync;");
		busyWait(5000000);
		*ppPORTFIO_TOGGLE |= 0x00FF;
		asm("ssync;");
		busyWait(5000000);
/*/
		// BOUNCE
		*ppPORTFIO_TOGGLE |= 1 << 0;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 0;
		*ppPORTFIO_TOGGLE |= 1 << 1;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 1;
		*ppPORTFIO_TOGGLE |= 1 << 2;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 2;
		*ppPORTFIO_TOGGLE |= 1 << 3;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 3;
		*ppPORTFIO_TOGGLE |= 1 << 4;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 4;
		*ppPORTFIO_TOGGLE |= 1 << 5;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 5;
		*ppPORTFIO_TOGGLE |= 1 << 6;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 6;
		*ppPORTFIO_TOGGLE |= 1 << 7;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 7;
		*ppPORTFIO_TOGGLE |= 1 << 6;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 6;
		*ppPORTFIO_TOGGLE |= 1 << 5;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 5;
		*ppPORTFIO_TOGGLE |= 1 << 4;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 4;
		*ppPORTFIO_TOGGLE |= 1 << 3;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 3;
		*ppPORTFIO_TOGGLE |= 1 << 2;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 2;
		*ppPORTFIO_TOGGLE |= 1 << 1;
		asm("ssync;");
		busyWait(800000);
		*ppPORTFIO_TOGGLE |= 1 << 1;

	}
}
 
