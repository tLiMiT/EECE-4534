/**
 *@file bufferPool.h
 *
 *@brief
 *  - manages a fixed size buffer pool for chunks
 *
 * Target:   TLL6537v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author    Rohan Kangralkar
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: bufferPool.h 512 2011-02-07 22:59:49Z rkangral $
 *
 *******************************************************************************/
#ifndef _BUFFER_POOL_H_
#define _BUFFER_POOL_H_

#include "queue.h"
#include "isrDisp.h"
#include <chunk.h>

/***************************************************
            DEFINES
***************************************************/   
/**
 * @def CHUNK_NUM_MAX
 * @brief maximum number of chunks managed in this bufffer pool
 * since we use static allocation, one number for all
 */
#define CHUNK_NUM_MAX (32)

/***************************************************
            DATA TYPESt
***************************************************/

/** bufferPool object
 */
typedef struct {
  queue_t    freeList;  /* List of free chunks */
  chunk_t    buffer[CHUNK_NUM_MAX];
  isrDisp_t  isrDisp; /* dispatcher for Rx Tx ISR */
} bufferPool_t;


/***************************************************
            Access Methods 
***************************************************/

/** Initialize buffer pool 
 *    - initialize freeList, populate with chunks
  *
 * Parameters:
 * @param pThis  pointer to buffer pool
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_init(bufferPool_t *pThis);


/** Get a chunk from the  buffer pool 
 *
 * Parameters:
 * @param pThis    pointer to queue data structure
 * @param ppChunk  pointer pointer to chunk aqcuired (null if emtpy)
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_acquire(bufferPool_t *pThis, chunk_t **ppChunk);

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
int bufferPool_release(bufferPool_t *pThis, chunk_t *pChunk);

/** Returns true if buffer pool is empty
 *
 *
 * Parameters:
 * @param pThis  pointer to queue data structure 
 *
 * @return   true (non-zero) if emtpy, 0 if no chunks available 
 */
int bufferPool_is_empty(bufferPool_t *pThis );

#endif

