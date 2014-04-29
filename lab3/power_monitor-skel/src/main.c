/**
 *@file main.c
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *  - Power monitor code. The module switches the core to Active and fullon and
 *  logs the power reading for analysis.
 *
 *
 * Target:   TLL6527v1-1
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 *@author    Rohan Kangralkar, ECE, Northeastern University
 *@date      01/16/2010
 *
 * LastChange:
 * $Id: main.c 466 2010-07-08 20:49:02Z rkangral $
 *
 *******************************************************************************/
#include <stdio.h>
#include "startup.h"
#include <power_monitor.h>

/**
 *
 * Main function for switching the core to fullon and active and reading the Power
 *
 * Parameters:
 * @param argc - not used
 * @param argv - not used
 *
 * @return int
 */
int main( int argc, char *argv[] ) {

    int ret = 0;

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

    /** Call the power monitoring function */
    powerMonitor();

    return 0;
}
