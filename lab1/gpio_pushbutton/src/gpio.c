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
#define PORTFIO_INEN_ADDR		0xFFC00740
#define PORTFIO_EDGE_ADDR		0xFFC00738
#define PORTFIO_POLAR_ADDR		0xFFC00734
#define PORTFIO_DATA_ADDR		0xFFC00700


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
    volatile unsigned short *ppPORTFIO_INEN = (unsigned short *) PORTFIO_INEN_ADDR;
    *ppPORTFIO_INEN |= 0xFF00;
    volatile unsigned short *ppPORTFIO_POLAR = (unsigned short *) PORTFIO_POLAR_ADDR;
    *ppPORTFIO_POLAR &= 0xFF00;
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
	volatile unsigned short *ppPORTFIO_DATA = (unsigned short *) PORTFIO_DATA_ADDR;

	// Clear
	*ppPORTFIO_CLEAR |= 0x00FF;
	asm("ssync;");

	int num = 5;
	printf("\nInput a number (0-7):\n");
	scanf("%d", &num);

	if (num > 7) {
		printf("\r\n Warning: Input must be less than 8.");
	}

	*ppPORTFIO_SET |= 1 << num;

	while (1) {
		volatile unsigned short data = *ppPORTFIO_DATA;
		volatile unsigned short gpio8 = (data >> 8) & 1;
		volatile unsigned short gpio9 = (data >> 9) & 1;
		if(gpio8 && gpio9)
		{
			printf("Both pressed\n");
		}
		else
		{
			if (gpio8 == 1) {
				printf("SW2 pressed\n");
			}
			if (gpio9 == 1) {
				printf("SW3 pressed\n");
			}
		}

	}

}
 
void fpga_gpio_button(void) {
	volatile unsigned short *ppPORTFIO_TOGGLE = (unsigned short *) PORTFIO_TOGGLE_ADDR;
	volatile unsigned short *ppPORTFIO_CLEAR = (unsigned short *) PORTFIO_CLEAR_ADDR;
	volatile unsigned short *ppPORTFIO_DATA = (unsigned short *) PORTFIO_DATA_ADDR;
	int bPressed1 = 0;
	int bPressed2 = 0;
	int bBothPressed = 0;
	volatile unsigned short data = 0;
	volatile unsigned short gpio8 = 0;
	volatile unsigned short gpio9 = 0;

	// Clear
	*ppPORTFIO_CLEAR |= 0x00FF;
	asm("ssync;");

	while (1) {
		data = *ppPORTFIO_DATA;
		gpio8 = (data >> 8) & 1;
		gpio9 = (data >> 9) & 1;
		if(bPressed1 && bPressed2)
		{
			printf("Both pressed\n");
		}
		else
		{
			if (gpio8 == 1 && bPressed1 == 0) {
				bPressed1 = 1;
				printf("SW2 pressed\n");
				*ppPORTFIO_TOGGLE |= 1 << 0;
			}
			if (gpio9 == 1 && bPressed2 == 0) {
				bPressed2 = 1;
				printf("SW3 pressed\n");
				*ppPORTFIO_TOGGLE |= 1 << 7;
			}
		}
		if (!gpio8 && bPressed1)
		{
			printf("SW2 released\n");
			bPressed1 = 0;
		}
		if (!gpio9 && bPressed2)
		{
			printf("SW3 released\n");
			bPressed2 = 0;
		}

	}
}
