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
#include "zhavam.h"
#include "zhavam_alsa.h"
#include "zhavam_errtra.h"


/**
 * "member" variable with the text names of the SND_PCM_FORMAT used
 */
static const char * zhv_alsa_snd_pcm_format_str[] = {
		"Signed 8 bit" ,
		"Unsigned 8 bit" ,
		"Signed 16 bit CPU endian",
		"Unsigned 16 bit CPU endian ",
		"Signed 24 bit CPU endian",
		"Unsigned 24 bit CPU endian",
		"Signed 32 bit CPU endian",
		"Unsigned 32 bit CPU endian",
		"Unknown"
};

/**
 * "member" variable with the values of the SND_PCM_FORMAT used sort as defined by zhv_snd_pcm_format_t
 */
static const int zhv_alsa_snd_pcm_format_int[] = {
		SND_PCM_FORMAT_S8 ,
		SND_PCM_FORMAT_U8 ,
		SND_PCM_FORMAT_S16,
		SND_PCM_FORMAT_U16,
		SND_PCM_FORMAT_S24,
		SND_PCM_FORMAT_U24,
		SND_PCM_FORMAT_S32,
		SND_PCM_FORMAT_U32,
		IND_SND_PCM_FORMAT_UNKNOWN
};

/**
 * get "method" to "member" variable zhv_snd_pcm_format_str
 * @returns the array with SND_PCM_FORMAT names
 */
char ** getZhvAlsaSndPcmFormatList(void)
{
	return zhv_alsa_snd_pcm_format_str;
}

/**
 * Looks for the SND_PCM_FORMAT enumerator supported by zhavam.
 * @param strSndPcmFormat: String to receive the SND_PCM_FORMAT name
 * @return the SND_PCM_FORMAT enumerator or SND_PCM_FORMAT_UNKNOWN if not found
 */
snd_pcm_format_t alsaSndPcmFormatDecode(const char * strSndPcmFormat)
{
	int i ;
	for(i = IND_SND_PCM_FORMAT_S8; i < IND_SND_PCM_FORMAT_LAST_ITEM; ++i)
		if (!strcmp(strSndPcmFormat, zhv_alsa_snd_pcm_format_str[i])) return i;
	return SND_PCM_FORMAT_UNKNOWN;
}

/**
 * Looks for the SND_PCM_FORMAT name supported by zhavam.
 * @param strSndPcmFormat: String to receive the SND_PCM_FORMAT name
 * @param sndPcmFormat: The enumerator for the sound pcm format
 * @return the SND_PCM_FORMAT name or "SND_PCM_FORMAT_S8" if not found
 */
char * alsaSndPcmFormatString(snd_pcm_format_t sndPcmFormat)
{
	int i;
	for(i = IND_SND_PCM_FORMAT_S8; i < IND_SND_PCM_FORMAT_LAST_ITEM; ++i)
		if (i == sndPcmFormat) return zhv_alsa_snd_pcm_format_str[i];
	return zhv_alsa_snd_pcm_format_str[i];
}

/**
 * Opening device tasks
 * 	snd_pcm_open
 * 	snd_pcm_hw_params_malloc
 * @param devID: The name of the device to open
 * @param ptr_capture_handle: Capture handler structure
 * @param ptr_hw_params: Pointer to the hardware params structure
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
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "audio device %s opened", devID);
	}
	if ((errno = snd_pcm_hw_params_malloc(ptr_hw_params)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot allocate hardware parameter structure (%s)",
				snd_strerror(errno));
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
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
 * @param capture_handle: Capture handler structure
 * @param hw_params: Pointer to the hardware params structure
 * @param format: Sound PCM format
 * @param rate: Recording rate
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
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params initialized");
	}

	if ((errno = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		sprintf (STATUS_MESSAGE, "cannot set access type (%s)",
			 snd_strerror(errno));
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params access has been set");
	}

	if ((errno = snd_pcm_hw_params_set_format(capture_handle, hw_params, format)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot set sample format (%s)",
			 snd_strerror(errno));
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params format has been set");
	}

	if ((errno = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, rate, 0)) < 0) {
		sprintf(STATUS_MESSAGE, "cannot set sample rate (%s)",
			 snd_strerror(errno));
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
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
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
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
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	else {
		sprintf(STATUS_MESSAGE, "hw_params have been set");
	}
	return errno;
}

/**
 * Device set up and PCM preparation
 * @param devID: The name of the device to open
 * @param capture_handle: Capture handler structure
 * @param hw_params: Pointer to the hardware params structure
 * @param format: Sound PCM format
 * @param rate: Recording rate
 * @return the status of the operations
 */
int setupAudioDevice(char * devID,
						snd_pcm_t * capture_handle,
						snd_pcm_hw_params_t * hw_params,
						snd_pcm_format_t format,
						unsigned int * rate)
{
	if ((errno = setupDevice(capture_handle, hw_params, format, rate)) < 0) {
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	snd_pcm_hw_params_free(hw_params);
	sprintf(STATUS_MESSAGE, "hw_params freed");
	if ((errno = pcmPrepare(capture_handle)) < 0) {
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	sprintf(STATUS_MESSAGE, "audio device ready");
	return errno;
}

/**
 * PCM prepare tasks
 * 	snd_pcm_prepare
 * @param capture_handle: Capture handler structure
 * @return the status of the operation
 */
int pcmPrepare(snd_pcm_t * capture_handle)
{
	if ((errno = snd_pcm_prepare(capture_handle)) < 0) {
		sprintf (STATUS_MESSAGE, "cannot prepare audio interface for use (%s)",
			   snd_strerror(errno));
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		ERROR("%s", STATUS_MESSAGE);
	}
	else {
		sprintf(STATUS_MESSAGE, "audio interface prepared");
	}
	return errno;
}

/**
 * Prints by console the content of the PCM Buffer
 * @param size: Size of the buffer
 * @param pcm_buffer. Pointer to the buffer
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
 * Writes the content of the PCM Buffer into a file called "grabacion.raw" in the zhavam home directory
 * @param size: Size of the buffer
 * @param pcm_buffer. Pointer to the buffer
 */
void writePcmBuffer(size_t nmemb,
					char * pcm_buffer)
{
	// Writes only if the check button at sound driver config section is set
	GtkWidget * pcmBufferWriteCheckButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pcmBufferWriteCheckButton"));
	if (!gtk_toggle_button_get_active ((GtkToggleButton *)pcmBufferWriteCheckButton)) return;

	char zhvGrabacionHome[2*ZHVHOMELEN];
	char home[ZHVHOMELEN];

	sprintf(home, "%s", getenv("HOME"));
	sprintf(zhvGrabacionHome, "%s/%s/%s", home, ZHVDIR, "grabacion.raw");

	FILE * file = fopen(zhvGrabacionHome, "wb");
	if (!file) {
		sprintf (STATUS_MESSAGE, "Cannot open %s for writing the pcm buffer. IO Error (%s)",
				zhvGrabacionHome, strerror(errno));
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		TRACE("%s", STATUS_MESSAGE);
		return;
	}
	fwrite(pcm_buffer, sizeof(char), nmemb, file);
	fclose(file);
}

/**
 * Starts recording the stream from the PCM and tries to recognize the recorded buffer using the service provided by ACRCloud
 * 	snd_pcm_hw_params_get_channels
 * 	snd_pcm_readi
 * 	acr_recognize_by_pcm
 * @param capture_handle: Capture handler structure
 * @param hw_params: Pointer to the hardware params structure
 * @param format: Sound PCM format
 * @param rate: Recording rate
 * @return the status of the operations
 */
int alsaStartRecord(snd_pcm_t * capture_handle,
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
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		//ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	sprintf(STATUS_MESSAGE, "pcm_buffer allocated size:%lu", pcm_buffer_len);
	unsigned int nchannels;
	if ((errno = snd_pcm_hw_params_get_channels(hw_params, &nchannels)) < 0) {
		sprintf (STATUS_MESSAGE, "Cannot get channels (%s)",
			  snd_strerror(errno));
		gtkSetCursor(NORMAL_CURSOR);
		gtkWarning("%s", STATUS_MESSAGE);
		//ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	sprintf(STATUS_MESSAGE, "START READING from audio interface...");
	gtkSetStatusZhvLabel(STATUS_MESSAGE);
	for (int i = 0; i < 3; ++i) {
		if ((errno = snd_pcm_readi(capture_handle, pcm_buffer, pcm_buffer_frames)) != pcm_buffer_frames) {
			sprintf (STATUS_MESSAGE, "read from audio interface failed (%s)",
				  snd_strerror(errno));
			gtkSetCursor(NORMAL_CURSOR);
			gtkWarning("%s", STATUS_MESSAGE);
			//ERROR("%s", STATUS_MESSAGE);
			return errno;
		}
		sprintf(STATUS_MESSAGE, "read %d done", i);

		/* FOR DEBUGGING PURPOSES */
		writePcmBuffer(pcm_buffer_len, pcm_buffer);
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "");
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "{\"status\":{\"msg\":\"No result\",\"code\":1001,\"version\":\"1.0\"}}");
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "{\"status\":{\"msg\":\"Success\",\"code\":0,\"version\":\"1.0\"},\"metadata\":{\"music\":[{\"external_ids\":{\"isrc\":\"GBAJH0600292\",\"upc\":\"0094636010359\"},\"play_offset_ms\":33340,\"external_metadata\":{\"spotify\":{\"album\":{\"name\":\"Violator\",\"id\":\"1v6DV6Bt0kDsX1Vd1f7CEe\"},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":\"762310PdDnwsDxAQxzQkfX\"}],\"track\":{\"name\":\"Enjoy The Silence - 2006 Digital Remaster\",\"id\":\"3enkvSCLKtGCCXfRyEK9Fl\"}},\"deezer\":{\"album\":{\"name\":\"The Best Of Depeche Mode Volume 1\",\"id\":86578},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":545}],\"genres\":[{\"id\":85}],\"track\":{\"name\":\"Enjoy The Silence (Remastered Version Original)\",\"id\":726176}}},\"acrid\":\"f9377e92e75d5dee3f0cd90a9c163f6a\",\"artists\":[{\"name\":\"Depeche Mode\"}],\"label\":\"(C) 2006 Depeche Mode under exclusive licence to Mute Records LtdThis label copy information is the subject of copyright protection. All rights reserved.(C) 2006 Mute Records Ltd\",\"release_date\":\"1990-03-19\",\"title\":\"Enjoy The Silence - 2006 Digital Remaster\",\"duration_ms\":372813,\"album\":{\"name\":\"Violator\"},\"result_from\":3,\"score\":82},{\"external_ids\":{\"isrc\":\"GBAJH0602198\",\"upc\":\"093624425663\"},\"play_offset_ms\":33440,\"release_date\":\"2006-11-14\",\"external_metadata\":{\"musicstory\":{\"album\":{\"id\":\"162576\"},\"release\":{\"id\":\"815068\"},\"track\":{\"id\":\"2206341\"}},\"deezer\":{\"album\":{\"name\":\"The Best Of Depeche Mode Volume 1\",\"id\":\"86578\"},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":\"545\"}],\"track\":{\"name\":\"Enjoy The Silence (Remastered Version) (Original)\",\"id\":\"726176\"}},\"spotify\":{\"album\":{\"name\":\"Classic Rock: Les Classiques de Marc Ysaye_90s00s\",\"id\":\"3fjD2coxF2SQwLRcjm0ctg\"},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":\"762310PdDnwsDxAQxzQkfX\"}],\"track\":{\"name\":\"Enjoy The Silence\",\"id\":\"6pznJ6pWLmxc69pAUVfgRq\"}},\"lyricfind\":{\"lfid\":\"001-9836867\"},\"youtube\":{\"vid\":\"aGSKrC7dGcY\"}},\"artists\":[{\"name\":\"Depeche Mode\"}],\"genres\":[{\"name\":\"Alternative\"}],\"title\":\"Enjoy The Silence (Remastered Version) (Original)\",\"label\":\"Sire//Reprise\",\"duration_ms\":372000,\"album\":{\"name\":\"The Best Of Depeche Mode Volume 1\"},\"acrid\":\"4ac1fdcab64947a971dee1163f3f2374\",\"result_from\":3,\"score\":100}],\"timestamp_utc\":\"2018-05-19 22:08:18\"},\"cost_time\":0.0060000419616699,\"result_type\":0}");
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "{\"status\":{\"msg\":\"Success\",\"code\":0,\"version\":\"1.0\"},\"metadata\":{\"music\":[{\"external_ids\":{\"isrc\":\"GBAHT8403350\",\"upc\":\"022924048364\"},\"play_offset_ms\":35560,\"title\":\"All I Need Is Everything\",\"external_metadata\":{\"musicstory\":{\"track\":{\"id\":\"424343\"}},\"youtube\":{\"vid\":\"0dbNUKy6QXM\"},\"spotify\":{\"album\":{\"name\":\"Knife\",\"id\":\"3eAbnzPwbYmbHdXQ9fmfXv\"},\"artists\":[{\"name\":\"Aztec Camera\",\"id\":\"7sbwBqdkynNUDgiWU3TQ5J\"}],\"track\":{\"name\":\"All I Need Is Everything\",\"id\":\"6DXL1O6MDN9kcb1yWbtDGK\"}},\"lyricfind\":{\"lfid\":\"001-4691140\"},\"deezer\":{\"album\":{\"name\":\"Knife\",\"id\":\"83860\"},\"artists\":[{\"name\":\"Aztec Camera\",\"id\":\"12940\"}],\"track\":{\"name\":\"All I Need Is Everything\",\"id\":\"698323\"}}},\"artists\":[{\"name\":\"Aztec Camera\"}],\"genres\":[{\"name\":\"Pop\"}],\"release_date\":\"1991-07-09\",\"label\":\"WM UK\",\"duration_ms\":343227,\"album\":{\"name\":\"Knife\"},\"acrid\":\"dbcb46dcdf7c79035a65713932e0668e\",\"result_from\":3,\"score\":100}],\"timestamp_utc\":\"2018-05-18 15:34:37\"},\"cost_time\":0.013999938964844,\"result_type\":0}");
		/* ********************** */

		reconResponse = recognize(acrConfig, pcm_buffer, pcm_buffer_len, nchannels, sample_rate);
		TRACE("ACRCloud response:%s", reconResponse);
		getAcrData(reconResponse, acrResponse);
		free(reconResponse);
		if (atoi(acrResponse->status.code) == 0) break;
	}
	free(pcm_buffer);
	sprintf(STATUS_MESSAGE, "pcm_buffer freed");
	return atoi(acrResponse->status.code);
}

/**
 * Close the audio interface device
 * @param capture_handle: Capture handler structure
 * @return the status of the operation
 */
int closeDevice(snd_pcm_t * capture_handle)
{
	snd_pcm_close(capture_handle);
	sprintf(STATUS_MESSAGE, "audio interface closed");
	return EXIT_SUCCESS;
}
