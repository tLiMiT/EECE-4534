/**
 *@file audioRx.c
 *
 *@brief
 *  - receive audio samples from DMA
 *
 * Target:   TLL6537v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:    Rohan Kangralkar
 * @date 03/15/2009
 *
 * LastChange:
 * $Id$
 *
 *******************************************************************************/
#ifndef _AUDIO_RX_H_
#define _AUDIO_RX_H_

#include "queue.h"
#include "bufferPool.h"
#include "isrDisp.h"
#include <audioSample.h>

/***************************************************
            DEFINES
***************************************************/   
/**
 * @def AUDIORX_QUEUE_DEPTH
 * @brief rx queue depth
 */
#define AUDIORX_QUEUE_DEPTH 7


/***************************************************
            DATA TYPES
***************************************************/

/** audio RX object
 */
typedef struct {
  queue_t        queue;  /* queue for received buffers */
  chunk_t        *pPending; /* pointer to pending chunk just in receiving */
  bufferPool_t   *pBuffP; /* pointer to buffer pool */
  FILE              *audioRx_pFile; /* audio file */
  audioSample_t  audioSample;
} audioRx_t;


/***************************************************
            Access Methods 
***************************************************/



/** Initialize audio rx
 *    - get pointer to buffer pool
 *    - register interrupt handler
 *    - initialize RX queue

 * Parameters:
 * @param pThis  pointer to own object
 * @param pBuffP  pointer to buffer pool to take and return chunks from
 * @param pIsrDisp   pointer to interrupt dispatcher to get ISR registered
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRx_init(audioRx_t *pThis, bufferPool_t *pBuffP, 
                 isrDisp_t *pIsrDisp);

/** start audio rx
 *    - start receiving first chunk from DMA
 *      - acqurie chunk from pool 
 *      - config DMA 
 *      - start DMA + SPORT
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRx_start(audioRx_t *pThis);


/** audio rx isr  (to be called from dispatcher) 

 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return None 
 */
void audioRx_isr(void *pThis);

/** audio rx get 
 *   copyies a filled chunk into pChunk
 *   blocking call, blocks if queue is empty 
 *     - get from queue 
 *     - copy in to pChunk
 *     - release chunk to buffer pool 
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRx_get(audioRx_t *pThis, chunk_t **pChunk);


#endif
