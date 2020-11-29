/*
 * zhavam_pulse.c
 *
 *  Created on: 23 dic. 2018
 *      Author: ipserc
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zhavam.h"
#include "zhavam_pulse.h"
#include "zhavam_errtra.h"


/**
 * "member" variable with the text names of the PA_SAMPLE used
 */
static const char * zhv_pa_sample_format_str[] = {
		"Unsigned 8 Bit PCM", 
		"8 Bit a-Law", 
		"8 Bit mu-Law", 
		"Signed 16 Bit PCM, little endian (PC)", 
		"Signed 16 Bit PCM, big endian", 
		"32 Bit IEEE floating point, little endian (PC), range -1.0 to 1.0", 
		"32 Bit IEEE floating point, big endian, range -1.0 to 1.0", 
		"Signed 32 Bit PCM, little endian (PC)", 
		"Signed 32 Bit PCM, big endian", 
		"Signed 24 Bit PCM packed, little endian (PC)", 
		"Signed 24 Bit PCM packed, big endian", 
		"Signed 24 Bit PCM in LSB of 32 Bit words, little endian (PC)", 
		"Signed 24 Bit PCM in LSB of 32 Bit words, big endian", 
		"Unknown sample format"
};

/**
 * "member" variable with the values of the PA_SAMPLE used sort as defined by zhv_snd_pcm_format_t
 */
static const int zhv_pa_sample_format_int[] = {
	IND_PA_SAMPLE_U8,
	IND_PA_SAMPLE_ALAW,
	IND_PA_SAMPLE_ULAW,
	IND_PA_SAMPLE_S16LE,
	IND_PA_SAMPLE_S16BE,
	IND_PA_SAMPLE_FLOAT32LE,
	IND_PA_SAMPLE_FLOAT32BE,
	IND_PA_SAMPLE_S32LE,
	IND_PA_SAMPLE_S32BE,
	IND_PA_SAMPLE_S24LE,
	IND_PA_SAMPLE_S24BE,
	IND_PA_SAMPLE_S24_32LE,
	IND_PA_SAMPLE_S24_32BE,
	IND_PA_SAMPLE_MAX,
	IND_PA_SAMPLE_INVALID
};

/**
 * get "method" to "member" variable zhv_snd_pcm_format_str
 * @returns the array with SND_PA_SAMPLE names
 */
const char ** getZhvPaSampleFormatStr(void)
{
	return zhv_pa_sample_format_str;
}

/**
 * Looks for the SND_PA_SAMPLE enumerator supported by zhavam.
 * @param strSndPaSample: String to receive the PA_SAMPLE name
 * @return the PA_SAMPLE enumerator or PA_SAMPLE_INVALID if not found
 */
pa_sample_format_t pulsePaSampleFormatDecode(const char * strPaSampleFormat)
{
	for(int i = IND_PA_SAMPLE_U8; i < IND_PA_SAMPLE_MAX; ++i )
		if (!strcmp(strPaSampleFormat, zhv_pa_sample_format_str[i])) return zhv_pa_sample_format_int[i];
	return PA_SAMPLE_INVALID;
}

/**
 * Looks for the PA_SAMPLE name supported by PulseAudio.
 * @param strSndPaSample: String to receive the PA_SAMPLE name
 * @param sndPcmFormat: The enumerator for the PA Sample Format
 * @return the PA_SAMPLE name or "An invalid value" if not found
 */
const char * pulsePaSampleFormatString(zhv_pa_sample_format_t paSampleFormat)
{
	if (paSampleFormat >= IND_PA_SAMPLE_U8 && paSampleFormat < IND_PA_SAMPLE_MAX) return zhv_pa_sample_format_str[paSampleFormat];
	else return zhv_pa_sample_format_str[IND_PA_SAMPLE_MAX];
}

/**
 * pa_sample_spec structure
 */
static pa_sample_spec paSampleSpec;

/**
 * Returns a pointer to the pa_sample_spec struct created by newPaSampleSpec()
 * @return the pointer to the new pa_sample_spec struct created
 */
pa_sample_spec * getPaSampleSpec(void)
{
	return &paSampleSpec;
}

/**
 * Returns the pulsePaSampleFormatStr from the UI or the zhavam configuration
 */
pa_sample_format_t getPulsePaSampleFormatStr(void)
{
	if (getGtkBuilder())
	{
		return pulsePaSampleFormatDecode(gtkGetPulsePaSampleFormatComboBoxText());
	}
	else
	{
		return getZhavamConf()->pulse.pa_sample_format;
	}
}

/**
 * Returns the pulseRate from the UI or the zhavam configuration
 */
uint32_t getPulseRate(void)
{
	if (getGtkBuilder())
	{
		return gtkGetPulseRateSpinButton();
	}
	else
	{
		return getZhavamConf()->pulse.rate;
	}
}

/**
 * Sets the values for the pa_sample_spec struct configured in the Pulse sound section
 * @return the pointer to the new pa_sample_spec struct created
 */
pa_sample_spec * setPaSampleSpec(void)
{
	pa_sample_spec * ptrPaSampleSpec = getPaSampleSpec(); 
	ptrPaSampleSpec->format = getPulsePaSampleFormatStr(); //pulsePaSampleFormatDecode(gtkGetPulsePaSampleFormatComboBoxText()); // PA_SAMPLE_S16LE;
	ptrPaSampleSpec->channels = 2;
	ptrPaSampleSpec->rate = getPulseRate(); //gtkGetPulseRateSpinButton(); // 44100;
	return ptrPaSampleSpec;
}

/**
 * pa_buffer_attr structure
 */
static pa_buffer_attr paBufferAttr;

/**
 * Returns a pointer to the pa_buffer_attr struct
 * @return the pointer to the new pa_buffer_attr struct created
 */
pa_buffer_attr * getPaBufferAttr(void)
{
	return &paBufferAttr;
}

/**
 * Returns the pulsePcmBufferFrames from the UI or the zhavam configuration
 */
unsigned int getPulsePcmBufferFrames() {
	if (getGtkBuilder())
	{
		return gtkGetPulsePcmBufferFramesSpinButton();
	}
	else
	{
		return getZhavamConf()->pulse.pcm_buffer_frames;
	}
}

/**
 * Sets the values for the pa_sample_spec struct configured in the Pulse sound section
 * @return the pointer to the new pa_sample_spec struct created
 */
pa_buffer_attr * setPaBufferAttr(void)
{
	pa_buffer_attr * ptrPaBufferAttr = getPaBufferAttr();

	ptrPaBufferAttr->maxlength = getPulsePcmBufferFrames() * pa_sample_size(getPaSampleSpec()) * pa_frame_size(getPaSampleSpec()) * 2; // I do not understand why it must have to be multiplied by 2
	ptrPaBufferAttr->fragsize = getPulsePcmBufferFrames() * pa_sample_size(getPaSampleSpec()) * pa_frame_size(getPaSampleSpec()) * 2; // I do not understand why it must have to be multiplied by 2
		
	if (ptrPaBufferAttr->maxlength < 1 || ptrPaBufferAttr->fragsize < 1) ptrPaBufferAttr = NULL; // Use default buffering attributes.

	return ptrPaBufferAttr;
}

/**
 * Connects to the Pulse Server. You need to get a valid connection before start recording
 */
pa_simple * pulseServerConnect(char * devID)
{
	zhavamConf_t * ptZhavamConf = getZhavamConf();
	int paErrno = 0;
	pa_simple * ptrPaSimple = NULL;
	
	TRACE("devID:%s", devID);

	pa_sample_spec * ptrPaSampleSpec = getPaSampleSpec();
	ptrPaSampleSpec = setPaSampleSpec();
	pa_buffer_attr * ptrPaBufferAttr = getPaBufferAttr();
	ptrPaBufferAttr = setPaBufferAttr();

	ptrPaSimple = pa_simple_new(NULL,					// Use the default server.
								ptZhavamConf->appName,	// Our application's name.
								PA_STREAM_RECORD,		// The direction of a pa_stream object	
								devID,					// Sink (resp. source) name, or NULL for default.
								SND_PA_STREAM_NAME,		// Description of our stream.
								ptrPaSampleSpec,		// Our sample format.
								NULL,					// Use default channel map.
								ptrPaBufferAttr,		// NULL pointer use the default buffering attributes.
								&paErrno				// Ignore error code.
							   );
	if (paErrno != PA_OK ) {
		sprintf(STATUS_MESSAGE, "pa_simple_new FAILED:%d - %s",paErrno, pa_strerror(paErrno));
		if (getGtkBuilder())
		{
			gtkSetCursor(NORMAL_CURSOR);
			gtkWarning("%s", STATUS_MESSAGE);
		}
		ERROR("%s", STATUS_MESSAGE);
		if (ptrPaSimple) pa_simple_free(ptrPaSimple);
	}  
	return ptrPaSimple;
}

/**
 * Starts recording the stream from the PCM and tries to recognize the recorded buffer using the service provided by ACRCloud
 * 	snd_pcm_hw_params_get_channels
 * 	snd_pcm_readi
 * 	acr_recognize_by_pcm
 * @param ptrPaSimple: pointer to pa_sample_spec struct
 * @param acrConfig: acrcloud_config structure
 * @param acrResponse: pointer to acr_data_t struct
 * @return the status of the recognition response
 */
int pulseStartRecord(pa_simple * ptrPaSimple,
	acrcloud_config acrConfig,
	acr_data_t * acrResponse)
{
	char * reconResponse;
 	int paErrno;
	char * buffer;
	pa_sample_spec * ptrPaSampleSpec = getPaSampleSpec();
	uint8_t nchannels = ptrPaSampleSpec->channels;
	uint32_t sample_rate = ptrPaSampleSpec->rate;
	pa_buffer_attr * ptrPaBufferAttr = getPaBufferAttr();
	size_t buffer_len = ptrPaBufferAttr->maxlength;

	if (!(buffer = malloc(buffer_len))) {
		sprintf (STATUS_MESSAGE, "memory allocation failed (%s)", strerror(errno));
		if (getGtkBuilder())
		{
			gtkSetCursor(NORMAL_CURSOR);
			gtkWarning("%s", STATUS_MESSAGE);
		}
		else ERROR("%s", STATUS_MESSAGE);
		return errno;
	}
	memset(buffer, 0, buffer_len);
	sprintf(STATUS_MESSAGE, STATUS05);
	if (getGtkBuilder()) gtkSetStatusZhvLabel(STATUS_MESSAGE);
	else TRACE("%s", STATUS_MESSAGE);
	for (int i = 1; i < 4; ++i) {
        /* Record some data ... */
        if (pa_simple_read(ptrPaSimple, buffer, ptrPaBufferAttr->maxlength, &paErrno) < 0) {
			sprintf(STATUS_MESSAGE, "pa_simple_read FAILED: %s", pa_strerror(paErrno));
			if (getGtkBuilder())
			{
				gtkSetCursor(NORMAL_CURSOR);
				gtkWarning("%s", STATUS_MESSAGE);
			}
			else ERROR("%s", STATUS_MESSAGE);
			if (ptrPaSimple) pa_simple_free(ptrPaSimple);
			return EXIT_FAILURE;
        }
        else
        {
        	sprintf(STATUS_MESSAGE, "read %d done", i);
			TRACE("%s", STATUS_MESSAGE);
        }
		/* FOR DEBUGGING PURPOSES */
		writePcmBuffer(buffer_len, buffer);
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "");
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "{\"status\":{\"msg\":\"No result\",\"code\":1001,\"version\":\"1.0\"}}");
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "{\"status\":{\"msg\":\"Success\",\"code\":0,\"version\":\"1.0\"},\"metadata\":{\"music\":[{\"external_ids\":{\"isrc\":\"GBAJH0600292\",\"upc\":\"0094636010359\"},\"play_offset_ms\":33340,\"external_metadata\":{\"spotify\":{\"album\":{\"name\":\"Violator\",\"id\":\"1v6DV6Bt0kDsX1Vd1f7CEe\"},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":\"762310PdDnwsDxAQxzQkfX\"}],\"track\":{\"name\":\"Enjoy The Silence - 2006 Digital Remaster\",\"id\":\"3enkvSCLKtGCCXfRyEK9Fl\"}},\"deezer\":{\"album\":{\"name\":\"The Best Of Depeche Mode Volume 1\",\"id\":86578},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":545}],\"genres\":[{\"id\":85}],\"track\":{\"name\":\"Enjoy The Silence (Remastered Version Original)\",\"id\":726176}}},\"acrid\":\"f9377e92e75d5dee3f0cd90a9c163f6a\",\"artists\":[{\"name\":\"Depeche Mode\"}],\"label\":\"(C) 2006 Depeche Mode under exclusive licence to Mute Records LtdThis label copy information is the subject of copyright protection. All rights reserved.(C) 2006 Mute Records Ltd\",\"release_date\":\"1990-03-19\",\"title\":\"Enjoy The Silence - 2006 Digital Remaster\",\"duration_ms\":372813,\"album\":{\"name\":\"Violator\"},\"result_from\":3,\"score\":82},{\"external_ids\":{\"isrc\":\"GBAJH0602198\",\"upc\":\"093624425663\"},\"play_offset_ms\":33440,\"release_date\":\"2006-11-14\",\"external_metadata\":{\"musicstory\":{\"album\":{\"id\":\"162576\"},\"release\":{\"id\":\"815068\"},\"track\":{\"id\":\"2206341\"}},\"deezer\":{\"album\":{\"name\":\"The Best Of Depeche Mode Volume 1\",\"id\":\"86578\"},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":\"545\"}],\"track\":{\"name\":\"Enjoy The Silence (Remastered Version) (Original)\",\"id\":\"726176\"}},\"spotify\":{\"album\":{\"name\":\"Classic Rock: Les Classiques de Marc Ysaye_90s00s\",\"id\":\"3fjD2coxF2SQwLRcjm0ctg\"},\"artists\":[{\"name\":\"Depeche Mode\",\"id\":\"762310PdDnwsDxAQxzQkfX\"}],\"track\":{\"name\":\"Enjoy The Silence\",\"id\":\"6pznJ6pWLmxc69pAUVfgRq\"}},\"lyricfind\":{\"lfid\":\"001-9836867\"},\"youtube\":{\"vid\":\"aGSKrC7dGcY\"}},\"artists\":[{\"name\":\"Depeche Mode\"}],\"genres\":[{\"name\":\"Alternative\"}],\"title\":\"Enjoy The Silence (Remastered Version) (Original)\",\"label\":\"Sire//Reprise\",\"duration_ms\":372000,\"album\":{\"name\":\"The Best Of Depeche Mode Volume 1\"},\"acrid\":\"4ac1fdcab64947a971dee1163f3f2374\",\"result_from\":3,\"score\":100}],\"timestamp_utc\":\"2018-05-19 22:08:18\"},\"cost_time\":0.0060000419616699,\"result_type\":0}");
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "{\"status\":{\"msg\":\"Success\",\"code\":0,\"version\":\"1.0\"},\"metadata\":{\"music\":[{\"external_ids\":{\"isrc\":\"GBAHT8403350\",\"upc\":\"022924048364\"},\"play_offset_ms\":35560,\"title\":\"All I Need Is Everything\",\"external_metadata\":{\"musicstory\":{\"track\":{\"id\":\"424343\"}},\"youtube\":{\"vid\":\"0dbNUKy6QXM\"},\"spotify\":{\"album\":{\"name\":\"Knife\",\"id\":\"3eAbnzPwbYmbHdXQ9fmfXv\"},\"artists\":[{\"name\":\"Aztec Camera\",\"id\":\"7sbwBqdkynNUDgiWU3TQ5J\"}],\"track\":{\"name\":\"All I Need Is Everything\",\"id\":\"6DXL1O6MDN9kcb1yWbtDGK\"}},\"lyricfind\":{\"lfid\":\"001-4691140\"},\"deezer\":{\"album\":{\"name\":\"Knife\",\"id\":\"83860\"},\"artists\":[{\"name\":\"Aztec Camera\",\"id\":\"12940\"}],\"track\":{\"name\":\"All I Need Is Everything\",\"id\":\"698323\"}}},\"artists\":[{\"name\":\"Aztec Camera\"}],\"genres\":[{\"name\":\"Pop\"}],\"release_date\":\"1991-07-09\",\"label\":\"WM UK\",\"duration_ms\":343227,\"album\":{\"name\":\"Knife\"},\"acrid\":\"dbcb46dcdf7c79035a65713932e0668e\",\"result_from\":3,\"score\":100}],\"timestamp_utc\":\"2018-05-18 15:34:37\"},\"cost_time\":0.013999938964844,\"result_type\":0}");
		// ---- reconResponse = malloc(5000); sprintf( reconResponse, "{\"status\":{\"msg\":\"Success\",\"code\":0,\"version\":\"1.0\"},\"metadata\":{\"music\":[{\"external_ids\":{\"isrc\":\"GBAHT8403350\",\"upc\":\"022924048364\"},\"play_offset_ms\":35560,\"title\":\"All I Need Is Everything\",\"external_metadata\":{\"musicstory\":{\"track\":{\"id\":\"424343\"}},\"youtube\":{\"vid\":\"0dbNUKy6QXM\"},\"spotify\":{\"album\":{\"name\":\"Knife\",\"id\":\"3eAbnzPwbYmbHdXQ9fmfXv\"},\"artists\":[{\"name\":\"Aztec Camera\",\"id\":\"7sbwBqdkynNUDgiWU3TQ5J\"}],\"track\":{\"name\":\"All I Need Is Everything\",\"id\":\"6DXL1O6MDN9kcb1yWbtDGK\"}},\"lyricfind\":{\"lfid\":\"001-4691140\"},\"deezer\":{\"album\":{\"name\":\"Knife\",\"id\":\"83860\"},\"artists\":[{\"name\":\"Aztec Camera\",\"id\":\"12940\"}],\"track\":{\"name\":\"All I Need Is Everything\",\"id\":\"698323\"}}},\"artists\":[{\"name\":\"Aztec Camera\"}],\"genres\":[{\"name\":\"Pop\"}],\"release_date\":\"1991-07-09\",\"label\":\"WM UK\",\"duration_ms\":343227,\"album\":{\"name\":\"Knife\"},\"acrid\":\"dbcb46dcdf7c79035a65713932e0668e\",\"result_from\":3,\"score\":100}],\"timestamp_utc\":\"2018-05-18 15:34:37\"},\"cost_time\":0.013999938964844,\"result_type\":0}");
		// ---- reconResponse = malloc(5000); sprintf( "{\"metadata\":{\"timestamp_utc\":\"2020-01-25 10:59:04\",\"music\":[{\"play_offset_ms\":335380,\"artists\":[{\"name\":\"Tears For Fears\"}],\"lyrics\":{\"copyrights\":[\"BMG Rights Management\"]},\"acrid\":\"bec7e81617c39bae5412311d1576723c\",\"genres\":[{\"name\":\"Pop\"},{\"name\":\"Rock\"}],\"album\":{\"name\":\"Guilty Pleasures\"},\"label\":\"Universal Music\",\"external_ids\":{\"isrc\":\"GBF088990128\",\"upc\":\"600753051122\"},\"result_from\":3,\"contributors\":{\"lyricists\":[\"CURT SMITH\",\"ROLAND ORZABAL\"]},\"title\":\"Sowing The Seeds Of Love\",\"duration_ms\":378173,\"score\":100,\"external_metadata\":{\"deezer\":{\"track\":{\"name\":\"Sowing The Seeds Of Love\",\"id\":\"908434\"},\"artists\":[{\"name\":\"Tears for Fears\",\"id\":\"1192\"}],\"album\":{\"name\":\"Guilty Pleasures\",\"id\":\"102529\"}},\"spotify\":{\"track\":{\"name\":\"Sowing The Seeds Of Love\",\"id\":\"0Mri4DItrIor5yN8nj7DRw\"},\"artists\":[{\"name\":\"Tears For Fears\",\"id\":\"4bthk9UfsYUYdcFyqxmSUU\"}],\"album\":{\"name\":\"De 100 Største Pop Hits (Vol. 2)\",\"id\":\"33UVNuGJNIPbe8IPpFcmHJ\"}},\"lyricfind\":{\"lfid\":\"001-2046491\"},\"youtube\":{\"vid\":\"VAtGOESO7W8\"},\"musicstory\":{\"track\":{\"id\":\"31854\"},\"release\":{\"id\":\"1113708\"},\"album\":{\"id\":\"400478\"}}},\"release_date\":\"2007-01-01\"}]},\"cost_time\":0.049000024795532,\"status\":{\"msg\":\"Success\",\"version\":\"1.0\",\"code\":0},\"result_type\":0}");
		/* ********************** */

		reconResponse = recognize(acrConfig, (char *)buffer, buffer_len, nchannels, sample_rate);
		TRACE("ACRCloud response:%s", reconResponse);
		getAcrData(reconResponse, acrResponse);
		free(reconResponse);
		if (atoi(acrResponse->status.code) == 0) break;
	}
	if (ptrPaSimple) pa_simple_free(ptrPaSimple);
	free(buffer);
	sprintf(STATUS_MESSAGE, "pcm_buffer freed");
	TRACE("%s", STATUS_MESSAGE);
	sprintf(STATUS_MESSAGE, STATUS02);
	if (getGtkBuilder())
	{
		gtkSetCursor(NORMAL_CURSOR);
		gtkSetStatusZhvLabel(STATUS_MESSAGE);
	}
	return atoi(acrResponse->status.code);
}
