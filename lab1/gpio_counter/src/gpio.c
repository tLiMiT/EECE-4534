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
	volatile unsigned short *ppPORTFIO_SET = (unsigned short *) PORTFIO_SET_ADDR;
	volatile unsigned short *ppPORTFIO_CLEAR = (unsigned short *) PORTFIO_CLEAR_ADDR;

	// Clear
	*ppPORTFIO_CLEAR |= 0x00FF;
	asm("ssync;");

	int num = 16;
	printf("\nInput an int:\n");
	scanf("%d", &num);

	if (num > 256) {
		printf("\r\n Warning: Input must be less than 256 for proper display.");
	}

	int i, j, setBit, value, bit;
	for (i = 0; i < num; i++) {
		// Clear
		*ppPORTFIO_CLEAR |= 0x00FF;
		asm("ssync;");

		setBit = i;

		for (j = 0; j < 8; j++) {
			value = setBit / 2;
			bit = setBit % 2;

			if (bit == 1) { *ppPORTFIO_SET |= 1 << j; }

			setBit = value;
			asm("ssync;");
			busyWait(1000000);
		}
		asm("ssync;");
		busyWait(1000000);
	}

}
 
