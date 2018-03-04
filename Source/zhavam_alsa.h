/*
 * zhavam_alsa.h
 *
 *  Created on: 7 ene. 2018
 *      Author: ipserc
 */

#include <alsa/asoundlib.h>

#include <errno.h>
#include <stddef.h>

#include "zhavam_acrcloud.h"
#include "acrcloud_recognizer.h"

#ifndef SOURCE_ZHAVAM_ALSA_H_
#define SOURCE_ZHAVAM_ALSA_H_

extern char STATUS_MESSAGE[];
extern int errno;

/**
 * PCM format enumerator
 */
typedef enum _zhv_snd_pcm_format
{
	IND_PCM_FORMAT_S8,
	IND_PCM_FORMAT_U8,
	IND_PCM_FORMAT_S16,
	IND_PCM_FORMAT_U16,
	IND_PCM_FORMAT_S24,
	IND_PCM_FORMAT_U24,
	IND_PCM_FORMAT_S32,
	IND_PCM_FORMAT_U32,
	IND_LAST_PCM_FORMAT
}zhv_snd_pcm_format_t;

/**
 * ALSA configuration parameters structure
 */
typedef struct
{
	snd_pcm_format_t snd_pcm_format;
	unsigned int rate;
	unsigned int pcm_buffer_frames;
	char * pcm_dev;
} alsa_config;

/*
 * prototypes
 */
char *getZhvSndPcmFormatStr(void);
snd_pcm_format_t sndPcmFormatDecode(char *sndPcmFormat);
char *pcmFormatString(char *strSndPcmFormat, snd_pcm_format_t sndPcmFormat);
int openDevice(char *devID, snd_pcm_t **ptr_capture_handle, snd_pcm_hw_params_t **ptr_hw_params);
int setupDevice(snd_pcm_t *capture_handle, snd_pcm_hw_params_t *hw_params, snd_pcm_format_t format, unsigned int *rate);
int setupAudioDevice(char *devID, snd_pcm_t *capture_handle, snd_pcm_hw_params_t *hw_params, snd_pcm_format_t format, unsigned int *rate);
int pcmPrepare(snd_pcm_t *capture_handle);
void printPcmBuffer(size_t size, char *pcm_buffer);
void writePcmBuffer(size_t size, char *pcm_buffer);
int startRecord(snd_pcm_t *capture_handle, snd_pcm_hw_params_t *hw_params, snd_pcm_format_t format, unsigned int sample_rate, unsigned int pcm_buffer_frames, acrcloud_config acrConfig, acr_data_t *acrResponse);
int closeDevice(snd_pcm_t *capture_handle);

#endif /* SOURCE_ZHAVAM_ALSA_H_ */
