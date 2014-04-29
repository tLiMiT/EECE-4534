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
 * @date 03/15/2009
 *
 * LastChange:
 * $Id: audioTx.c 813 2013-03-12 03:06:44Z ovaskevi $
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
	/* 1. Disable DMA 4*/
	*pDMA4_CONFIG &= ~DMAEN;

	/* 2. Configure start address */
	*pDMA4_START_ADDR = pchunk->u16_buff;

	/* 3. set X count */
	*pDMA4_X_COUNT = pchunk->bytesUsed/2;
   
	/* 4. set X modify */
	*pDMA4_X_MODIFY = 2;
   
	/* 5. Re-enable DMA */
	*pDMA4_CONFIG |= DMAEN;

	printf("DMA Config Complete\n");

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
        printf("[ATX]: Failed init\r\n");
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
    *pDMA4_CONFIG = WDSIZE_16 | DI_EN | DI_EN; /* 16 bit amd DMA enable */
    
    // register own ISR to the ISR dispatcher
    isrDisp_registerCallback(pIsrDisp, ISR_DMA4_SPORT0_TX, audioTx_isr, pThis);
    
    printf("[ARX]: TX init complete\r\n");
    
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
     
    printf("[AUDIO TX]: audioTx_start: implemented\r\n");

    // empty nothing to be done, DMA kicked off during run time 
    return PASS;   
}



/** audio tx isr  (to be called from dispatcher) 
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
        printf("[TXISR]\n");
        /* We need to remove the data from the queue and create space for more data
           (The data was read previously by the DMA)

        1. First, attempt to get the new chunk, and check if it's available: */
    	if (queue_get(&pThis->queue, &pchunk) == PASS) {
    		/* 2. If so, release old chunk on success back to buffer pool */
    		bufferPool_release(pThis->pBuffP, pThis->pPending);


        /* 3. Register the new chunk as pending */
    		pThis->pPending = pchunk;
    	}
       
        *pDMA4_IRQ_STATUS  |= 0x0001;     // Clear the interrupt
        
        // config DMA either with new chunk (if there was one), or with old chunk on empty Q
        audioTx_dmaConfig(pThis->pPending);
    }
}




/** audio tx put
 *   copyies filled pChunk into the TX queue for transmission
 *    if queue is full, then chunk is dropped 
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioTx_put(audioTx_t *pThis, chunk_t *pChunk)
{
    int                         count                   = 0;
    
    if ( NULL == pThis || NULL == pChunk ) {
        printf("[TX]: Failed to put\r\n");
        return FAIL;
    }
    
    // block if queue is full
    while(queue_is_full(&pThis->queue)) {
        //printf("[TX]: Queue Full\r\n");
        powerMode_change(PWR_ACTIVE);
        asm("idle;");
    }
    powerMode_change(PWR_FULL_ON);
        
    /* If DMA not running ? */
    if ( 0 == pThis->running ) {
    	//printf("DMA Not Running\n");
        /* directly put chunk to DMA transfer & enable */
        pThis->running  = 1;
        pThis->pPending = pChunk;
        audioTx_dmaConfig(pThis->pPending);  
        ENABLE_SPORT0_TX();
    } else {
    	//printf("DMA Running\n");
        /* DMA is already running, so we need to add chunk to queue
        1. Try to add chunk to queue and check status */
    	if (queue_put(&pThis->queue, pChunk) != PASS) {
    		/* 2.  If we could not add chunk to queue because queue is full,
    		return chunk to pool, effectivly dropping the chunk */
			bufferPool_release(pThis->pBuffP, pChunk);
			printf("TX Queue Full\n");
		}
    }
    
    return PASS;
}

