/**
 *@file audioTx.c
 *
 *@brief
 *  - transmission of audio
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
#ifndef _AUDIO_TX_H_
#define _AUDIO_TX_H_

#include "queue.h"
#include "bufferPool.h"
#include "isrDisp.h"

/***************************************************
            DEFINES
***************************************************/   
/**
 * @def AUDIOTX_QUEUE_DEPTH
 * @brief tx queue depth
 */
#define AUDIOTX_QUEUE_DEPTH 7

/***************************************************
            DATA TYPES
***************************************************/

/** audio RX object
 */
typedef struct {
  queue_t       queue;  /* queue for received buffers */
  chunk_t       *pPending; /* pointer to pending chunk just in receiving */
  bufferPool_t  *pBuffP; /* pointer to buffer pool */
  int              running; /* DMA is Running */
} audioTx_t;


/***************************************************
            Access Methods 
***************************************************/

/** Initialize audio tx
 *    - get pointer to buffer pool
 *    - register interrupt handler
 *    - initialize TX queue

 * Parameters:
 * @param pThis  pointer to own object
 * @param pBuffP  pointer to buffer pool to take and return chunks from
 * @param pIsrDisp   pointer to interrupt dispatcher to get ISR registered
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioTx_init(audioTx_t *pThis, bufferPool_t *pBuffP, 
                 isrDisp_t *pIsrDisp);

/** start audio tx
 *   - empthy for now
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioTx_start(audioTx_t *pThis);


/** audio rtx isr  (to be called from dispatcher) 
 *   - get chunk from tx queue
 *    - if valid, release old pending chunk to buffer pool 
 *    - configure DMA 
 *    - if not valide, configure DMA to replay same chunk again
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return None 
 */
void audioTx_isr(void *pThis);

/** audio rx put
 *   copyies filled pChunk into the TX queue for transmission
 *    if queue is full, then chunk is dropped 
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioTx_put(audioTx_t *pThis, chunk_t *pChunk);


#endif
