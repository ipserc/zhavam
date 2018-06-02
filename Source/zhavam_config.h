/*
 * zhavam_config.h
 *
 *  Created on: 13 feb. 2018
 *      Author: ipserc
 */


#ifndef SOURCE_ZHAVAM_CONFIG_H_
#define SOURCE_ZHAVAM_CONFIG_H_

#include <gtk/gtk.h>
#include "zhavam_alsa.h"
#include "zhavam_devices.h"

/**
 * ZhavamConf structure
 */
typedef struct {
	acrcloud_config acrcloud;
	alsa_config alsa;
} zhavamConf_t;

void gtkSetAcrCloudHostEntry(zhavamConf_t *ptZhavamConf);
const char *gtkGetAcrCloudHostEntry(void);
void gtkSetAcrCloudAccessKeyEntry(zhavamConf_t *ptZhavamConf);
const char *gtkGetAcrCloudAccessKeyEntry(void);
void gtkSetAcrCloudAccessSecretEntry(zhavamConf_t *ptZhavamConf);
const char *gtkGetAcrCloudAccessSecretEntry(void);
void gtkSetAcrCloudTimeOutSpinButton(zhavamConf_t *ptZhavamConf);
int gtkGetAcrCloudTimeOutSpinButton(void);
void gtkSetAcrCloudRecTypeComboBoxText(zhavamConf_t *ptZhavamConf);
char *gtkGetAcrCloudRecTypeComboBoxText(void);
void gtkSetAlsaSndPcmFormatComboBoxText(zhavamConf_t *ptZhavamConf);
char *gtkGetAlsaSndPcmFormatComboBoxText(void);
void gtkSetAlsaRateSpinButton(zhavamConf_t *ptZhavamConf);
int gtkGetAlsaRateSpinButton(void);
void gtkSetAlsaPcmBufferFramesSpinButton(zhavamConf_t *ptZhavamConf);
int gtkGetAlsaPcmBufferFramesSpinButton(void);
void gtkSetAlsaPcmDeviceComboBoxText(list_t *pcmRecDevList, zhavamConf_t *ptZhavamConf);
char *gtkGetAlsaPcmDeviceComboBoxText(void);
void gtkZhavamConfigEdit(GtkImageMenuItem *menuConfigEdit, gpointer user_data);
void gtkConfigCloseButton(GtkButton *configCloseButton, gpointer user_data);
void configUpdate(zhavamConf_t *ptZhavamConf);
void gtkConfigSaveButton(GtkButton *configCloseButton, gpointer user_data);
void gtkConfigCleanButton(GtkButton *configCloseButton, gpointer user_data);
void gtkConfigDialogSetUp(zhavamConf_t *ptZhavamConf, list_t *pcmRecDevList);
void gtkZhavamConfigReload(GtkImageMenuItem *menuConfigReload, gpointer user_data);
void gtkZhavamConfigSave(GtkImageMenuItem *menuConfigSave, gpointer user_data);

#endif /* SOURCE_ZHAVAM_CONFIG_H_ */
