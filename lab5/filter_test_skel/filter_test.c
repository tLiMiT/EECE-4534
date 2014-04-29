/*****************************************************************************
 * Filter_Test.c
 *****************************************************************************/

#include <stdio.h> 
#include <string.h>
#include <filter.h>
#include <cycle_count.h>

#define DO_CYCLE_COUNTS
#define FILTER_SIZE		(32)
#define BUFFER_SIZE		(2048)
#define WAVE_HEADER_SIZE (108)

FILE *fileInput;
FILE *fileOutput;

//filter functions
void filter(short *pData, short *pDelay, short *pCoeffs, int length, int nCoeffs);
void filter_optimized(fract16 *pData, int length, fir_state_fr16 *pState);
 
//main function
int main( void )
{	
	//filter coefficients
	short coeffs[FILTER_SIZE] = {-52,-118,-224,-354,-485,-574,-571,-424,-93,436,1143,1970,2829,3609,4203,4525,4525,4203,3609,2829,1970,1143,436,-93,-424,-571,-574,-485,-354,-224,-118,-52};
	char header[WAVE_HEADER_SIZE];			//holder array for wav header data
	short dataIn[BUFFER_SIZE];	//in data buffer
	short delay[FILTER_SIZE];	//delay line for holding last samples of previous data	
	fir_state_fr16 filter_state;//filter state variable for optimized filter	
	cycle_t cycles_init ; //cycle stats start variable
	cycle_t cycles_fin ; //cycle stats finish variable
	
	int count =0;					//variable for valid data buffer length
	int i = 0;					//counter for cycle averaging	
	unsigned long total = 0;	//variable to hold number of bytes transfered
	
	//open the files	
	printf("\nOpening the files ...\n");	
	fileInput = fopen("../200Hz_to_10kHz_44100_16bit.wav", "rb");
	if ( NULL == fileInput ){
		printf("Input file not found\n");
		return -1;
	}
	fileOutput = fopen("../filtered_wave.wav", "wb");
	if ( NULL == fileOutput ){
		perror("");
		return -1;
	}	
	//copy the wav header to the new file
	printf("Copying header ...\n");
	fread(header, sizeof(char), 108, fileInput);
	fwrite(header, sizeof(char), 108, fileOutput);
	
	//initialize the optimized filter state for pre 5.6

	
	//initialize the cycle counter for pre 5.7
	
	
	printf("Filtering ...\n");
	
	do
	{
		//read the file and save the number of shorts read
		count = fread(dataIn, sizeof(short), BUFFER_SIZE, fileInput);
		
		//start the cycle counter for pre 5.7
		
		//filter the data
		filter(dataIn, delay, coeffs, count, FILTER_SIZE);
		//filter_optimized(dataIn, count, &filter_state);
		
		//stop the cycle counter for pre 5.7
		
		//write the data back to a file
		fwrite(dataIn, sizeof(short), count, fileOutput);
		
		//print the bytes that have been transfered
		total += count*2;
		printf("%i Bytes filtered\n", total);
				
	} while(!feof(fileInput)); //loop untill the file ends
	
	printf("\nCycle statistics:\n");
	
	//close the files
	fclose(fileInput);
	fclose(fileOutput);
	
	//finish the program
	printf("\nDone.\n");
	
	return 0;
}

/**
 * filter function for pre 5.5
 *
 */
void filter(short *pData, short *pDelay, short *pCoeffs, int length, int nCoeffs)
{
	//declare counters and temp variable
	int i = 0;
	int j = 0;
	int temp = 0;
	
	//loop through each data point
	for(i = 0; i < length; i++)
	{		
		temp = 0;
		
		//prime the delay line with the new data
		
		//sum the coefficients multiplied by the delay line
		for(j = 0; j < nCoeffs; j++)
		{				

		}
		
		//shift the delay line down 1 to make room for the new data short next cycle.
					
		//shift and type cast the data for the output
		
	}
}

//optimized filter implementation for pre 5.6
void filter_optimized(fract16 *pData, int length, fir_state_fr16 *pState)
{

}
