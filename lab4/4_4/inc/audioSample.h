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
 * $Id: audioRx.h 513 2011-02-07 22:59:49Z rkangral $
 *
 *******************************************************************************/
#ifndef _AUDIO_SAMPLE_H_
#define _AUDIO_SAMPLE_H_


/***************************************************
            DEFINES
***************************************************/   


/***************************************************
            DATA TYPES
***************************************************/

/** audio RX object
 */
typedef struct {
  unsigned char *pmem;
  unsigned int  size;
  unsigned int  count;
}audioSample_t;


/***************************************************
            Access Methods 
***************************************************/

int audioSample_init(audioSample_t *pThis);

int audioSample_get(audioSample_t *pThis, chunk_t *pchunk_rx);


#endif
