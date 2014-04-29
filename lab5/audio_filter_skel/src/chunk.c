/**
 *@file chunk.c
 *
 *@brief
 *  - chunk initializationand copy routenes
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: chunk.c 512 2011-02-07 22:59:49Z rkangral $
 *
 *******************************************************************************/
#include "tll_common.h"
#include "chunk.h"

/** Initialize buffer chunk
 *    - set max size of buffer and the current fill level
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int chunk_init(chunk_t *pThis)
{
    if ( NULL == pThis ) {
        return FAIL;
    }
    
    pThis->size = SAMPLE_SIZE;
    pThis->len  = 0; // default not filled
    return PASS;
}


/** copy on chunk into nother 
 *@param pSrc  pointer to source object (will not be modified)
 *@param pDst  pointer to destination object (will get the data of the src object)
 *
 *@return 0 success, non-zero otherwise
 **/
int chunk_copy(chunk_t *pSrc, chunk_t *pDst){
    unsigned int count;
    unsigned int len = pSrc->len/4;
    
    // copy manuall since memcpy does not work currently 
    // 
    for ( count = 0; len > count; count++ ) {
        pDst->u32_buff[count] = pSrc->u32_buff[count];
    }
    // update length of actual copied data
    pDst->len = pSrc->len;
   
    return PASS;
}

