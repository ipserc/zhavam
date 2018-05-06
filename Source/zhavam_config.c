/*
 * zhavam_config.c
 *
 *  Created on: 13 feb. 2018
 *      Author: ipserc
 */

#include <gtk/gtk.h>

#include <stdio.h>

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
 * Sets AcrCloudHostEntry field with ptZhavamConf->acrcloud.host_
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAcrCloudHostEntry(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudHostEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudHostEntry"));
	if (ptZhavamConf->acrcloud.host_) gtk_entry_set_text(acrCloudHostEntry, ptZhavamConf->acrcloud.host_);
	else gtk_entry_set_text(acrCloudHostEntry, "");
}

/**
 * Gets AcrCloudHostEntry field
 * @return AcrCloudHostEntry value
 */
char * gtkGetAcrCloudHostEntry(void)
{
	GtkWidget * acrCloudHostEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudHostEntry"));
	return gtk_entry_get_text(acrCloudHostEntry);
}

/**
 * Sets AcrCloudAccessKeyEntry field with ptZhavamConf->acrcloud.access_key_
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAcrCloudAccessKeyEntry(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudAccessKeyEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessKeyEntry"));
	if (ptZhavamConf->acrcloud.access_key_) gtk_entry_set_text(acrCloudAccessKeyEntry, ptZhavamConf->acrcloud.access_key_);
	else gtk_entry_set_text(acrCloudAccessKeyEntry, "");
}

/**
 * Gets AcrCloudAccessKeyEntry field
 * @return AcrCloudAccessKeyEntry value
 */
char * gtkGetAcrCloudAccessKeyEntry(void)
{
	GtkWidget * acrCloudAccessKeyEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessKeyEntry"));
	return gtk_entry_get_text(acrCloudAccessKeyEntry);
}

/**
 * Sets AcrCloudAccessSecretEntry field with ptZhavamConf->acrcloud.access_secret_
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAcrCloudAccessSecretEntry(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudAccessSecretEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessSecretEntry"));
	if (ptZhavamConf->acrcloud.access_secret_) gtk_entry_set_text(acrCloudAccessSecretEntry, ptZhavamConf->acrcloud.access_secret_);
	else gtk_entry_set_text(acrCloudAccessSecretEntry, "");
}

/**
 * Gets AcrCloudAccessSecretEntry field
 * @return AcrCloudAccessSecretEntry value
 */
char * gtkGetAcrCloudAccessSecretEntry(void)
{
	GtkWidget * acrCloudAccessSecretEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessSecretEntry"));
	return gtk_entry_get_text(acrCloudAccessSecretEntry);
}

/**
 * Sets AcrCloudTimeOutSpinButton value with ptZhavamConf->acrcloud.timeout_ms_
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAcrCloudTimeOutSpinButton(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudTimeOutSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudTimeOutSpinButton"));
	gtk_spin_button_set_value((GtkSpinButton *)acrCloudTimeOutSpinButton, (gdouble)ptZhavamConf->acrcloud.timeout_ms_);
}

/**
 * Gets AcrCloudTimeOutSpinButton field
 * @return AcrCloudTimeOutSpinButton value
 */
int gtkGetAcrCloudTimeOutSpinButton(void)
{
	GtkWidget * acrCloudTimeOutSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudTimeOutSpinButton"));
	return gtk_spin_button_get_value_as_int(acrCloudTimeOutSpinButton);
}

/**
 * Sets AcrCloudRecTypeComboBoxText field with ptZhavamConf->acrcloud.rec_type_
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAcrCloudRecTypeComboBoxText(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudRecTypeComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudRecTypeComboBoxText"));

	gtk_combo_box_text_remove_all((GtkComboBoxText *)acrCloudRecTypeComboBoxText);

	char ** zhv_acr_opt_rec_str = getZhvAcrOptRecStr();
	for (int i = ind_acr_opt_rec_audio; i < ind_last_acr_opt_rec; ++i)
		gtk_combo_box_text_append_text((GtkComboBoxText *)acrCloudRecTypeComboBoxText, zhv_acr_opt_rec_str[i]);
	gtk_combo_box_set_active((GtkComboBox *)acrCloudRecTypeComboBoxText, ptZhavamConf->acrcloud.rec_type_);
}

/**
 * Gets AcrCloudRecTypeComboBoxText field
 * @return AcrCloudRecTypeComboBoxText value
 */
char * gtkGetAcrCloudRecTypeComboBoxText(void)
{
	GtkWidget * acrCloudRecTypeComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudRecTypeComboBoxText"));
	return gtk_combo_box_text_get_active_text((GtkComboBox *)acrCloudRecTypeComboBoxText);
}

/**
 * Sets AlsaSndPcmFormatComboBoxText field with ptZhavamConf->alsa.snd_pcm_format
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAlsaSndPcmFormatComboBoxText(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * alsaSndPcmFormatComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaSndPcmFormatComboBoxText"));

	gtk_combo_box_text_remove_all((GtkComboBoxText *)alsaSndPcmFormatComboBoxText);

	char ** zhv_snd_pcm_format_str = getZhvSndPcmFormatStr();
	for (zhv_snd_pcm_format_t i = IND_PCM_FORMAT_S8; i < IND_LAST_PCM_FORMAT; ++i)
		gtk_combo_box_text_append_text((GtkComboBoxText *)alsaSndPcmFormatComboBoxText, zhv_snd_pcm_format_str[i]);
	gtk_combo_box_set_active((GtkComboBox *)alsaSndPcmFormatComboBoxText, ptZhavamConf->alsa.snd_pcm_format);
}

/**
 * Gets AlsaSndPcmFormatComboBoxText field
 * @return AlsaSndPcmFormatComboBoxText value
 */
char * gtkGetAlsaSndPcmFormatComboBoxText(void)
{
	GtkWidget * alsaSndPcmFormatComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaSndPcmFormatComboBoxText"));
	return gtk_combo_box_text_get_active_text((GtkComboBox *)alsaSndPcmFormatComboBoxText);
}

/**
 * Sets AlsaRateSpinButton value with ptZhavamConf->alsa.rate
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAlsaRateSpinButton(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * alsaRateSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaRateSpinButton"));
	gtk_spin_button_set_value((GtkSpinButton *)alsaRateSpinButton,(gdouble) ptZhavamConf->alsa.rate);
}

/**
 * Gets AlsaRateSpinButton field
 * @return AlsaRateSpinButton value
 */
int gtkGetAlsaRateSpinButton(void)
{
	GtkWidget * alsaRateSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaRateSpinButton"));
	return gtk_spin_button_get_value_as_int((GtkSpinButton *)alsaRateSpinButton);
}

/**
 * Sets AlsaPcmBufferFramesSpinButton value with ptZhavamConf->alsa.pcm_buffer_frames
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAlsaPcmBufferFramesSpinButton(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * alsaPcmBufferFramesSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaPcmBufferFramesSpinButton"));
	gtk_spin_button_set_value((GtkSpinButton *)alsaPcmBufferFramesSpinButton,(gdouble) ptZhavamConf->alsa.pcm_buffer_frames);
}

/**
 * Gets AlsaPcmBufferFramesSpinButton field
 * @return AlsaPcmBufferFramesSpinButton value
 */
int gtkGetAlsaPcmBufferFramesSpinButton(void)
{
	GtkWidget * alsaPcmBufferFramesSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaPcmBufferFramesSpinButton"));
	return gtk_spin_button_get_value_as_int((GtkSpinButton *)alsaPcmBufferFramesSpinButton);
}

/**
 * Sets AlsaPcmDeviceComboBoxText value with ptZhavamConf->alsa.pcm_dev
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetAlsaPcmDeviceComboBoxText(list_t * pcmRecDevList, zhavamConf_t * ptZhavamConf)
{
	char devComboTextLine[MAX_LEN_DEV_NAME] = "";

	GtkWidget * alsaPcmDeviceComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaPcmDeviceComboBoxText"));

	gtk_combo_box_text_remove_all((GtkComboBoxText*)alsaPcmDeviceComboBoxText);

	if (pcmRecDevList->head == NULL) return; // Empty list

	gtk_combo_box_text_append_text((GtkComboBoxText*)alsaPcmDeviceComboBoxText, "");
	for(node_t * ptr = pcmRecDevList->head;ptr;ptr = ptr->next)
		gtk_combo_box_text_append_text((GtkComboBoxText*)alsaPcmDeviceComboBoxText, ((pcmDev_t*)(ptr->item))->devName);

	gtkSetDefaultDevice(alsaPcmDeviceComboBoxText, ptZhavamConf);
}

/**
 * Gets AlsaPcmDeviceComboBoxText field
 * @return AlsaPcmDeviceComboBoxText value
 */
char * gtkGetAlsaPcmDeviceComboBoxText(void)
{
	GtkWidget * alsaPcmDeviceComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaPcmDeviceComboBoxText"));
	return gtk_combo_box_text_get_active_text((GtkComboBox *)alsaPcmDeviceComboBoxText);
}

/**
 * Opens (shows) the configDialog of zhavam
 * @param GtkImageMenuItem * menuConfigEdit
 * @param gpointer user_data
 */
void gtkZhavamConfigEdit(GtkImageMenuItem * menuConfigEdit, gpointer user_data)
{
	GtkWidget * configDialog = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "configDialog"));
	gtk_widget_show_all(configDialog);
	//gtk_widget_grab_focus((GtkWidget*)entryTokenFind);
}

/**
 * ConfigCloseButton callback
 * Calls configUpdate
 * @param GtkButton * configCloseButton
 * @param gpointer user_data
 */
void gtkConfigCloseButton(GtkButton * configCloseButton, gpointer user_data)
{
	GtkWidget * configDialog = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "configDialog"));
	gtk_widget_hide(configDialog);
	configUpdate((zhavamConf_t *)getZhavamConf());
}

/**
 * Updates ptZhavamConf with the values taken from configDialog
 * @param zhavamConf_t * ptZhavamConf
 */
void configUpdate(zhavamConf_t * ptZhavamConf)
{
	/*
	 * acrcloud
	 */
	if (ptZhavamConf->acrcloud.access_key_) free(ptZhavamConf->acrcloud.access_key_);
	ptZhavamConf->acrcloud.access_key_ = malloc(strlen(gtkGetAcrCloudAccessKeyEntry())+1);
	sprintf(ptZhavamConf->acrcloud.access_key_, "%s", gtkGetAcrCloudAccessKeyEntry());

	if (ptZhavamConf->acrcloud.access_secret_) free(ptZhavamConf->acrcloud.access_secret_);
	ptZhavamConf->acrcloud.access_secret_ = malloc(strlen(gtkGetAcrCloudAccessSecretEntry())+1);
	sprintf(ptZhavamConf->acrcloud.access_secret_, "%s", gtkGetAcrCloudAccessSecretEntry());

	if (ptZhavamConf->acrcloud.host_) free(ptZhavamConf->acrcloud.host_);
	ptZhavamConf->acrcloud.host_ = malloc(strlen(gtkGetAcrCloudHostEntry())+1);
	sprintf(ptZhavamConf->acrcloud.host_, "%s", gtkGetAcrCloudHostEntry());

	ptZhavamConf->acrcloud.rec_type_ = recTypeDecode(gtkGetAcrCloudRecTypeComboBoxText());

	ptZhavamConf->acrcloud.timeout_ms_ = gtkGetAcrCloudTimeOutSpinButton();

	/*
	 * alsa
	 */
	ptZhavamConf->alsa.pcm_buffer_frames = gtkGetAlsaPcmBufferFramesSpinButton();

	ptZhavamConf->alsa.rate = gtkGetAlsaRateSpinButton();

	if (ptZhavamConf->alsa.pcm_dev) free(ptZhavamConf->alsa.pcm_dev);
	ptZhavamConf->alsa.pcm_dev = malloc(strlen(gtkGetAlsaPcmDeviceComboBoxText())+1);
	sprintf(ptZhavamConf->alsa.pcm_dev, "%s", gtkGetAlsaPcmDeviceComboBoxText());

	ptZhavamConf->alsa.snd_pcm_format = sndPcmFormatDecode(gtkGetAlsaSndPcmFormatComboBoxText());

}

/**
 * ConfigSaveButton callback
 * Calls configUpdate(ptZhavamConf) and writeZhavamConfig(zhvHome, ptZhavamConf)
 * @param GtkButton * configCloseButton
 * @param gpointer user_data
 */
void gtkConfigSaveButton(GtkButton * configCloseButton, gpointer user_data)
{
	GtkWidget * configDialog = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "configDialog"));
	gtk_widget_hide(configDialog);
	zhavamConf_t * ptZhavamConf = (zhavamConf_t *)getZhavamConf(); //(zhavamConf_t *)user_data;
	configUpdate(ptZhavamConf);

	char zhvHome[2*ZHVHOMELEN];
	sprintf(zhvHome, "%s/%s/%s", getenv("HOME"), ZHVDIR, ZHVFILENAME);
	writeZhavamConfig(zhvHome, ptZhavamConf);
}

/**
 * ConfigCleanButton callback
 * Cleans the configDialog and sets it to its default values
 * @param GtkButton * configCloseButton
 * @param gpointer user_data
 */
void gtkConfigCleanButton(GtkButton * configCloseButton, gpointer user_data)
{
	zhavamConf_t * ptZhavamConf = (zhavamConf_t *)getZhavamConf(); //(zhavamConf_t *)user_data;
	setupZhavamConfigStruct(ptZhavamConf);
	list_t * pcmRecDevList;
	pcmRecDevList = listNew(pcmRecDevList);
	pcmRecDevList = getPCMRecDevices(pcmRecDevList);

	gtkConfigDialogSetUp(ptZhavamConf, NULL);
}

/**
 * Sets up the configDialog widgets with the values stored in ptZhavamConf
 * @param zhavamConf_t * ptZhavamConf
 * @param list_t * pcmRecDevList
 */
void gtkConfigDialogSetUp(zhavamConf_t * ptZhavamConf, list_t * pcmRecDevList)
{
	gtkSetAcrCloudHostEntry(ptZhavamConf);
	gtkSetAcrCloudAccessKeyEntry(ptZhavamConf);
	gtkSetAcrCloudAccessSecretEntry(ptZhavamConf);
	gtkSetAcrCloudTimeOutSpinButton(ptZhavamConf);
	gtkSetAcrCloudRecTypeComboBoxText(ptZhavamConf);

	gtkSetAlsaSndPcmFormatComboBoxText(ptZhavamConf);
	gtkSetAlsaRateSpinButton(ptZhavamConf);
	gtkSetAlsaPcmBufferFramesSpinButton(ptZhavamConf);
	if (pcmRecDevList) gtkSetAlsaPcmDeviceComboBoxText(pcmRecDevList, ptZhavamConf);
}

/**
 * menuConfigReload callback
 * Reloads the configuration kept in zhavam.conf config file and stores it in ptZhavamConf
 * @param GtkImageMenuItem * menuConfigReload
 * @param gpointer user_data
 */
void gtkZhavamConfigReload(GtkImageMenuItem * menuConfigReload, gpointer user_data)
{
	zhavamConf_t * ptZhavamConf = (zhavamConf_t *)getZhavamConf(); //(zhavamConf_t *)user_data;
	char zhvHome[2*ZHVHOMELEN];
	sprintf(zhvHome, "%s/%s/%s", getenv("HOME"), ZHVDIR, ZHVFILENAME);

	configLoad(zhvHome, ptZhavamConf);
	gtkConfigDialogSetUp(ptZhavamConf, NULL);
}

/**
 * menuConfigSave callback
 * Saves the configuration set in configDialog from ptZhavamConf into zhavam.conf config file
 * @param GtkImageMenuItem * menuConfigSave
 * @param gpointer user_data
 */
void gtkZhavamConfigSave(GtkImageMenuItem * menuConfigSave, gpointer user_data)
{
	zhavamConf_t * ptZhavamConf = (zhavamConf_t *)getZhavamConf(); //(zhavamConf_t *)user_data;
	configUpdate(ptZhavamConf);

	char zhvHome[2*ZHVHOMELEN];
	sprintf(zhvHome, "%s/%s/%s", getenv("HOME"), ZHVDIR, ZHVFILENAME);
	writeZhavamConfig(zhvHome, ptZhavamConf);
}

