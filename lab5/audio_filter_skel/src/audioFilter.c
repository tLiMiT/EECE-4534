
/******************************************************************************
 *@file: audioFilter.c
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
 * LastChange:
 * $Id: audioFilter.c 472 2010-07-09 22:07:48Z rkangral $
 *
 *******************************************************************************/

#include <tll_common.h>
#include "audioFilter.h"

/* Declare filter coefficients - Generated by Matlab filterbuilder */
fract16 audioFilter_filter1_coeff[] = {22, -296, -107, 275, 295, -286, -589, 151, 959, 231, -1355, -1032, 1711, 2774, -1958, -10183, 18431, -10183, -1958, 2774, 1711, -1032, -1355, 231, 959, 151, -589, -286, 295, 275, -107, -296, 22};
fract16 audioFilter_filter2_coeff[] = {-67, 253, 260, 71, -291, -506, -246, 431, 957, 658, -558, -1819, -1714, 647, 4728, 8635, 10244, 8635, 4728, 647, -1714, -1819, -558, 658, 957, 431, -246, -506, -291, 71, 260, 253, -67};
fract16 audioFilter_filter3_coeff[] = {166, 99, -299, -570, -19, 993, 979, -639, -2028, -920, 1888, 2769, 9, -3226, -2596, 1437, 3801, 1437, -2596, -3226, 9, 2769, 1888, -920, -2028, -639, 979, 993, -19, -570, -299, 99, 166};

/* Declare 2 arrays for filter input and output */
fract16 audioFilter_output[SAMPLE_SIZE/2];
fract16 audioFilter_input[SAMPLE_SIZE/2];

/** Initialization for audioFilter
 *
 * @param pThis  pointer to own object
 *
 * @return Zero on success, negative otherwise 
 */
int audioFilter_init(audioFilter_t *pThis)
{
	int i;
	for(i = 0; i < FILTER_COEFFICIENTS; i++)
	{
		pThis->filter1_delay[i] = 0;
		pThis->filter2_delay[i] = 0;
		pThis->filter3_delay[i] = 0;
	}
	
	fir_init(pThis->filter1State, audioFilter_filter1_coeff, pThis->filter1_delay, FILTER_COEFFICIENTS, 1);
	fir_init(pThis->filter2State, audioFilter_filter2_coeff, pThis->filter2_delay, FILTER_COEFFICIENTS, 1);
	fir_init(pThis->filter3State, audioFilter_filter3_coeff, pThis->filter3_delay, FILTER_COEFFICIENTS, 1);

	return PASS;
}

/** The optimized filter process of audioFilter
 *
 * @param pData  pointer to a chunk of audio data - the filtered data is returned in this variable
 * @param pState pointer to a state variable for filter coefficients and delay line
 *
 * @return no return
 */
void audioFilter_optimized(chunk_t *pData, fir_state_fr16 *pState)
{
	chunk_t			temp_chunk;

	chunk_init(&temp_chunk);
	

	temp_chunk.len = pData->len;
	
	chunk_copy(&temp_chunk, pData);
}

/** The non-optimized filter process of audioFilter
 *
 * @param pData  pointer to a chunk of audio data - the filtered data is returned in this variable
 * @param pState pointer to a state variable for filter coefficients and delay line
 *
 * @return no return
 */
void audioFilter_filter(chunk_t *pData, fir_state_fr16 *pState)
{
	//declare counters and temp variable
	int i, j;
	int temp;
	chunk_t			temp_chunk;

	chunk_init(&temp_chunk);
    	
	//loop through each data point
	for(i = 0; i < pData->len/2; i++)
	{		
		temp = 0;
		
		//prime the delay line with the new data
		
		//sum the coefficients multiplied by the delay line
		for(j = 0; j < pState->k; j++)
		{				
		}
		
		//shift the delay line down 1 to make room for the new data short next cycle
			
		//type cast and shift the data for the output
	}
	
	temp_chunk.len = pData->len;
	
	chunk_copy(&temp_chunk, pData);
}
