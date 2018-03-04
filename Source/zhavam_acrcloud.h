/*
 * zhavam_acrcloud.h
 *
 *  Created on: 19 ene. 2018
 *      Author: ipserc
 */

#include "acrcloud_recognizer.h"

#ifndef SOURCE_ZHAVAN_ARCCLOUD_H_
#define SOURCE_ZHAVAN_ARCCLOUD_H_

#define VALUE_MAX_LEN 128 // Maximum length of the value field
#define MAX_ITEMS 10	// Maximum number of item in a list

/**
 * zhv_acr_rec_type enumerator
 */
typedef enum _zhv_acr_rec_type
{
	ind_acr_opt_rec_audio,
	ind_acr_opt_rec_humming,
	ind_acr_opt_rec_both,
	ind_last_acr_opt_rec
} zhv_acr_rec_t;

/**
 * status definition section
 */
typedef struct
{
	char msg[VALUE_MAX_LEN];
	char code[VALUE_MAX_LEN];
	char version[VALUE_MAX_LEN];
} status_t;

/**
 * music structure
 */
typedef struct
{
	char external_ids[VALUE_MAX_LEN];
	char play_offset_ms[VALUE_MAX_LEN];
	char external_metadata[VALUE_MAX_LEN];
	char artists[MAX_ITEMS][VALUE_MAX_LEN];
	char genres[MAX_ITEMS][VALUE_MAX_LEN];
	char title[VALUE_MAX_LEN];
	char release_date[VALUE_MAX_LEN];
	char label[VALUE_MAX_LEN];
	char duration_ms[VALUE_MAX_LEN];
	char album[VALUE_MAX_LEN];
	char acrid[VALUE_MAX_LEN];
	char result_from[VALUE_MAX_LEN];
	char score[VALUE_MAX_LEN];
} music_t;

/**
 * metadata structure
 */
typedef struct
{
	music_t music[MAX_ITEMS];
	char timestamp_utc[VALUE_MAX_LEN];
} metadata_t;

/**
 * data structure
 */
typedef struct
{
	status_t status;
	metadata_t metadata;
	char cost_time[VALUE_MAX_LEN];
	char result_type[VALUE_MAX_LEN];
} acr_data_t;

/*
 * Prototypes
 */
char *getZhvAcrOptRecStr(void);
ACRCLOUD_OPT_REC_TYPE recTypeDecode(char *recTypeString);
char *recTypeString(char *acrcloud_rec_type_str, ACRCLOUD_OPT_REC_TYPE acrcloud_rec_type);
char *recognize(acrcloud_config acrConfig, char *pcm_buffer, int pcm_buffer_len, int nchannels, int sample_rate);

#endif /* SOURCE_ZHAVAN_ARCCLOUD_H_ */
