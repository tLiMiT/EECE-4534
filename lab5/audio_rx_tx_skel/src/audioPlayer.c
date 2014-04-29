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
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: audioPlayer.c 545 2011-03-07 19:24:57Z adipasqu $
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
 * @brief Configure I2C clock to run at 400KHz
  */
#define I2C_CLOCK   (400*_1KHZ)


/** initialize audio player 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioPlayer_init(audioPlayer_t *pThis)
{
    int                         status                  = 0;
    
    printf("[AP]: Init start\n");
    
    /** Initialize the core timer that can be used to create delays */
    coreTimer_init();
    
    /* configure TWI interface for I2C operation */
    bf52xI2cMaster_init(0, I2C_CLOCK);
    
    /**
     * Initialize the IsrDispatcher that is used for dispatching the interrupts
     */
    status = isrDisp_init(&pThis->isrDisp);
    if ( PASS != status ) {
        return FAIL;
    }

    /* Initialize the SSM2602 over the I2C interface */
    /* Initialize the SSM2602 to playback audio data */
    /* Initialize sport0 to receive audio data */
    status = ssm2602_init(&pThis->isrDisp, 0x27, SSM2602_SR_16000, SSM2602_RX|SSM2602_TX);
    if (PASS != status) {
        printf("SSM2602 init failed\n");
        return status;
    }
    
    /**
     * Initialize the buffer Pool
     */
    status = bufferPool_init(&pThis->bp);
    if ( PASS != status ) {
        return FAIL;
    }
    /**
     * Initialize the extio Module
     */
    status = extio_init(&pThis->isrDisp);
    if ( PASS != status ) {
        return FAIL;
    }

    /**
     * Initialize the audioRX module
     */
    status = audioRx_init(&pThis->rx, &pThis->bp, &pThis->isrDisp) ;
    if ( PASS != status) {
        return FAIL;
    }

    /**
     * Initialize the audio TX module
     */
    status = audioTx_init(&pThis->tx, &pThis->bp, &pThis->isrDisp);
    if ( PASS != status ) {
        return FAIL;
    }   

    /**
     * Subscribe to the extio module for switch 0 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_SW0_HIGH);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     * Subscribe to the extio module for switch 1 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_SW1_HIGH);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     * Subscribe to the extio module for switch 2 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_SW2_HIGH);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     * Subscribe to the extio module for switch 3 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_SW3_HIGH);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     * Subscribe to the extio module for pushbutton 0 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_PB0_LOW);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     * Subscribe to the extio module for pushbutton 1 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_PB1_LOW);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     * Subscribe to the extio module for pushbutton 2 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_PB2_LOW);
    if ( PASS != status) {
        return FAIL;
    }
    /**
     * Subscribe to the extio module for pushbutton 3 high event. The event will be
     * parsed in get event
     */
    status = extio_eventSubscribe(EXTIO_PB3_LOW);
    if ( PASS != status) {
        return FAIL;
    }
    
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
    
    status = audioRx_start(&pThis->rx);
    if ( PASS != status) {
        return FAIL;
    }

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
void audioPlayer_run(audioPlayer_t *pThis)
{
    chunk_t                  chunk;
    int                         start_tx                = 0;    
    int                         status                  = FAIL;
    int                         filterMask              = 0;
    extio_input                 event;
    
    // init local chunk    
    chunk_init(&chunk);
    
    while(1) {
    	/** get audio chunk */
        audioRx_get(&pThis->rx, &chunk);
        /** play audio chunk through speakers */
        audioTx_put(&pThis->tx, &chunk);
    }
}


