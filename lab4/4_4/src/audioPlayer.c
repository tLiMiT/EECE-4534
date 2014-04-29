/**
 *@file audioPlayer.c
 *
 *@brief
 *  - core module for audio player
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:  Gunar Schirner
 *           Rohan Kangralkar
 * @date	03/08/2010
 *
 * LastChange:
 * $Id: audioPlayer.c 846 2014-02-27 15:35:54Z fengshen $
 *
 *******************************************************************************/

#include <tll_common.h>
#include "audioPlayer.h"
#include <bf52xI2cMaster.h>
#include "ssm2602.h"
#include <isrDisp.h>
#include <extio.h>
#include <tll6527_core_timer.h>

/**
 * @def I2C_CLK
 * @brief Configure I2C clock to run at 100KHz
  */
#define I2C_CLOCK   (100*_1KHZ)
/**
 * @def VOLUME_CHANGE_STEP
 * @brief Magnitude of change in the volume when increasing or decreasing
 */
#define VOLUME_CHANGE_STEP (4)
/**
 * @def VOLUME_MAX
 * @brief MAX volume possible is +6db refer to ssm2603 manual
 */
#define VOLUME_MAX (0x7F)
/**
 * @def VOLUME_MIN
 * @brief MIN volume possible is -73db refer to ssm2603 manual
 */
#define VOLUME_MIN (0x2F)


/** initialize audio player 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioPlayer_init(audioPlayer_t *pThis)
{
    int                         status                  = 0;
    
    printf("[AP]: Init start\r\n");
    
    pThis->volume 		= VOLUME_MIN; /*default volume */
    pThis->frequency 	= SSM2602_SR_8000/2; /* default frequency, need not copy to Left and right channel*/

    /* Initialize the core timer */
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
        printf("SSM2602 init failed\r\n");
        return status;
    }
    
    /* Initialize the buffer pool */
    status = bufferPool_init(&pThis->bp);
    if ( PASS != status ) {
        return FAIL;
    }
    
    /* Initialize the extio */
    status = extio_init(&pThis->isrDisp);
    if ( PASS != status ) {
        return FAIL;
    }

    /* Initialize the audio RX module*/
    status = audioRx_init(&pThis->rx, &pThis->bp, &pThis->isrDisp) ;
    if ( PASS != status) {
        return FAIL;
    }

    /* Initialize the audio TX module */
    status = audioTx_init(&pThis->tx, &pThis->bp, &pThis->isrDisp);
    if ( PASS != status ) {
        return FAIL;
    }   
    
    printf("[AP]: Init complete\r\n");

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
    
    printf("[AP]: startup \r\n");
    
    /* Start the audio RX module */
    status = audioRx_start(&pThis->rx);
    if ( PASS != status) {
        return FAIL;
    }

    /* Start the audio TX module */
    status = audioTx_start(&pThis->tx);
	if ( PASS != status) {
        return FAIL;
    }
    
    return PASS;
}



/** main loop of audio player does not terminate
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
void audioPlayer_run (audioPlayer_t *pThis) {
    
	int                      status = FAIL;
    
    while(1) {

    	/** get audio chunk */
        status = audioRx_get(&pThis->rx, &pThis->chunk);

        /** If we have chunks that can be played then we provide them
         * to the audio TX
         */
        if ( PASS == status ) {
          /** play audio chunk through speakers */
          audioTx_put(&pThis->tx, pThis->chunk);
        }
    }
}


