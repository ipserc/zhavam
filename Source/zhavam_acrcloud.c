/*
 * zhavan_arccloud.c
 *
 *  Created on: 19 ene. 2018
 *      Author: ipserc
 */

#include <string.h>
#include "zhavam_acrcloud.h"
#include "acrcloud_recognizer.h"

/**
 * String array with the names for acr_opt_rec
 */
char * zhv_acr_opt_rec_str[] = {
		"acr_opt_rec_audio",
		"acr_opt_rec_humming",
		"acr_opt_rec_both"
};

/**
 * Array with the indexes for getting the strings from zhv_acr_opt_rec_str
 * A pseudo-dictionary
 */
int zhv_acr_opt_rec_int[] = {
		acr_opt_rec_audio,
		acr_opt_rec_humming,
		acr_opt_rec_both
};

/**
 * method to get zhv_acr_opt_rec_str array
 */
char ** getZhvAcrOptRecStr(void)
{
	return zhv_acr_opt_rec_str;
}

/**
 * Returns the zhv_acr_opt_rec_int from a given zhv_acr_opt_rec_str
 * @param recTypeString: The zhv_acr_opt_rec_str string
 * @return the zhv_acr_opt_rec_int associated
 */
ACRCLOUD_OPT_REC_TYPE recTypeDecode(const char * recTypeString)
{
	if (!strcmp(recTypeString, "acr_opt_rec_audio")) return acr_opt_rec_audio;
	if (!strcmp(recTypeString, "acr_opt_rec_humming")) return acr_opt_rec_humming;
	if (!strcmp(recTypeString, "acr_opt_rec_both")) return acr_opt_rec_both;
	return -1;
}

/**
 * Returns the zhv_acr_opt_rec_str from a given zhv_acr_opt_rec_int
 * #param acrcloud_rec_type_str: A pointer to the string in which the zhv_acr_opt_rec_str will be copied
 * @param acrcloud_rec_type: The given given zhv_acr_opt_rec_int value
 * @return a pointer to the string with the zhv_acr_opt_rec_str value copied
 */
char * recTypeString(char * acrcloud_rec_type_str, ACRCLOUD_OPT_REC_TYPE acrcloud_rec_type)
{
	switch (acrcloud_rec_type)
	{
	case acr_opt_rec_audio:   acrcloud_rec_type_str = strcpy(acrcloud_rec_type_str, "acr_opt_rec_audio"); break;
	case acr_opt_rec_humming: acrcloud_rec_type_str = strcpy(acrcloud_rec_type_str, "acr_opt_rec_humming"); break;
	case acr_opt_rec_both:    acrcloud_rec_type_str = strcpy(acrcloud_rec_type_str, "acr_opt_rec_both"); break;
	default: acrcloud_rec_type_str = strcpy(acrcloud_rec_type_str, "acr_opt_rec_audio");
	}
	return acrcloud_rec_type_str;
}

/**
 * Does the song's snippet recognition by calling the ACR CLOUD service
 * acrConfig: Configuration to call the service
 * pcm_buffer,
 * pcm_buffer_len,
 * nchannels,
 * sample_rate
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
