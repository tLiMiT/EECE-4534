/**
 *@file power_monitor.h
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
 * @author  Enqiang Sun, ECE, Northeastern University
 * @date 	01/23/09
 *
 * LastChange:
 * $Id: power_monitor.h 721 2011-12-11 16:52:42Z schirner $
 *
 *******************************************************************************/

 
 
/* Referenced documentation:
 * [1]  "ADSP-BF52x Blackfin Processor Hardware Reference (Volume 1 of 2)",
 *      Revision 0.31 (Preliminary), May 2008, Part Number 82-000525-03
 *
 */
 
 
// protecting against multiple times including the same header file
#ifndef _POWER_MONITOR_H_
#define _POWER_MONITOR_H_


// set default platform version if not already set
#ifndef PLATFORM
#error "The code is specific to TLL6527M. Please check the project properties"
#endif

// definitions below are platform specific 
#if PLATFORM == __TLL6527__

/** power_monitor_config.
 *
 * This function configures the power monitor slave devices via I2C.
 *
 * Pre-conditions:
 *  - The command to be sent to the slave device.
 *
 * Post condtions:
 
 *
 * Parameters:
 *
 *
 * @return  0 on success, -1 on fail 
 */
int powerMonitor_config(void);


/** powerMonitor_read
 *
 * This function reads values from the configured 
 * power monitor slave devices via I2C.
 *
 * Pre-conditions:
 *  - The power monitor should be configured properly .
 *
 * Post condtions:
 *  - data is read to the buffer which is then converted to 
 *     current and voltage codes.
 *
 * Parameters:
 * @param pPower - interger power in mW
 *
 *
 * @return 
 */
int powerMonitor_read(unsigned int *pPower);

/** powerMonitor_record
 *    record the current power consumption into an array 
 *    uses global variables     
 *      global_power, global_power_counter
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
void powerMonitor_record(void);

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
 void powerMonitor_print(void);

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
int powerMonitor_init(void);
/** powerMonitor
 * This function is the core that initializes and executes the power monitor
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
void powerMonitor(void);

#endif
#endif
