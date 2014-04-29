/**
 *@file audioPlayer.h
 *
 *@brief
 *  - core module for audio player
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 * @date 03/15/2009
 *
 * LastChange:
 * $Id$
 *
 *******************************************************************************/
#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include <bufferPool.h>
#include <audioRx.h>
#include <audioTx.h>
#include <ssm2602.h>



/** audioPlayer object
 */
typedef struct {
  audioRx_t      	rx;  /* receive object */
  audioTx_t      	tx;  /* transmit object */
  bufferPool_t   	bp;  /* buffer pool */
  isrDisp_t      	isrDisp; /* dispatcher for Rx Tx ISR */
  int 					volume;	/* Volume of the audio player */
  eSsm2602SampleFreq 	frequency;	/* Frequency of the audio player */
  chunk_t            *chunk;  /* Chunk for copy */
} audioPlayer_t;

/** initialize audio player 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioPlayer_init(audioPlayer_t *pThis);

/** startup phase after initialization 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioPlayer_start(audioPlayer_t *pThis);

/** main loop of audio player does not terminate
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
void audioPlayer_run(audioPlayer_t *pThis);

#endif



