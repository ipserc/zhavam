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
#include "zhavam_pulse.h"
#include "zhavam_devices.h"

typedef enum
{
	ALSA,
	PULSE,
	LAST_CONTROLLER,
	UNKNOWN_CONTROLLER = -1
} driverController_t;

/**
 * ZhavamConf structure
 */
typedef struct {
	char appName[256];
	acrcloud_config acrcloud;
	driverController_t driverController;
	alsa_config_t alsa;
	pulse_config_t pulse;
} zhavamConf_t;

/*
 * prototypes
 */
/* zhavam_config.c */
zhavamConf_t *newZhavamConf(void);
zhavamConf_t *getZhavamConf(void);
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
void gtkSetDriverControllerNotebookPage(zhavamConf_t * ptZhavamConf);
void gtkConfigDialogSetUp(zhavamConf_t *ptZhavamConf);
void gtkZhavamConfigReload(GtkImageMenuItem *menuConfigReload, gpointer user_data);
void gtkZhavamConfigSave(GtkImageMenuItem *menuConfigSave, gpointer user_data);
void gtkSetPulsePaSampleFormatComboBoxText(zhavamConf_t *ptZhavamConf);
char *gtkGetPulsePaSampleFormatComboBoxText(void);
void gtkSetPulseRateSpinButton(zhavamConf_t *ptZhavamConf);
int gtkGetPulseRateSpinButton(void);
void gtkSetPulsePcmBufferFramesSpinButton(zhavamConf_t *ptZhavamConf);
int gtkGetPulsePcmBufferFramesSpinButton(void);
void gtkSetPulsePcmDeviceComboBoxText(list_t *pcmRecDevList, zhavamConf_t *ptZhavamConf);
char *gtkGetPulsePcmDeviceComboBoxText(void);
char **getZhvDriverControllerList(void);
void gtkSetDefaultDriverController(GtkComboBoxText *driverControllerComboBoxText, zhavamConf_t *ptZhavamConf);
void gtkSetDriverControllerEntry(zhavamConf_t *ptZhavamConf);
void gtkSetDriverControllerComboBoxText(zhavamConf_t *ptZhavamConf);
char *gtkGetdriverControllerComboBoxText(void);
driverController_t driverControllerDecode(const char *driverControllerStr);
char *driverControllerString(driverController_t driverController);
void printZhavamConf(zhavamConf_t *ptZhavamConf);

#endif /* SOURCE_ZHAVAM_CONFIG_H_ */
