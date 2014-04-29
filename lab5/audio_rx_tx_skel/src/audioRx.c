/**
 *@file audioRx.c
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
 * $Id: audioRx.c 552 2011-03-09 15:24:06Z adipasqu $
 *
 *******************************************************************************/
#include "tll_common.h"
#include "audioRx.h"
#include "bufferPool.h"
#include "isrDisp.h"
#include <tll_config.h>
#include <tll_sport.h>
#include <queue.h>
#include <power_mode.h>


/**
 * @def ENABLE_FILE_STUB
 * @brief define this to use a file stub instead of actual audio input
 */
#define ENABLE_FILE_STUB

#ifdef ENABLE_FILE_STUB
#define FILE_NAME "../testaudio.bin"
#endif


#ifdef ENABLE_FILE_STUB
/** read a chunk of data from the file
 *    - check for end of file 
 *    - if eof rewind back to the actual data
 *    - read the file into chunk
 *    - update the length

 * Parameters:
 * @param pchunk_rx  pointer to rx chunk
 *
 * @return Zero on success.
 */
int audioRx_fileRead(FILE *pFile, chunk_t *pchunk_rx)
{
    int count = 0;
    /**
     * Rewind the file to the begning of the file and skip the wav header
     */
    if ( feof(pFile) ) {
        fseek(pFile, 44, SEEK_SET);
    }
    count = fread(&pchunk_rx->u08_buff[0],  pchunk_rx->size, 1, pFile);
    // this is NOT right, however, the second fread call returns a smaller number 
    // although buffer was read completely
    pchunk_rx->len = pchunk_rx->size;   
    return PASS;
}
#endif

/** 
 * Configures the DMA rx with the buffer and the buffer length to 
 * receive
 * Parameters:
 * @param pchunk  pointer to receive chunk
 *
 * @return void
 */
void audioRx_dmaConfig(chunk_t *pchunk)
{
    DISABLE_DMA(*pDMA3_CONFIG);
    *pDMA3_START_ADDR   = &pchunk->u16_buff[0];
    *pDMA3_Y_COUNT		= pchunk->size/2;  // 16 bit data so we change the stride and count
    *pDMA3_X_COUNT      = 2;
    *pDMA3_Y_MODIFY		= 2;
    *pDMA3_X_MODIFY     = 0;
    ENABLE_DMA(*pDMA3_CONFIG);
}




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
                 isrDisp_t *pIsrDisp)
{
    if ( NULL == pThis || NULL == pBuffP || NULL == pIsrDisp) {
        printf("[ARX]: Failed init\n");
        return FAIL;
    }
    
    pThis->pPending     = NULL;
    pThis->pBuffP       = pBuffP;
    
    // init queue with 
    queue_init(&pThis->queue, AUDIORX_QUEUE_DEPTH);   
 
     /* Configure the DMA3 for RX (data receive/memory write) */
     /* Read, 1-D, interrupt enabled, Memory write operation, 16 bit transfer,
      * Auto buffer
      */
    *pDMA3_CONFIG = WNR | WDSIZE_16 | DI_EN | DMA2D;

    /**
     * Register the interrupt handler
     */
    isrDisp_registerCallback(pIsrDisp, ISR_DMA3_SPORT0_RX, audioRx_isr, pThis);
    
    printf("[ARX]: RX init complete\n");
    
    return PASS;
}




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
int audioRx_start(audioRx_t *pThis)
{
#ifndef ENABLE_FILE_STUB
    /* prime the system by getting the first buffer filled */
     if ( FAIL == bufferPool_acquire(pThis->pBuffP, &pThis->pPending ) ) {
         printf("[ARX]: Failed to acquire buffer\n");
         return FAIL;
     }
     
     /* get a free chunk from the buffer pool */
     
     /* configure the dma to fill the empty chunk */
     
     // enable the audio transfer 
     ENABLE_SPORT0_RX();
#else
    pThis->audioRx_pFile = fopen(FILE_NAME, "rb");
    if(pThis->audioRx_pFile== NULL) {
        printf("Can't open files.\n");
    }
    fseek(pThis->audioRx_pFile, 44, SEEK_SET); // remove wav header
#endif
     
     return PASS;                                         
}



/** audioRx_isr

 * Parameters:
 * @param pThisArg  pointer to own object
 *
 * @return None 
 */
void audioRx_isr(void *pThisArg)
{
    // local pThis to avoid constant casting 
    audioRx_t *pThis  = (audioRx_t*) pThisArg; 
    
    /* a. Check if it is DMA3 IRQ that caused this interrupt */
    
    /* clear the status DMA IRQ STATUS */
        
    /* d. chunk is now filled so update the length i.e update the len of the pending chunk with the max  size */
        
        
    /* e. Insert the chunk previously read by the DMA RX on the
       RX QUEUE
     */
    
     /* What happens if the queue was already full? 
        Reuse the same buffer and overwrite last samples if we fail to put it on the cqueue 
        the packet is effectivly dropped 
      */

    /* f. If we have sucessfully put a packet on the RX queue then we acquire a
       new chunk from the buffer pool. The new chunk received from the bufferpool
       becomes our pending buffer and the dma is configured to receive data into it. 
      */
            
}



/** audio rx get 
 *   copyies a filled chunk into pChunk
 *   blocking call, blocks if queue is empty 
 *     - get from queue 
 *     - copy in to pChunk
 *     - release chunk to buffer pool 
 * Parameters:
 * @param pThis  pointer to own object
 * @param pChunk Pointer to chunk object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRx_get(audioRx_t *pThis, chunk_t *pChunk)
{
    chunk_t                  *chunk_rx;
    int                         count                   = 0;
    
    
#ifdef ENABLE_FILE_STUB 
    audioRx_fileRead(pThis->audioRx_pFile, pChunk);
#else
    /* Block till a chunk arrives on the rx queue */
    while( queue_is_empty(&pThis->queue) ) {
        powerMode_change(PWR_ACTIVE);
        asm("idle;");
    }
    powerMode_change(PWR_FULL_ON);
    
    queue_get(&pThis->queue, (void**)&chunk_rx);

    chunk_copy(chunk_rx, pChunk);
    
    if ( FAIL == bufferPool_release(pThis->pBuffP, chunk_rx) ) {
        return FAIL;
    }
    
#endif
    return PASS;
}

