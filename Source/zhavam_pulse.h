/*
 * zhavam_pulse.h
 *
 *  Created on: 23 dic. 2018
 *      Author: ipserc
 */

#include <pulse/pulseaudio.h>
#include <pulse/sample.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#include <errno.h>
#include <stddef.h>

#include "zhavam_acrcloud.h"
#include "acrcloud_recognizer.h"
#include "zhavam_jsonparser.h"

#ifndef SOURCE_ZHAVAM_PULSE_H_
#define SOURCE_ZHAVAM_PULSE_H_

extern char STATUS_MESSAGE[];
extern int errno;

#define SND_PA_STREAM_NAME "zhavam rec stream"

#define RECORD_RETRIES 0

/**
 * PA SAMPLE enumerator
 */
typedef enum _zhv_pa_sample_format
{
	IND_PA_SAMPLE_U8 = PA_SAMPLE_U8,
	IND_PA_SAMPLE_ALAW = PA_SAMPLE_ALAW,
	IND_PA_SAMPLE_ULAW = PA_SAMPLE_ULAW,
	IND_PA_SAMPLE_S16LE = PA_SAMPLE_S16LE,
	IND_PA_SAMPLE_S16BE = PA_SAMPLE_S16BE,
	IND_PA_SAMPLE_FLOAT32LE = PA_SAMPLE_FLOAT32LE,
	IND_PA_SAMPLE_FLOAT32BE = PA_SAMPLE_FLOAT32BE,
	IND_PA_SAMPLE_S32LE = PA_SAMPLE_S32LE,
	IND_PA_SAMPLE_S32BE = PA_SAMPLE_S32BE,
	IND_PA_SAMPLE_S24LE = PA_SAMPLE_S24LE,
	IND_PA_SAMPLE_S24BE = PA_SAMPLE_S24BE,
	IND_PA_SAMPLE_S24_32LE = PA_SAMPLE_S24_32LE,
	IND_PA_SAMPLE_S24_32BE = PA_SAMPLE_S24_32BE,
	IND_PA_SAMPLE_MAX = PA_SAMPLE_MAX,
	IND_PA_SAMPLE_INVALID = PA_SAMPLE_INVALID
} zhv_pa_sample_format_t;

/**
 * PULSE AUDIO configuration parameters structure
 */
typedef struct
{
	pa_sample_format_t pa_sample_format;
	unsigned int rate;
	unsigned int pcm_buffer_frames;
	char * pcm_dev;
} pulse_config_t;

/*
 * prototypes
 */
/* zhavam_pulse.c */
const char **getZhvPaSampleFormatStr(void);
pa_sample_format_t pulsePaSampleFormatDecode(const char *strPaSampleFormat);
const char *pulsePaSampleFormatString(zhv_pa_sample_format_t paSampleFormat);
pa_sample_spec *getPaSampleSpec(void);
pa_sample_format_t getPulsePaSampleFormatStr(void);
uint32_t getPulseRate(void);
pa_sample_spec *setPaSampleSpec(void);
pa_buffer_attr *getPaBufferAttr(void);
unsigned int getPulsePcmBufferFrames(void);
pa_buffer_attr *setPaBufferAttr(void);
pa_simple *pulseServerConnect(char *devID);
int pulseStartRecord(pa_simple *ptrPaSimple, acrcloud_config acrConfig, acr_data_t *acrResponse);

#endif /* SOURCE_ZHAVAM_PULSE_H_ */
