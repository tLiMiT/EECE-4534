/**
 *@file main.c
 *
 *@brief
 *  - Test audio loopback
 * 
 * 1. Cofigure I2C controller to communicate with codec
 * 2. receives audio via mic_in
 * 3. transfers received data via dma3
 * 4. Playback the audio data
 * 5. transfer data using DMA ch.4
 *
 * Target:   TLL6537v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:    Rohan Kangralkar, ECE, Northeastern University
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: main.c 513 2011-02-07 22:59:49Z rkangral $
 *
 *******************************************************************************/

#include <stdio.h>
#include "ssm2602.h"
#include <tll_config.h>
#include <sys/exception.h>
#include <bf52xI2cMaster.h>
#include "startup.h"
#include "tll_sport.h"
#include "audioPlayer.h"


/******************************************************************************
 *                     GLOBALS
 *****************************************************************************/
/**
 * @var audioPlayer
 * @brief  global audio player object
 */
audioPlayer_t            audioPlayer;
    
/** 
 *
 * Main function for audio playback
 *
 * Parameters:
 *
 * @return int
 */
int main(void)
{
    int                         status                  = -1;
    
    /* Blackfin setup function to configure processor */	
    status = blackfin_setup(); //returns 0 if successful and -1 if failed
    if (status) {
        printf("\r\n Blackfin Setup Failed"); 
        return -1;
    }
    
    /* FPGA setup function to configure FPGA, make sure the FPGA configuration
    binary data is loaded in to SDRAM at "FPGA_DATA_START_ADDR" */
    status = fpga_setup(); //returns 0 if successful and -1 if failed
    if (status) {
        printf("\r\n FPGA Setup Failed"); 
        return -1;
    }
    
    printf("[MAIN]: Starting Audio Player\n");
    
    audioPlayer_init(&audioPlayer);
    audioPlayer_start(&audioPlayer);
    audioPlayer_run(&audioPlayer);
}
