/**
 *@file HelloWorld.c
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *  - Example of standard I/O
 * 
 * 1. Output "Hello, World!" 
 *
 * @author    Rohan Kangralkar, ECE Department Northeastern University
 * @date      01/16/2010
 *
 * LastChange:
 * $Id: HelloWorld.c 774 2012-02-01 14:52:03Z rkangralkar $
 *
 *******************************************************************************/

 
#include <stdio.h>
#include <tll_config.h>
#include "startup.h"
#include "tll_common.h"

/** 
 *
 * Main function for standard output
 *
 * Parameters:
 * @param argc - not used
 * @param argv - not used
 *
 * @return int
 */ 
int main( int argc, char *argv[] )
{
	int status = 0;

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

	double tempF = 0, tempC = 0;
	tempF = 32;

	printf("\nInput a temperature (F):\n");
	scanf("%f", &tempF);

	// Convert to degrees Celsius
	tempC = (tempF-32) * 5/9;

	if (tempC <= 0) {
		printf("Warning: Freezing!\n");
	}

	printf("%f degrees Fahrenheit is equivalent to %f degrees Celsius", tempF, tempC);

	return 0;
}
