 /**
 *@file profile_app.c
 *
 *@brief
 *  - test application for profiling of power consumption  	
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Dr Gunar Schirner
 * @date 	02/10/2010
 *
 * LastChange:
 * $Id: profile_app.c 779 2012-02-04 00:25:28Z rkangralkar $
 *
 *******************************************************************************/

#include <tll_config.h>
#include <power_monitor.h>
#include <power_mode.h>
#include <sys/exception.h>
#include <stdio.h>




/******************************************************************************
 *                     DEFINES
 *****************************************************************************/
/**
 *  @def APP_ITERATIONS
 *  @brief The Number of iteration the application must run.
 */
#define APP_ITERATIONS   10


/******************************************************************************
 *                     STATIC GLOBALS
 *****************************************************************************/
volatile int 		dataVariable = 0;
static unsigned int gOtherIntCntr = 0;
static unsigned int gGpioIntCntr = 0;


/** 
 *
 * Small test application for power profiling
 *
 * Pre-conditions:
 *  - None.
 *
 * Post condtions:
 *  - None.
 *
 * Parameters:
 *
 *
 * @return void
 */
void profile_demo(void){
	int i, j =0, k;

	// 1) FULL ON ----------------------------------------------
	for(k=0;k<APP_ITERATIONS;k++){
		powerMonitor_record();
		for (i=0;i<1000;i++){
			// perform some floating operations  
			dataVariable = i * 0.3 * (float) dataVariable;
			// some integer operations 
			j = (i *3 +5) / (j - 2);
		}
	}
	
	// 2) ACTIVE ----------------------------------------------
    /** change the processor mode from FULL_ON to ACTIVE*/
    powerMode_change(PWR_ACTIVE);

    for(k=0;k<APP_ITERATIONS;k++){
		powerMonitor_record();
		for (i=0;i<1000;i++){
			// perform some floating operations  
			dataVariable = i * 0.3 * (float) dataVariable;
			// some integer operations 
			j = (i *3 +5) / (j - 2);
		}
	}

#if 1
	// 3) SLEEP (and react to interrupts)-----------------------------------------
	gGpioIntCntr=0;
    while(gGpioIntCntr < APP_ITERATIONS){
    	 /* change power to sleep, need to push a button to wake up 
    	    processor again */  
	     powerMode_change(PWR_SLEEP);
	     powerMonitor_record();
	     gGpioIntCntr++;
    }
#endif
    powerMode_change(PWR_FULL_ON);

    printf("Int Total = %d, Int GPIO = %d\n", gOtherIntCntr, gGpioIntCntr);
}

