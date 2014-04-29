/**
 *@file audioTx.c
 *
 *@brief
 *  - receive audio samples from DMA
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: audioTx.c 551 2011-03-09 15:23:56Z adipasqu $
 *
 *******************************************************************************/
#include "tll_common.h"
#include "audioTx.h"
#include "bufferPool.h"
#include "isrDisp.h"
#include <tll_config.h>
#include <tll_sport.h>
#include <queue.h>
#include <power_mode.h>


/** 
 * Configures the DMA tx with the buffer and the buffer length to 
 * transfer
 * Parameters:
 * @param pchunk  pointer to tx chunk
 * @return void
 */
void audioTx_dmaConfig(chunk_t *pchunk)
{
    DISABLE_DMA(*pDMA4_CONFIG);
    *pDMA4_START_ADDR   = &pchunk->u16_buff[0];
    *pDMA4_Y_COUNT		= pchunk->len/2;  // 16 bit data so we change the stride and count
    *pDMA4_X_COUNT      = 2;
    *pDMA4_Y_MODIFY		= 2;
    *pDMA4_X_MODIFY     = 0;
    ENABLE_DMA(*pDMA4_CONFIG);
}


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
                 isrDisp_t *pIsrDisp)
{
    // paramter checking
    if ( NULL == pThis || NULL == pBuffP || NULL == pIsrDisp) {
        printf("[ATX]: Failed init\n");
        return FAIL;
    }
    
    // store pointer to buffer pool for later access     
    pThis->pBuffP       = pBuffP;

    pThis->pPending     = NULL; // nothing pending
    pThis->running      = 0;    // DMA turned off by default
    
    // init queue 
    queue_init(&pThis->queue, AUDIOTX_QUEUE_DEPTH);   
 
    /* Configure the DMA4 for TX (data transfer/memory read) */
    /* Read, 1-D, interrupt enabled, 16 bit transfer, Auto buffer */
    *pDMA4_CONFIG = WDSIZE_16 | DI_EN | DMA2D;
    
    // register own ISR to the ISR dispatcher
    isrDisp_registerCallback(pIsrDisp, ISR_DMA4_SPORT0_TX, audioTx_isr, pThis);
    
    printf("[ARX]: TX init complete\n");
    
    return PASS;
}



/** start audio tx
 *   - empthy for now
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioTx_start(audioTx_t *pThis)
{
     
    printf("[AUDIO TX]: audioTx_start: implemented\n");    

    // empty nothing to be done, DMA kicked off during run time 
    return PASS;   
}



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
void audioTx_isr(void *pThisArg)
{
    // create local casted pThis to avoid casting on every single access
    audioTx_t  *pThis = (audioTx_t*) pThisArg;

    chunk_t                  *pchunk              = NULL;
    
    // validate that TX DMA IRQ was triggered 
    if ( *pDMA4_IRQ_STATUS & 0x1  ) {
        //printf("[TXISR]\n");
        /* Remove the  data from the queue and create space for more data
           The data was read previously by the DMA
         */
         /* fist attempt to get new chunk */
        if(PASS == queue_get(&pThis->queue, (void **)&pchunk) ) {
                /* release old chunk on success */
               bufferPool_release(pThis->pBuffP, pThis->pPending);
               /* register new chunk as pending */
               pThis->pPending = pchunk;
        } else {
            printf("TX Q Emtpy\n");           
        }
        *pDMA4_IRQ_STATUS  |= 0x0001;     // Clear the interrupt
        
        // config DMA either with new chunk (if there was one), or with old chunk on empty Q
        audioTx_dmaConfig(pThis->pPending);        
    }
}




/** audio rx put
 *   copyies filled pChunk into the TX queue for transmission
 *    if queue is full, then chunk is dropped 
 * Parameters:
 * @param pThis  pointer to own object
 * @param pChunk Pointer to chunk
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioTx_put(audioTx_t *pThis, chunk_t *pChunk)
{
    chunk_t                  *pchunk_temp         = NULL;
    int                         count                   = 0;
    
    if ( NULL == pThis || NULL == pChunk ) {
        printf("[TX]: Failed to put\n");
        return FAIL;
    }
    
    // block if queue is full
    while(queue_is_full(&pThis->queue) ) {
        printf("[TX]: Queue Full\n");
        powerMode_change(PWR_ACTIVE);
        asm("idle;");
    }
    powerMode_change(PWR_FULL_ON);
    
    // get free chunk from pool 
    if ( PASS == bufferPool_acquire(pThis->pBuffP, &pchunk_temp) ) {
        // copy chunk into free buffer for queue 
        //   (manually since memcpy is not working)
        chunk_copy(pChunk, pchunk_temp);
        
        /* If DMA not running ? */
        if ( 0 == pThis->running ) {
            /* directly put chunk to DMA transfer & enable */
            pThis->running  = 1;
            pThis->pPending = pchunk_temp;
            audioTx_dmaConfig(pThis->pPending);  
            ENABLE_SPORT0_TX();  
        } else { 
            /* DMA already running add chunk to queue */
            if ( PASS != queue_put(&pThis->queue, pchunk_temp) ) {
                
                // return chunk to pool if queue is full, effectivly dropping the chunk 
                bufferPool_release(pThis->pBuffP, pchunk_temp);
                return FAIL;
            }
        }
    } else {
        // drop if we dont get free space 
        printf("[TX] failed to get buffer\n");
    }
    
    return PASS;
}

