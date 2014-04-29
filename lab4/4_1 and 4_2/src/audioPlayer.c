/**
 *@file audioPlayer.c
 *
 *@brief
 *  - core module for audio player
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 * @date	02/15/2010
 *
 * LastChange:
 * $Id: audioPlayer.c 809 2013-02-25 01:58:38Z ovaskevi $
 *
 *******************************************************************************/

#include <tll_common.h>
#include "audioPlayer.h"
#include <bf52xI2cMaster.h>
#include "ssm2602.h"
#include <isrDisp.h>
#include <extio.h>
#include <snd_sample.h>
#include <tll6527_core_timer.h>
#include <tll_sport.h>

/**
 * @def I2C_CLK
 * @brief Configure I2C clock to run at 400KHz
  */
#define I2C_CLOCK   (400*_1KHZ)
/**
 * @def VOLUME_CHANGE_STEP
 * @brief Magnitude of change in the volume when increasing or decreasing
 */
#define VOLUME_CHANGE_STEP (4)
/**
 * @def VOLUME_MAX
 * @brief MAX volume possible is +6db refer to ssm2603 manual
 */
#define VOLUME_MAX (0x2F)
/**
 * @def VOLUME_MIN
 * @brief MIN volume possible is -73db refer to ssm2603 manual
 */
#define VOLUME_MIN (0x7F)

int current_volume = 0;

/**********************************************************************
*              Private functions
**********************************************************************/
/** audioPlayer_volumeIncrease.
 *
 * @brief This function configures the volume on the codec using i2c. This
 * function calls i2c methods to increase the volume of the codec
 *
 * Pre-conditions:
 *  - initialized I2C interface
 *
 * Post condtions:
 * Increase volume
 *
 * Parameters:
 * @param _pArg  Contains the audioPlayer object.
 *
 */
void audioPlayer_volumeIncrease(void *_pArg)
{
    /* Insert your code here */
	current_volume += VOLUME_CHANGE_STEP;
	ssm2602_setVolume(SSM2602_MAIN_OUT, current_volume,current_volume);
	//{ unsigned int n; for ( n=0; n < 5000000; n++ ) { asm("nop; ssync;"); } }
}

/** audioPlayer_volumeDecrease.
 *
 * @brief This function configures the volume on the codec using i2c. This
 * function calls i2c methods decrease the volume of the codec
 *
 * Pre-conditions:
 *  - initialized I2C interface
 *
 * Post condtions:
 * Decrease volume
 *
 * Parameters:
 * @param _pArg  Contains the audioPlayer object.
 *
 */
void audioPlayer_volumeDecrease(void *_pArg)
{
    /* Insert your code here */
	current_volume -= VOLUME_CHANGE_STEP;
	ssm2602_setVolume(SSM2602_MAIN_OUT, current_volume,current_volume);
	//{ unsigned int n; for ( n=0; n < 5000000; n++ ) { asm("nop; ssync;"); } }
}


/** audioPlayer_freqIncrease.
 *
 * @brief This function configures the frequency on the codec using i2c. This
 * function calls i2c methods to change the frequency of the audio codec
 *
 * Pre-conditions:
 *  - initialized I2C interface
 *
 * Post condtions:
 * Increase frequency
 *
 * Parameters:
 * @param _pArg  Contains the audioPlayer object.
 *
 */
void audioPlayer_freqIncrease(void *_pArg)
{
    /* Insert your code here */
}

/** audioPlayer_freqDecrease.
 *
 * @brief This function configures the frequency on the codec using i2c. This
 * function calls i2c methods to change the frequency of the audio codec
 *
 * Pre-conditions:
 *  - initialized I2C interface
 *
 * Post condtions:
 * Decrease frequency
 *
 * Parameters:
 * @param _pArg  Contains the audioPlayer object.
 *
 */
void audioPlayer_freqDecrease(void *_pArg)
{
    /* Insert your code here */
}



/** initialize audio player 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioPlayer_init(audioPlayer_t *pThis)
{
    int                         status                  = 0;
    
    printf("[AP]: Init start\n");
    
    pThis->volume 		= VOLUME_MIN; /*default volume */
    current_volume = pThis->volume;
    pThis->frequency 	= SSM2602_SR_16000; /* default frequency */

    /* Initialize the core internal timer for generating delays */
    coreTimer_init();
 
    /* configure TWI interface for I2C operation */
    bf52xI2cMaster_init(0, I2C_CLOCK);
    
    /* Initialize the interrupt Dispatcher */
    status = isrDisp_init(&pThis->isrDisp);
    if ( PASS != status ) {
        return FAIL;
    }
    
    
    /* Initialize the SSM2602 over the I2C interface */
    /* Initialize the SSM2602 to playback audio data */
    /* Initialize sport0 to receive audio data */
    status = ssm2602_init(&pThis->isrDisp, pThis->volume, pThis->frequency, SSM2602_TX);
    if (PASS != status) {
        printf("SSM2602 init failed\n");
        return status;
    }
    
    /* Initialize the extio module */
    status = extio_init(&pThis->isrDisp);
    if ( PASS != status ) {
        return FAIL;
    }
    
    /* volume decrease function will be called when the pushbutton 0 go high, the audioPlayer object can be sent as an argument */
    status = extio_callbackRegister(EXTIO_PB0_HIGH , audioPlayer_volumeDecrease, &pThis);
    if ( PASS != status) {
        return FAIL;
    }
    /* frequency increase function will be called when the pushbutton 1 go high, the audioPlayer object is sent as an argument */
    
    /* volume increase function will be called when the pushbutton 2 go high, the audioPlayer object is sent as an argument */
    status = extio_callbackRegister(EXTIO_PB1_HIGH , audioPlayer_volumeIncrease, &pThis);
	if ( PASS != status) {
		return FAIL;
	}
    /* frequency decrease function will be called when the pushbutton 3 go high, the audioPlayer object is sent as an argument */
    

    printf("[AP]: Init complete\n");

    return PASS;
}




/** startup phase after initialization 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioPlayer_start(audioPlayer_t *pThis)
{
    int                         status                  = 0;
    
    printf("[AP]: startup \n"); 
    
    /* Enable the SPORT0 TX */
    ENABLE_SPORT0_TX();
 
    return PASS;
}



/** main loop of audio player does not terminate
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
void audioPlayer_run(audioPlayer_t *pThis)
{
    int                         status                  = FAIL;
    short                       data                    = 0;
    int                         count                   = 0;
    int							realcount				= 0;
    unsigned short *pSPORT0_TX_FIFO = SPORT0_TX;

    printf("[AP]: run \n");

    while(1) {
    	data = 0;

        /** get audio from the snd_sample.h header */
    	data |= (snd_samples[count+1] << 8);
    	data |= snd_samples[count];

    	//while full
    	while(*pSPORT0_STAT & (1 << 3))
    	{
    		//printf("FIFO Full\n");
    	}

    	//not full
    	*pSPORT0_TX_FIFO = data;
    	//printf("Count: %d, Data %04x\n", count, data);

    	//increment count if realcount is 2 (mono to stereo)
    	realcount++;
    	if(!(realcount % 2))
    		count+=2;

    	if(count > snd_samples_nBytes)
    		count = 0;

    	//printf("Count: %d\n", realcount);
    }
}


