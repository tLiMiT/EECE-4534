/**
 *@file bufferPool.c
 *
 *@brief
 *  - Global buffer pool divided into chunks and kept on the free list
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: bufferPool.c 512 2011-02-07 22:59:49Z rkangral $
 *
 *******************************************************************************/
#include "tll_common.h"
#include "bufferPool.h"

/** Initialize buffer pool 
 *    - initialize freeList, populate with chunks
  *
 * Parameters:
 * @param pThis  pointer to buffer pool data structure
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_init(bufferPool_t *pThis)
{
    int                         count                   = 0;
    
    // init queue
    if ( FAIL == queue_init(&pThis->freeList, CHUNK_NUM_MAX) ) {
        printf("[BP]: Failed to initialize free list\n");
        return FAIL;
    }
    
    /* We put all the chunk on the free list */
    for(count = 0; CHUNK_NUM_MAX > count; count++){
        // init chunk 
        chunk_init(&pThis->buffer[count]);
        // put initialized chunk into queue 
        queue_put(&pThis->freeList, (void **)&pThis->buffer[count] );
    }
    
    printf("[BP]: Initialised\n");
    return PASS;
}

/** Get a chunk from the  buffer pool 
 *
 * Parameters:
 * @param pThis    pointer to queue data structure
 * @param ppChunk  pointer pointer to chunk aqcuired (null if emtpy)
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_acquire(bufferPool_t *pThis, chunk_t **ppChunk)
{
    if ( NULL == pThis || NULL == ppChunk ) {
        printf("[BP]: Acquire failed\n");
        return FAIL;
    }
    
    if (FAIL == queue_get(&pThis->freeList, (void **)ppChunk) ){
        *ppChunk = NULL;
        return FAIL;
    }
    (*ppChunk)->size = SAMPLE_SIZE;
    (*ppChunk)->len  = 0;
    return PASS;
}



/** Release chunk into the free list 
 *    - non blocking 
 *    - error on null passed 
  *
 * Parameters:
 * @param pThis    pointer to queue data structure
 * @param pChunk    pointer to chunk to release
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_release(bufferPool_t *pThis, chunk_t *pChunk)
{
    if ( NULL == pThis || NULL == pChunk ) {
        printf("[BP]: Acquire failed\n");
        return FAIL;
    }    
    
    if (FAIL == queue_put(&pThis->freeList, (void **)pChunk) ){
        pChunk = NULL;
        return FAIL;
    }
    return PASS;
}

/** Returns true if buffer pool is empty
 *
 *
 * Parameters:
 * @param pThis  pointer to queue data structure 
 *
 * @return   true (non-zero) if emtpy, 0 if no chunks available 
 */
int bufferPool_is_empty(bufferPool_t *pThis )
{
    if ( NULL == pThis ) {
        printf("[BP]: bufferPool_is_empty failed\n");
        return FAIL;
    }   
    
    if ( FAIL != queue_is_empty(&pThis->freeList) ) {
        printf("[BP]: The buffer has free chunks\n");
        return FAIL;
    }
    
    return PASS;
}

