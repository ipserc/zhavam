/*
 * zhavam_alsa.c
 *
 *  Created on: 6 ene. 2018
 *      Author: ipserc
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn.h"
#include "list.h"

#include "zhavam.h"
#include "zhavam_acrcloud.h"
#include "zhavam_alsa.h"
#include "zhavam_config.h"
#include "zhavam_devices.h"
#include "zhavam_errtra.h"
#include "zhavam_jsonparser.h"
#include "acrcloud_recognizer.h"

/**
 * "member" variable with the text names of the SND_PCM_FORMAT used
 */
char * zhv_snd_pcm_format_str[] = {
		"SND_PCM_FORMAT_S8" ,
		"SND_PCM_FORMAT_U8" ,
		"SND_PCM_FORMAT_S16",
		"SND_PCM_FORMAT_U16",
		"SND_PCM_FORMAT_S24",
		"SND_PCM_FORMAT_U24",
		"SND_PCM_FORMAT_S32",
		"SND_PCM_FORMAT_U32"
};

/**
 * "member" variable with the values of the SND_PCM_FORMAT used sort as defined by zhv_snd_pcm_format_t
 */
int zhv_snd_pcm_format_int[] = {
		SND_PCM_FORMAT_S8 ,
		SND_PCM_FORMAT_U8 ,
		SND_PCM_FORMAT_S16,
		SND_PCM_FORMAT_U16,
		SND_PCM_FORMAT_S24,
		SND_PCM_FORMAT_U24,
		SND_PCM_FORMAT_S32,
		SND_PCM_FORMAT_U32
};

/**
 * get "method" to "memeber" variable zhv_snd_pcm_format_str
 */
char * getZhvSndPcmFormatStr(void)
{
	return zhv_snd_pcm_format_str;
}

/**
 * Looks for the SND_PCM_FORMAT enumerator supported by zhavam.
 * Returns the SND_PCM_FORMAT enumerator or SND_PCM_FORMAT_UNKNOWN if not found
 */
snd_pcm_format_t sndPcmFormatDecode(char * sndPcmFormat)
{
	for(int i = IND_PCM_FORMAT_S8; i < IND_LAST_PCM_FORMAT; ++i )
		if (!strcmp(sndPcmFormat, zhv_snd_pcm_format_str[i] )) return zhv_snd_pcm_format_int[i];
	return SND_PCM_FORMAT_UNKNOWN;
}

/**
 * Looks for the SND_PCM_FORMAT name supported by zhavam.
 * Returns the SND_PCM_FORMAT name or "SND_PCM_FORMAT_S8" if not found
 */
char * pcmFormatString(char * strSndPcmFormat, snd_pcm_format_t sndPcmFormat)
{
	switch (sndPcmFormat)
	{
	case SND_PCM_FORMAT_S8 : strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_S8" ); break;
	case SND_PCM_FORMAT_U8 : strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_U8" ); break;
	case SND_PCM_FORMAT_S16: strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_S16"); break;
	case SND_PCM_FORMAT_U16: strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_U16"); break;
	case SND_PCM_FORMAT_S24: strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_S24"); break;
	case SND_PCM_FORMAT_U24: strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_U24"); break;
	case SND_PCM_FORMAT_S32: strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_S32"); break;
	case SND_PCM_FORMAT_U32: strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_U32"); break;
	default: strSndPcmFormat = strcpy(strSndPcmFormat, "SND_PCM_FORMAT_S8");
	}
	return strSndPcmFormat;
}

/**
 * Opening device tasks
 * 	snd_pcm_open
 * 	snd_pcm_hw_params_malloc
 * @param char * devID: The name of the device to open
 * @param snd_pcm_t ** ptr_capture_handle: Capture handler structure
 * @param snd_pcm_hw_params_t ** ptr_hw_params: Pointer to the hardware params structure
 * @return the status of the operations
 */
int openDevice(char * devID,
				snd_pcm_t ** ptr_capture_handle,
				snd_pcm_hw_params_t ** ptr_hw_params)
{
	if ((errno = snd_pcm_open(ptr_capture_handle, devID, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot open audio device %s (%s)",
			   devID,
			   snd_strerror(errno));
		//ERROR("%s", STATUS_MESSAGE);
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "audio device %s opened", devID);
	}
	if ((errno = snd_pcm_hw_params_malloc(ptr_hw_params)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot allocate hardware parameter structure (%s)",
				snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params allocated");
	}

	return errno;
}

/**
 * Setup device tasks
 * 	snd_pcm_hw_params_any
 * 	snd_pcm_hw_params_set_access
 * 	snd_pcm_hw_params_set_format
 * 	snd_pcm_hw_params
 * @param snd_pcm_t * capture_handle
 * @param snd_pcm_hw_params_t * hw_params
 * @param snd_pcm_format_t format
 * @param unsigned int * rate
 * @return the status of the operations
 */
int setupDevice(snd_pcm_t * capture_handle,
				snd_pcm_hw_params_t * hw_params,
				snd_pcm_format_t format,
				unsigned int * rate)
{
	if ((errno = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot initialize hardware parameter structure (%s)",
			 snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params initialized");
	}

	if ((errno = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		sprintf (STATUS_MESSAGE, "cannot set access type (%s)",
			 snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params access has been set");
	}

	if ((errno = snd_pcm_hw_params_set_format(capture_handle, hw_params, format)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot set sample format (%s)",
			 snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params format has been set");
	}

	if ((errno = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, rate, 0)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot set sample rate (%s)",
			 snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params rate has been set");
	}

	/****************************
	****************************
	if ((errno = snd_pcm_hw_params_set_channels(capture_handle, hw_params, 2)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot set channel count (%s)",
			 snd_strerror(errno));
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params channels have been set");
		TRACE("%s", STATUS_MESSAGE);
	}
	****************************
	****************************/

	if ((errno = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot set hw parameters (%s)",
			 snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params have been set");
	}
	return errno;
}

/**
 * Device set up and PCM preparation
 * @param char * devID: The name of the device to open
 * @param snd_pcm_t * capture_handle: Capture handler structure
 * @param snd_pcm_hw_params_t * hw_params: Pointer to the hardware params structure
 * @param snd_pcm_format_t format: Sound PCM format
 * @param unsigned int * rate: Recording rate
 * @return the status of the operations
 */
int setupAudioDevice(char * devID,
						snd_pcm_t * capture_handle,
						snd_pcm_hw_params_t * hw_params,
						snd_pcm_format_t format,
						unsigned int * rate)
{
	if ((errno = setupDevice(capture_handle, hw_params, format, rate)) < 0) return errno;
	snd_pcm_hw_params_free(hw_params);
	sprintf(STATUS_MESSAGE, "hw_params freed");
	if ((errno = pcmPrepare(capture_handle)) < 0) return errno;
	sprintf(STATUS_MESSAGE, "audio device ready");
	return errno;
}

/**
 * PCM prepare tasks
 * 	snd_pcm_prepare
 * @param snd_pcm_t * capture_handle: Capture handler structure
 * @return the status of the operation
 */
int pcmPrepare(snd_pcm_t * capture_handle)
{
	if ((errno = snd_pcm_prepare(capture_handle)) < 0) {
		sprintf (STATUS_MESSAGE, "cannot prepare audio interface for use (%s)",
			   snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
	}
	else {
		sprintf(STATUS_MESSAGE, "audio interface prepared");
	}
	return errno;
}

/**
 * Prints in console the content of the PCM Buffer
 * @param size_t size: Size of the buffer
 * @param char * pcm_buffer. Pointer to the buffer
 */
void printPcmBuffer(size_t size,
					char * pcm_buffer)
{
	char * pt = pcm_buffer;
	for (int i = 0; i < size; ++i)
		printf("%u", (*pt)++);
	puts("");
}

/**
 * Writes the content of the PCM Buffer in a file called "grabacion.raw" in the current directory
 * @param size_t size: Size of the buffer
 * @param char * pcm_buffer. Pointer to the buffer
 */
void writePcmBuffer(size_t size,
					char * pcm_buffer)
{
	FILE * file = fopen("grabacion.raw", "wb");
	fwrite(pcm_buffer, sizeof(char), size, file);
	fclose(file);
}

/**
 * Starts recording the stream from the PCM and tries to recognize the recorded buffer using the service provided by ACRCloud
 * 	snd_pcm_hw_params_get_channels
 * 	snd_pcm_readi
 * 	acr_recognize_by_pcm
 * @param snd_pcm_t * capture_handle: Capture handler structure
 * @param snd_pcm_hw_params_t * hw_params: Pointer to the hardware params structure
 * @param snd_pcm_format_t format: Sound PCM format
 * @param unsigned int * rate: Recording rate
 * @return the status of the operations
 */
int startRecord(snd_pcm_t * capture_handle,
		snd_pcm_hw_params_t * hw_params,
		snd_pcm_format_t format,
		unsigned int sample_rate,
		unsigned int pcm_buffer_frames,
		acrcloud_config acrConfig,
		acr_data_t * acrResponse)
{
	char * pcm_buffer;
	char * reconResponse;
	size_t pcm_buffer_len = pcm_buffer_frames * snd_pcm_format_width(format) / 8 * 2;
	if (!(pcm_buffer = malloc(pcm_buffer_len))) {
		sprintf (STATUS_MESSAGE, "memory allocation failed (%s)",
			  strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}
	sprintf(STATUS_MESSAGE, "pcm_buffer allocated size:%lu", pcm_buffer_len);

	unsigned int nchannels;
	if ((errno = snd_pcm_hw_params_get_channels(hw_params, &nchannels)) < 0) {
		sprintf (STATUS_MESSAGE, "Cannot get channels (%s)",
			  snd_strerror(errno));
		gtkWarning("%s", STATUS_MESSAGE);
		return errno;
	}

	for (int i = 0; i < 3; ++i) {
		sprintf(STATUS_MESSAGE, "START READING from audio interface");
		if ((errno = snd_pcm_readi(capture_handle, pcm_buffer, pcm_buffer_frames)) != pcm_buffer_frames) {
			sprintf (STATUS_MESSAGE, "read from audio interface failed (%s)",
				  snd_strerror(errno));
			gtkWarning("%s", STATUS_MESSAGE);
			return errno;
		}
		sprintf(STATUS_MESSAGE, "read %d done", i);
		reconResponse = recognize(acrConfig, pcm_buffer, pcm_buffer_len, nchannels, sample_rate);
		TRACE("ACRCloud response:%s", reconResponse);
		getAcrData(reconResponse, acrResponse);
		free(reconResponse);
		if (acrResponse->status.code[0] == '0') break;
	}
	free(pcm_buffer);
	sprintf(STATUS_MESSAGE, "pcm_buffer freed");
	return atoi(acrResponse->status.code);
}

/**
 * Close the audio interface device
 * @param snd_pcm_t * capture_handle: Capture handler structure
 * @return the status of the operation
 */
int closeDevice(snd_pcm_t * capture_handle)
{
	snd_pcm_close(capture_handle);
	sprintf(STATUS_MESSAGE, "audio interface closed");
	return EXIT_SUCCESS;
}
