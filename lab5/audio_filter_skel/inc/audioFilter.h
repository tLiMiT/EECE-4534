/******************************************************************************
 *@file: audioFilter.h
 *
 *@brief: 
 *  - core module for audio filter
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:  Gunar Schirner
 *           Rohan Kangralkar
 *			 Anthony DiPasquale
 * @date 	 03/15/2009
 *
 * LastChange:
 * $Id: audioFilter.h 472 2010-07-09 22:07:48Z rkangral $
 *
 *******************************************************************************/


#ifndef _FILTER_H_
#define _FILTER_H_

#define FILTER_ECHO         (0x01)
#define FILTER_LOWPASS      (0x01<<1)
#define FILTER_HIGHPASS     (0x01<<2)
#define FILTER_COEFFICIENTS	33

#include <filter.h>
#include <string.h>
#include <chunk.h>

/** audioFilter attributes
 */
typedef struct {
	fir_state_fr16 	filter1State; /* state variable to hold filter 1 data */
	fir_state_fr16 	filter2State; /* state variable to hold filter 2 data */
	fir_state_fr16 	filter3State; /* state variable to hold filter 3 data */
	
	fract16			filter1_delay[FILTER_COEFFICIENTS]; /* delay line for filter 1 calculations */
	fract16			filter2_delay[FILTER_COEFFICIENTS]; /* delay line for filter 2 calculations */
	fract16			filter3_delay[FILTER_COEFFICIENTS]; /* delay line for filter 3 calculations */
	
} audioFilter_t;


/** Initialization for audioFilter
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 */
int audioFilter_init(audioFilter_t *pThis);

/** The optimized filter process of audioFilter
 *
 * @param pData  pointer to a chunk of audio data - the filtered data is returned in this variable
 * @param pState pointer to a state variable for filter coefficients and delay line
 *
 * @return no return
 */
void audioFilter_optimized(chunk_t *pData, fir_state_fr16 *pState);

/** The non-optimized filter process of audioFilter
 *
 * @param pData  pointer to a chunk of audio data - the filtered data is returned in this variable
 * @param pState pointer to a state variable for filter coefficients and delay line
 *
 * @return no return
 */
void audioFilter_filter(chunk_t *pData, fir_state_fr16 *pState);


#endif /* _FILTER_H_ */
