/*********************************************************************
 *                              sine.h
 *
 * Copyright(C) 2008.MindTree Ltd. All rights reserved.
 *
 * Revision History:
 *
 * Date        Author           Description
 *
 * 30-Oct-2008 Hemanth Kumar.V  Initial version.
 * 02/20/10    Gunar Schirner   update 16kHz 16bit mono
 *
 * LastChange:
 * $Id: snd_sample.h 468 2010-07-09 03:59:44Z rkangral $
 *
 *******************************************************************************/

/**
 * @file snd_sample.h
 * @brief This file contains the buffers to hold pre-recorded audio.
 *
 */

#ifndef _SND_SAMPLE_H_
#define _SND_SAMPLE_H_

#include "ssm2602.h"

#if AUDIO_DIGITAL_LOOPBACK_FEATURE == ENABLED

#define RXBUFFER_SIZE 0x0100

extern char rxbuffer[RXBUFFER_SIZE];
#endif /* AUDIO_DIGITAL_LOOPBACK_FEATURE */

extern unsigned int  snd_samples_nBytes;
extern unsigned char snd_samples[] ;

#endif
