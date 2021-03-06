/**
 *@file gpio_interrupt.c
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *  - gpio input example using interrupts.The leds corresponding to the push
 *  buttons are turned on/off
 * 
 *
 * Target:   TLL6527v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 *@author    Rohan Kangralkar, ECE, Northeastern University
 *@date      01/16/2010
 *
 * LastChange:
 * $Id: gpio_interrupt.c 804 2013-02-04 17:22:09Z ovaskevi $
 *
 *******************************************************************************/

#include <stdio.h>
#include "startup.h"
#include "tll_config.h"
#include <sys/exception.h>
#include <gpio_interrupt.h>
#include "ADP5588_Driver.h"

int bSW2Pressed = 0;
int bSW3Pressed = 0;

/******************************************************************************
 *                     DEFINES
 *****************************************************************************/
 /* 
 * Please reffer to the mapping.
 * The first 8 bits are output. 
 * PF0_PPID0_DROPRI    FPGA_GPIO0
 * .............................
 */

/******************************************************************************
 *                     STATIC GLOBALS
 *****************************************************************************/

/** 
 *
 * This is the interrupt service routine that should be registered with the
 * register_handler().
 * Push Button ISR. 
 * 
 * Parameters:
 *
 * @return void
 */
/* Define interrupt service routine for handling GPIO interrupts */
void Pushbutton_ISR()__attribute__((interrupt_handler));

void  Pushbutton_ISR ()
{
    /* clear interrupt and put your user code here */

	printf("Entering Interrupt\n");

	unsigned short data = (*pPORTFIO) >> 8;

	//printf("Data - %02x\n",data);
	if(data == 1 && !bSW2Pressed)
	{
		//printf("Interrupt SW2 Button pressed\n");
		bSW2Pressed = 1;
	}
	else if(data == 2 && !bSW3Pressed)
	{
		//printf("Interrupt SW3 Button pressed\n");
		bSW3Pressed = 1;
	}

	*pPORTFIO_CLEAR = 0xFF00;	// Clear interrupt at GPIO


    /* Creates delay for CCLK SCLK ratio */
    asm("nop;");
    asm("ssync;");
    asm("nop;");    
    asm("ssync;");
} 

 
 
/** 
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
         
    /* Put your initialization code below this comment */
    *pPORTF_FER &= 0x0000;			// GPIO
	*pPORTFIO_DIR |= 0x00FF;		// Input / Output
	*pPORTFIO_INEN |= 0xFF00;		// Enable / Ignore
	*pPORTFIO_EDGE |= 0xFF00;		// Edge
	*pPORTFIO_BOTH &= 0x0000;		// Not Both
	*pPORTFIO_POLAR &= 0x0000;		// Rising
	*pPORTFIO_MASKA |= 0xFF00;		// Enable interrupts for 16-8

	/* Interrupt Handler */
	//Enable SIC interrupt
	*pSIC_IMASK1 |= 0x2000; // 13
	// install handler
	// register_handler(IVGx, ISR_Name)
	register_handler(ik_ivg13, Pushbutton_ISR);

	*pPORTFIO = 0x0000;

	
	//Timer init
	*pTIMER_DISABLE = 0xFF;
	*pTIMER_STATUS = TIMIL1 | TOVF_ERR1 | TRUN1 | TRUN0;
	asm("nop;");
	asm("ssync;");
	if (!*pTIMER_STATUS) {
		*pTIMER1_CONFIG = PWM_OUT | PERIOD_CNT | PULSE_HI;
		*pTIMER1_PERIOD = 2000000;
		*pTIMER1_WIDTH = 150000;
		*pTIMER_ENABLE = 2;
		asm("nop;");
		asm("ssync;");
	}
	else {
		printf("Timer failed.\n");
	}

}

