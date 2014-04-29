/**
 *@file: main.c
 *
 *@brief: 
 *  - Introduction lab for the TLL6527 platform
 *  - gpio input using interrupt example
 * 
 *
 * Target:   TLL6527v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 *@author    Rohan Kangralkar, ECE, Northeastern University
 *@date      01/16/2010
 *
 * LastChange:
 * $Id: main.c 465 2010-07-08 18:39:47Z rkangral $
 *
 *******************************************************************************/
 
#include <stdio.h>
#include "startup.h"
#include "tll_config.h"
#include <gpio_interrupt.h>
#include "FPGA_TMR1_reroute.bin.h"

#define PWM_MIN -15
#define PWM_MAX 15
#define PWM_NEUTRAL 0



/** 
 *
 * Main function for GPIO interrupts skeleton
 *
 * Parameters:
 * @param argc - not used
 * @param argv - not used
 *
 * @return int
 */
int main( int argc, char *argv[] )
{
	int             ret             = 0;
	int Counter = 0;
	int bMax = 0;
	int bMin = 0;

	/* Blackfin setup function to configure processor */
	ret = blackfin_setup(); //returns 0 if successful and -1 if failed
	if (ret) {
		printf("\r\n Blackfin Setup Failed");
		return -1;
	}
	/* FPGA setup function to configure FPGA, make sure the FPGA configuration
	   binary data is loaded in to SDRAM at "FPGA_DATA_START_ADDR" */
	ret = fpga_setup(); //returns 0 if successful and -1 if failed
	//ret = fpga_programmer((unsigned char*) FPGA_TMR1_reroute_bin, sizeof(FPGA_TMR1_reroute_bin));
	if (ret) {
		printf("\r\n FPGA Setup Failed");
		return -1;
	}


	/* initialize PORTF and register the interrupt handler*/
	gpio_init();



	/* the program waits here forever and responds to interrupts in the interrupt
	 * handler. This loop is necessary so that the program does not run to
	 * completion.
	 * The "idle" command puts the processor into power saving idle mode
	 */
	while(1) {
		// suspend core execution until next interrupt
		//Increment counter and clear buttons


		//SW2 Pressed, increment counter
		if(bSW2Pressed)
		{
			printf("SW2 Pressed\n");
			printf("Counter: %d\n", Counter);

			if(!bMax) { Counter++; }
			else { printf("Maximum\n"); }

			bSW2Pressed = 0;
		}
		//SW3 Pressed, decrement counter
		if(bSW3Pressed)
		{
			printf("SW3 Pressed\n");
			printf("Counter: %d\n", Counter);

			if(!bMin) { Counter--; }
			else { printf("Minimum\n"); }

			bSW3Pressed = 0;
		}


		//Check for max/min values

		//Normal Mode
		if(Counter < PWM_MAX && Counter > PWM_MIN)
		{
			bMax = 0;
			bMin = 0;
		}
		//Min mode
		else if(Counter <= PWM_MIN && !bMin)
		{
			printf("Minimum\n");
			bMin = 1;
		}

		//Max mode
		else if(Counter >= PWM_MAX && !bMax)
		{
			printf("Maximum\n");
			bMax = 1;
		}
		else
		{
			//No change needed
		}

		//Set LEDs
		if(bMin)
		{
			*pPORTFIO_SET |= 1;
		}
		else if(bMax)
		{
			*pPORTFIO_SET |= (1 << 7);
		}
		else
		{
			//Normal mode clear LEDs
			*pPORTFIO = 0x00;
		}

		//Set Timer Width
		*pTIMER1_WIDTH = Counter *6000 + 150000;

		//Wait for cycles
		asm("nop");
		asm("ssync");

	}
	return ret;
}

