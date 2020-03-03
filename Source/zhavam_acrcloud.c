/*
 * zhavan_arccloud.c
 *
 *  Created on: 19 ene. 2018
 *      Author: ipserc
 */

#include <string.h>
#include "zhavam_acrcloud.h"
#include "acrcloud_recognizer.h"
#include "zhavam_errtra.h"

/**
 * String array with the names for acr_opt_rec
 */
static const char * zhv_acr_opt_rec_str[] = {
		"acr_opt_rec_audio",
		"acr_opt_rec_humming",
		"acr_opt_rec_both"
};

/**
 * Array with the indexes for getting the strings from zhv_acr_opt_rec_str
 * A pseudo-dictionary
 */
static const int zhv_acr_opt_rec_int[] = {
		ind_acr_opt_rec_audio,
		ind_acr_opt_rec_humming,
		ind_acr_opt_rec_both
};

/**
 * method to get zhv_acr_opt_rec_str array
 */
const char ** getZhvAcrOptRecStr(void)
{
	return zhv_acr_opt_rec_str;
}

/**
 * Returns the zhv_acr_opt_rec_int from a given zhv_acr_opt_rec_str
 * @param recTypeString: The zhv_acr_opt_rec_str string
 * @return the zhv_acr_opt_rec_int associated
 */
zhv_acr_rec_t recTypeDecode(const char * recTypeString)
{
	for (int i = ind_acr_opt_rec_audio; i < ind_last_acr_opt_rec; ++i)
	{
		if (!strcmp(recTypeString, zhv_acr_opt_rec_str[i])) return i;
	}
	return ind_acr_opt_rec_unknown;
}

/**
 * Returns the zhv_acr_opt_rec_str from a given zhv_acr_opt_rec_int
 * @param acrcloud_rec_type_str: A pointer to the string in which the zhv_acr_opt_rec_str will be copied
 * @param acrcloud_rec_type: The given given zhv_acr_opt_rec_int value
 * @return a pointer to the string with the zhv_acr_opt_rec_str value copied
 */
const char * recTypeString(zhv_acr_rec_t acrcloud_rec_type)
{
	if (acrcloud_rec_type < 0 || acrcloud_rec_type >= ind_last_acr_opt_rec) return zhv_acr_opt_rec_str[0];
	else return zhv_acr_opt_rec_str[acrcloud_rec_type];
}

/**
 * Does the song's snippet recognition by calling the ACR CLOUD service
 * @param acrConfig acrcloud_config: Configuration to call the service
 * @param pcm_buffer char*
 * @param pcm_buffer_len int
 * @param nchannels int
 * @param sample_rate int
 */
char * recognize(acrcloud_config acrConfig,
				char * pcm_buffer,
				int pcm_buffer_len,
				int nchannels,
				int sample_rate)
{
	char * result = NULL;
	int result_len = 0;

	acr_recognize_by_pcm(acrConfig, pcm_buffer, pcm_buffer_len, nchannels, sample_rate, &result, &result_len);
	//if (result != NULL) { printf("result: %s\n", result);}

	// Resample
	/*char *out_pcm_buffer = NULL;
	int out_pcm_buffer_len = 0;
	acr_resample_to_8k_pcm(pcm_buffer, nres, nchannels, sample_rate, &out_pcm_buffer, &out_pcm_buffer_len);
	printf("%d\n", out_pcm_buffer_len);*/


	// Create audio fingerprint;
	/*char *audio_fp = NULL;
	int audio_fp_len = 0;
	acr_create_audio_fingerprint_by_pcm(pcm_buffer, nres, nchannels, sample_rate, &audio_fp, &audio_fp_len);
	if (audio_fp != NULL) {
		printf("fp len: %d\n", audio_fp_len);
		acr_free(audio_fp);
	}*/

	// Create humming fingerprint
	/*char *humming_fp = NULL;
	int humming_fp_len = 0;
	acr_create_audio_fingerprint_by_pcm(pcm_buffer, nres, nchannels, sample_rate, &humming_fp, &humming_fp_len);
	if (audio_fp != NULL) {
		printf("fp len: %d\n", humming_fp_len);
		acr_free(humming_fp);
	}*/
	return result;
}
