/**
 *@file chunk.h

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
 * $Id: chunk.h 512 2011-02-07 22:59:49Z rkangral $
 *
 *******************************************************************************/

#ifndef _CHUNK_H_
#define _CHUNK_H_

/**
 * @def SAMPLE_SIZE
 * @brief Size of one chunk
 */
#define SAMPLE_SIZE                 (1024*2)

/**
 * Chunk status enumeration 
 */ 
typedef enum {
	START,     /** startup */
	PROGRESS,  /** in progress -- don't touch otherwise */
	COMPLETE,  /** finished processing */
	FREE       /** free */
} e_buff_status_t;



/** Chunk Object
 */
typedef struct {
  /* define a union to have different acess to same data in chunk */
  union {
    unsigned char       u08_buff[SAMPLE_SIZE];  /** Unsigned Data Chunk */
    unsigned short      u16_buff[SAMPLE_SIZE/2];
    unsigned int        u32_buff[SAMPLE_SIZE/4];
    signed char         s08_buff[SAMPLE_SIZE];  /** Signed Data Chunk */
    signed short        s16_buff[SAMPLE_SIZE/2];
    signed int          s32_buff[SAMPLE_SIZE/4];
  };
  int                 size;         /** total number bytes in chunk */ 
  int                 len;          /**  used bytes in chunk (fill level) */ 
  e_buff_status_t     e_status;     /** status */ 
  
} chunk_t;

/** initialize chunk 
 *  - reset size of chunk to maximum 
 *  - does NOT zero the buffer !
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int chunk_init(chunk_t *pThis); 



/** copy on chunk into nother 
 *@param pSrc  pointer to source object (will not be modified)
 *@param pDst  pointer to destination object (will get the data of the src object)
 *
 *@return 0 success, non-zero otherwise
 **/
int chunk_copy(chunk_t *pSrc, chunk_t *pDst); 


#endif
