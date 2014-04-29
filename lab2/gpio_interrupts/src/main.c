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
    
    /* Blackfin setup function to configure processor */
    ret = blackfin_setup(); //returns 0 if successful and -1 if failed
    if (ret) {
        printf("\r\n Blackfin Setup Failed"); 
        return -1;
    }
    /* FPGA setup function to configure FPGA, make sure the FPGA configuration
	   binary data is loaded in to SDRAM at "FPGA_DATA_START_ADDR" */
    ret = fpga_setup(); //returns 0 if successful and -1 if failed
    if (ret) {
        printf("\r\n FPGA Setup Failed"); 
        return -1;
    }

    printf("Initializing GPIO and interrupt handler\n");
    /* initialize PORTF and register the interrupt handler*/
    gpio_init();

    /* the program waits here forever and responds to interrupts in the interrupt
     * handler. This loop is necessary so that the program does not run to
     * completion.
     * The "idle" command puts the processor into power saving idle mode
     */

    printf("Entering While Loop\n");
    while(1) {
    	// suspend core execution until next interrupt 
    	asm("IDLE;");
    }
    return ret;
}
 
