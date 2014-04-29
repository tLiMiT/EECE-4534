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
 *           Rohan Kangralkar
 * @date 03/15/2009
 *
 * LastChange:
 * $Id: audioRx.c 846 2014-02-27 15:35:54Z fengshen $
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
#include <audioSample.h>

/**
 * @def ENABLE_FILE_STUB
 * @brief define this to use audio from file over JTAG instead of audio in memory
 */
#undef ENABLE_FILE_STUB

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
 * @return number of bytes read.
 */
int audioRx_fileRead(FILE *pFile, chunk_t *pchunk_rx)
{
    int count = 0;
    /** Rewind to the beginning of the file and skip the wav header */
    if ( feof(pFile) ) {
        fseek(pFile, 44, SEEK_SET);
    }
    count = fread(&pchunk_rx->u08_buff[0],  pchunk_rx->bytesMax, 1, pFile);

    // this is NOT right, however, the second fread call returns a smaller number 
    // although buffer was read completely
    pchunk_rx->len = pchunk_rx->bytesMax;   
    return pchunk_rx->len;
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
//void audioRx_dmaConfig(chunk_t *pchunk)
//{
//   /* 1. Disable DMA 3*/
//
//   /* 2. Configure start address */
//
//   /* 3. set X count */
//
//   /* 4. set X modify */
//
//   /* 5 Re-enable DMA */
//
//}




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
        printf("[ARX]: Failed init\r\n");
        return FAIL;
    }
    
    pThis->pPending     = NULL;
    pThis->pBuffP       = pBuffP;
    
    // init queue with 
    queue_init(&pThis->queue, AUDIORX_QUEUE_DEPTH);   

#ifndef ENABLE_FILE_STUB
    audioSample_init(&pThis->audioSample);
#endif

    printf("[ARX]: RX init complete\r\r\n");
    
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
#ifdef ENABLE_FILE_STUB
	pThis->audioRx_pFile = fopen(FILE_NAME, "rb");
    if(NULL == pThis->audioRx_pFile) {
        printf("Can't open files: %s\r\n",FILE_NAME);
    }
    fseek(pThis->audioRx_pFile, 44, SEEK_SET); // remove wav header
#else
    pThis->audioSample.count = 44; /** we skip the wave header we assume that the data is
                             16 bit mono*/
#endif
                             
    return PASS;
}



/** audio rx isr  (to be called from dispatcher) 

 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return None 
 */
//void audioRx_isr(void *pThisArg)
//{
//    // local pThis to avoid constant casting
//    audioRx_t *pThis  = (audioRx_t*) pThisArg;
//
//    if ( *pDMA3_IRQ_STATUS & 0x1 ) {
//
//        // chunk is now filled, so update the length
//        pThis->pPending->bytesUsed = pThis->pPending->bytesMax;
//
//        /* 1. Attempt to insert the pending chunk previously read by the
//         *  DMA RX into the RX QUEUE and a data is inserted to queue
//         */
//
//            /* 2. If chunk could not be inserted into the queue,
//             * configure the DMA and overwrite last samples.
//             * This means the RX packet was dropped */
//
//            /*
//             * 3. Otherwise, attempt to acquire a chunk from the buffer
//             *    pool and if successful, configure the DMA to write
//             *    to this chunk. If not successful, then we are out of
//             *    memory because the buffer pool is empty */
//
//        *pDMA3_IRQ_STATUS  |= 0x0001;  // clear the interrupt
//    }
//}



/** audio rx get 
 * @brief Read the chunk from the file
 * Parameters:
 * @param pThis  pointer to own object
 * @param pChunk Chunk Pointer to be filled
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRx_get(audioRx_t *pThis, chunk_t **pChunk)
{
	int size = 0;

    if (bufferPool_acquire(pThis->pBuffP, pChunk) != PASS) {
        printf("Could not acquire chunk for audio sample\n");
        return FAIL;
    }

#ifdef ENABLE_FILE_STUB
    size = audioRx_fileRead(pThis->audioRx_pFile, *pChunk);
#else
    size = audioSample_get(&pThis->audioSample, *pChunk);
#endif

    return size > 0 ? PASS: FAIL;
}

