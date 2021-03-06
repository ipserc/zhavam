/*
 * zhavam_acrcloud.h
 *
 *  Created on: 19 ene. 2018
 *      Author: ipserc
 */

#include "acrcloud_recognizer.h"

#ifndef SOURCE_ZHAVAN_ARCCLOUD_H_
#define SOURCE_ZHAVAN_ARCCLOUD_H_

/**
 * zhv_acr_rec_type enumerator
 */
typedef enum _zhv_acr_rec_type
{
	ind_acr_opt_rec_unknown = -1,
	ind_acr_opt_rec_audio,
	ind_acr_opt_rec_humming,
	ind_acr_opt_rec_both,
	ind_last_acr_opt_rec
} zhv_acr_rec_t;

/*
 * Prototypes
 */
/* zhavam_acrcloud.c */
const char **getZhvAcrOptRecStr(void);
zhv_acr_rec_t recTypeDecode(const char *recTypeString);
const char *recTypeString(zhv_acr_rec_t acrcloud_rec_type);
char *recognize(acrcloud_config acrConfig, char *pcm_buffer, int pcm_buffer_len, int nchannels, int sample_rate);

#endif /* SOURCE_ZHAVAN_ARCCLOUD_H_ */
