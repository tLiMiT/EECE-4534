/**
 *@file power_monitor.c
 *
 *@brief
 *  - Reading current monitor lab on the TLL6537 platform
 *  - Configure I2C controller to communicate with current monitor chip 
 *  - Read out voltage/current data
 * 
 * 1. Configure I2C controller as master
 * 2. Probe current monitor to check if it is present
 * 3. Read voltage/current data from its register
 *
 * Target:   TLL6527v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 *@author    Rohan Kangralkar, ECE, Northeastern University
 *@date      01/16/2010
 *
 * LastChange:
 * $Id: power_monitor.c 807 2013-02-21 23:46:19Z ovaskevi $
 *
 *******************************************************************************/

#include <stdio.h>
#include <bf52xI2cMaster.h>
#include "adm1192.h"
#include "power_monitor.h"
#include "startup.h"
#include "profile_app.h"
#include <extio.h>
#include <isrDisp.h>
#include <tll_common.h>

/**
 * @def POWER_SAMPLES
 * @brief Total number of power samples to be taken
 */
#define                     POWER_SAMPLES       (30)

/******************************************************************************
 *                     STATIC GLOBALS
 *****************************************************************************/
// note use static so that the global is only visible within this file 
/**
 * @var  global_power
 * @brief Global storage to capture power measurements
 */
static int  global_power[256];
static int  global_power_counter = 0;
static isrDisp_t isrDisp;


/** power_monitor_config.
 *
 * This function configures the power monitor slave devices via I2C.
 *
 * Pre-conditions:
 *  - initialized I2C interface
 *
 * Post condtions:
 
 *
 * Parameters:
 *
 *
 * @return  0 on success, -1 on fail 
 */
int powerMonitor_config(void)
{
    unsigned char               buff[I2C_MSG_SIZE_MAX];
    bf52x_i2c_cmd_data_t		i2c_trans;
    int                         status      = FAIL;
    int i;
    
    /* Prepare command  to send to power monitor*/
    for (i = 0; i < I2C_MSG_SIZE_MAX; i++)
        	buff[i] = 0;
    buff[0]                    = 0b0010101;
    i2c_trans.addr             = 0b0101110;
    i2c_trans.data             = buff;
    i2c_trans.datalen          = 1;

    printf("i2c_trans.addr - %x\n", i2c_trans.addr);
    printf("i2c_trans.data - %x\n", *i2c_trans.data);
	printf("i2c_trans.datalen - %x\n", i2c_trans.datalen);

    /* send data and return success value */
    status = bf52xI2cMaster_send(&i2c_trans);
    printf("Status - %d\n", status);
    return status;
}



/** power_monitor_read
 *
 * This function reads values from the configured 
 * power monitor slave devices via I2C.
 *
 * Pre-conditions:
 *  - The power monitor should be configured properly .
 *
 * Post conditions:
 *  - data is read to the buffer which is then converted to 
 *     current and voltage codes.
 *
 * Parameters:
 * @param pPower - integer power in mW
 *
 *
 * @return 
 */
int powerMonitor_read(unsigned int *pPower)
{
    unsigned char               buff[I2C_MSG_SIZE_MAX];
    bf52x_i2c_cmd_data_t		i2c_trans;
    int 						retVal				= 0;
    int 						current_code		= 0;
    int			 				voltage_code		= 0;
    float 						current				= 0.00;
    float						voltage				= 0.00;
    int i;

    /* Prepare command */
    for (i = 0; i < I2C_MSG_SIZE_MAX; i++)
    	buff[i] = 0;
    i2c_trans.addr             = 0b0101110;
    i2c_trans.data             = buff;
    i2c_trans.datalen          = 3;
    
    /* send data */ 
    retVal = bf52xI2cMaster_receive(&i2c_trans);

    /* evaluate data */
    voltage_code = (buff[0] << 4) | ((buff[2] & 0xF0) >> 4);
    current_code = (buff[1] << 4) | (buff[2] & 0xF);

    printf("Voltage code: %d\n", voltage_code);
    printf("Current code: %d\n", current_code);

    voltage = 1000.0 * 6.65 * (voltage_code / 4096.0) / 1000; 	// Volts
    current = (105.84 / 4096 * current_code) / 0.05 / 1000;		// Amps

    printf("Voltage: %f\n", voltage);
    printf("Current: %f\n\n", current);

    *pPower = current * voltage * 1000;	// mWatts

    return retVal; // return FAIL if data could not be read
}

/** powerMonitor_record
 * @brief    record the current power consumption into an array
 *    uses global variables  global_power, global_power_counter, We save the
 *    values to the global array to get an accurate results over continuous time without delays
 *
  *
 * Pre-conditions:
 *  - The power monitor should be configured properly .
 *
 * Post condtions:
 *
 * Parameters:
 *
 *
 * @return 
 */
 
void powerMonitor_record(void) {
	unsigned int power;
	/* collect data */
    if(global_power_counter < POWER_SAMPLES){
        if(powerMonitor_read(&power) == PASS) {
	        global_power[global_power_counter] = power; /* Save the read value to the global array */
    	    global_power_counter++;
        }
    }
}

/** powerMonitor_print
 *    print recorded power consumption [mW]
 *
 * Pre-conditions:
 *  - The power monitor should be configured properly .
 *
 * Post condtions:
 *
 * Parameters:
 *
 *
 * @return 
 */
 void powerMonitor_print(void) {
 	int  counter	= 0;
    for(counter=0; counter<global_power_counter; counter++) {
        printf("Power= %d\r\n", global_power[counter]);
    }
}

/** powerMonitor_extioISR
 *  @brief The function is used to interrupt the processor from sleep state and 
 *   then read the power drawn by the processor.
 */
 void powerMonitor_extioISR(void *_pArg)
 {
     unsigned int       led     = 0;
     
     /*
      * Read the extio and toggle the led
      */
     led = (unsigned int)_pArg;
     
     /*
      * Check if the read value is valid and within the number of leds we have on EXT1
      */
     if ( EXTO_LED_INVALID > led ) {
        extio_ledToggle((extio_output)led);
     }     
 }


/** powerMonitor_init
 * This function Initializes the i2c module, extio module and the 
 * ISR dispatcher
 *
 * Pre-conditions:
 *  - None.
 *
 * Post condtions:
 *  - None.
 *
 * Parameters:
 *
 * @return Zero on success.
 * Negative value on failure.
 */
 int powerMonitor_init(void)
 {
    int                         status                  = 0;
    
     /* configure TWI interface for I2C operation and set the clock*/
    bf52xI2cMaster_init(0, ADM1192_I2C_CLK);
       
    /* Initialize the ISR dispatcher */
    status = isrDisp_init(&isrDisp);
    if ( PASS != status ) {
        return status;
    }
    
    /* Initialize the extio module */
    status = extio_init(&isrDisp);
    if ( PASS != status ) {
        return status;
    }    
   
    /**
     *Register a callback when the switch 0 goes high. The argument passed to the ISR is led0
     */
    status = extio_callbackRegister(EXTIO_SW0_HIGH, powerMonitor_extioISR, (void *)EXTIO_LED0 );
    if ( PASS != status) {
        return status;
    }
    /**
     *Register a callback when the switch 1 goes high. The argument passed to the ISR is led1
     */
    status = extio_callbackRegister(EXTIO_SW1_HIGH, powerMonitor_extioISR, (void *)EXTIO_LED1 );
    if ( PASS != status) {
        return status;
    }
    /**
     *Register a callback when the switch 2 goes high. The argument passed to the ISR is led2
     */
    status = extio_callbackRegister(EXTIO_SW2_HIGH, powerMonitor_extioISR, (void *)EXTIO_LED2);
    if ( PASS != status) {
        return status;
    }
    /**
     *Register a callback when the switch 3 goes high. The argument passed to the ISR is led3
     */
    status = extio_callbackRegister(EXTIO_SW3_HIGH, powerMonitor_extioISR, (void *)EXTIO_LED3);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     *Register a callback when the pushbutton 0 goes high. The argument passed to the ISR is led4
     */
    status = extio_callbackRegister(EXTIO_PB0_LOW, powerMonitor_extioISR, (void *)EXTIO_LED4);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     *Register a callback when the pushbutton 1 goes high. The argument passed to the ISR is led5
     */
    status = extio_callbackRegister(EXTIO_SW1_LOW, powerMonitor_extioISR, (void *)EXTIO_LED5);
    if ( PASS != status) {
        return status;
    }
    /**
     *Register a callback when the pushbutton 2 goes high. The argument passed to the ISR is led6
     */
    status = extio_callbackRegister(EXTIO_SW2_LOW, powerMonitor_extioISR, (void *)EXTIO_LED6);
    if ( PASS != status) {
        return status;
    }
    /**
     *Register a callback when the pushbutton 3 goes high. The argument passed to the ISR is led7
     */
    status = extio_callbackRegister(EXTIO_SW3_LOW, powerMonitor_extioISR, (void *)EXTIO_LED7);
    if ( PASS != status) {
        return status;
    }    
    
    return status;
 }



/** powerMonitor
 * This function enables the continuous voltage and current readings.
 * Also displays the current and voltage readings by reading it from
 * the adm1192 over the i2c lines.
 *
 * Pre-conditions:
 *  - None.
 *
 * Post condtions:
 *  - None.
 *
 * Parameters:
 *
 * @return Zero on success.
 * Negative value on failure.
 */
void powerMonitor(void)
{
    /** variables to store and calculate received data */
    unsigned int                power                   = 0;
    
    /* initialize the power module */   
    powerMonitor_init();
      
    /* configure the power monitor */
    powerMonitor_config();

    /* read the power monitor */
    powerMonitor_read(&power);


    /* enable to execute test application */
    if(1) {
        profile_demo();
        powerMonitor_print();
    }
}
