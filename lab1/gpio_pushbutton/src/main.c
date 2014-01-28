/**
 *@file main.c
 *
 *@brief Testing code for External I/O abstract
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author    Rohan Kangralkar, ECE, Northeastern University
 * @date      07/08/2010
 *
 * LastChange:
 * $Id: main.c 467 2010-07-08 20:56:50Z rkangral $
 *
 *******************************************************************************/

#include <stdio.h>
#include <tll_config.h>
#include "startup.h"
#include "tll_common.h"
#include <gpio.h>

/**
 *
 * Main function for gpio skeleton
 *
 * Parameters:
 *
 * @return int
 */
int main(void) {
    int status = PASS;

	/* Blackfin setup function to configure processor */
	status = blackfin_setup(); //returns 0 if successful and -1 if failed
	if (status) {
		printf("\r\n Blackfin Setup Failed");
		return -1;
	}

	/* FPGA setup function to configure FPGA, make sure the FPGA configuration
	 binary data is loaded in to SDRAM at "FPGA_DATA_START_ADDR" */
	status = fpga_setup(); //returns 0 if successful and -1 if failed
	if (status) {
		printf("\r\n FPGA Setup Failed");
		return -1;
	}

	printf("[MAIN]: Starting GPIO Demo\n");

	// initialize external IO component and configure the input and output
	gpio_init();

	// The main control function
	fpga_gpio_button();

    return status;
}
