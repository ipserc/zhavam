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
#include "zhavam_jsonparser.h"

#ifndef SOURCE_ZHAVAM_ALSA_H_
#define SOURCE_ZHAVAM_ALSA_H_

extern char STATUS_MESSAGE[];
extern int errno;

#define SND_PCM_FORMAT_UNKNOWN -1

/**
 * PCM format enumerator for zhavam
 */
typedef enum _zhv_alsa_snd_pcm_format
{
	IND_SND_PCM_FORMAT_S8 ,
	IND_SND_PCM_FORMAT_U8 ,
	IND_SND_PCM_FORMAT_S16,
	IND_SND_PCM_FORMAT_U16,
	IND_SND_PCM_FORMAT_S24,
	IND_SND_PCM_FORMAT_U24,
	IND_SND_PCM_FORMAT_S32,
	IND_SND_PCM_FORMAT_U32,
	IND_SND_PCM_FORMAT_LAST_ITEM,
	IND_SND_PCM_FORMAT_UNKNOWN = -1
} zhv_alsa_snd_pcm_format_t;

/**
 * ALSA configuration parameters structure
 */
typedef struct
{
	snd_pcm_format_t snd_pcm_format;
	unsigned int rate;
	unsigned int pcm_buffer_frames;
	char * pcm_dev;
} alsa_config_t;

/*
 * prototypes
 */
/* zhavam_alsa.c */
const char **getZhvAlsaSndPcmFormatList(void);
snd_pcm_format_t alsaSndPcmFormatDecode(const char *strSndPcmFormat);
const char *alsaSndPcmFormatString(snd_pcm_format_t sndPcmFormat);
int openDevice(char *devID, snd_pcm_t **ptr_capture_handle, snd_pcm_hw_params_t **ptr_hw_params);
int setupDevice(snd_pcm_t *capture_handle, snd_pcm_hw_params_t *hw_params, snd_pcm_format_t format, unsigned int *rate);
int setupAudioDevice(char *devID, snd_pcm_t *capture_handle, snd_pcm_hw_params_t *hw_params, snd_pcm_format_t format, unsigned int *rate);
int pcmPrepare(snd_pcm_t *capture_handle);
void printPcmBuffer(size_t size, char *pcm_buffer);
void writePcmBuffer(size_t nmemb, char *pcm_buffer);
int alsaStartRecord(snd_pcm_t *capture_handle, snd_pcm_hw_params_t *hw_params, snd_pcm_format_t format, unsigned int sample_rate, unsigned int pcm_buffer_frames, acrcloud_config acrConfig, acr_data_t *acrResponse);
int closeDevice(snd_pcm_t *capture_handle);

#endif /* SOURCE_ZHAVAM_ALSA_H_ */
