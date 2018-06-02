/*
 * zhavam_config.c
 *
 *  Created on: 13 feb. 2018
 *      Author: ipserc
 */

#include <gtk/gtk.h>

#include <stdio.h>

#include "zhavam.h"
#include "zhavam_config.h"

/**
 * Sets AcrCloudHostEntry field with ptZhavamConf->acrcloud.host_
 * @param ptZhavamConf
 */
void gtkSetAcrCloudHostEntry(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudHostEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudHostEntry"));
	if (ptZhavamConf->acrcloud.host_) gtk_entry_set_text((GtkEntry *)acrCloudHostEntry, ptZhavamConf->acrcloud.host_);
	else gtk_entry_set_text((GtkEntry *)acrCloudHostEntry, "");
}

/**
 * Gets AcrCloudHostEntry field
 * @return AcrCloudHostEntry value
 */
const char * gtkGetAcrCloudHostEntry(void)
{
	GtkWidget * acrCloudHostEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudHostEntry"));
	return gtk_entry_get_text((GtkEntry *)acrCloudHostEntry);
}

/**
 * Sets AcrCloudAccessKeyEntry field with ptZhavamConf->acrcloud.access_key_
 * @param ptZhavamConf
 */
void gtkSetAcrCloudAccessKeyEntry(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudAccessKeyEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessKeyEntry"));
	if (ptZhavamConf->acrcloud.access_key_) gtk_entry_set_text((GtkEntry* )acrCloudAccessKeyEntry, ptZhavamConf->acrcloud.access_key_);
	else gtk_entry_set_text((GtkEntry* )acrCloudAccessKeyEntry, "");
}

/**
 * Gets AcrCloudAccessKeyEntry field
 * @return AcrCloudAccessKeyEntry value
 */
const char * gtkGetAcrCloudAccessKeyEntry(void)
{
	GtkWidget * acrCloudAccessKeyEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessKeyEntry"));
	return gtk_entry_get_text((GtkEntry* )acrCloudAccessKeyEntry);
}

/**
 * Sets AcrCloudAccessSecretEntry field with ptZhavamConf->acrcloud.access_secret_
 * @param ptZhavamConf
 */
void gtkSetAcrCloudAccessSecretEntry(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * acrCloudAccessSecretEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessSecretEntry"));
	if (ptZhavamConf->acrcloud.access_secret_) gtk_entry_set_text((GtkEntry* )acrCloudAccessSecretEntry, ptZhavamConf->acrcloud.access_secret_);
	else gtk_entry_set_text((GtkEntry* )acrCloudAccessSecretEntry, "");
}

/**
 * Gets AcrCloudAccessSecretEntry field
 * @return AcrCloudAccessSecretEntry value
 */
const char * gtkGetAcrCloudAccessSecretEntry(void)
{
	GtkWidget * acrCloudAccessSecretEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "acrCloudAccessSecretEntry"));
	return gtk_entry_get_text((GtkEntry* )acrCloudAccessSecretEntry);
}

/**
 * Sets AcrCloudTimeOutSpinButton value with ptZhavamConf->acrcloud.timeout_ms_
 * @param ptZhavamConf
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
	return gtk_spin_button_get_value_as_int((GtkSpinButton *)acrCloudTimeOutSpinButton);
}

/**
 * Sets AcrCloudRecTypeComboBoxText field with ptZhavamConf->acrcloud.rec_type_
 * @param ptZhavamConf
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
	return gtk_combo_box_text_get_active_text((GtkComboBoxText *)acrCloudRecTypeComboBoxText);
}

/**
 * Sets AlsaSndPcmFormatComboBoxText field with ptZhavamConf->alsa.snd_pcm_format
 * @param ptZhavamConf
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
	return gtk_combo_box_text_get_active_text((GtkComboBoxText *)alsaSndPcmFormatComboBoxText);
}

/**
 * Sets AlsaRateSpinButton value with ptZhavamConf->alsa.rate
 * @param ptZhavamConf
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
 * @param ptZhavamConf
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
 * @param ptZhavamConf
 */
void gtkSetAlsaPcmDeviceComboBoxText(list_t * pcmRecDevList, zhavamConf_t * ptZhavamConf)
{
	GtkWidget * alsaPcmDeviceComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaPcmDeviceComboBoxText"));

	gtk_combo_box_text_remove_all((GtkComboBoxText *)alsaPcmDeviceComboBoxText);

	if (pcmRecDevList->head == NULL) return; // Empty list

	gtk_combo_box_text_append_text((GtkComboBoxText *)alsaPcmDeviceComboBoxText, "");
	for(node_t * ptr = pcmRecDevList->head;ptr;ptr = ptr->next)
		gtk_combo_box_text_append_text((GtkComboBoxText *)alsaPcmDeviceComboBoxText, ((pcmDev_t*)(ptr->item))->devName);

	gtkSetDefaultDevice((GtkComboBoxText *)alsaPcmDeviceComboBoxText, ptZhavamConf);
}

/**
 * Gets AlsaPcmDeviceComboBoxText field
 * @return AlsaPcmDeviceComboBoxText value
 */
char * gtkGetAlsaPcmDeviceComboBoxText(void)
{
	GtkWidget * alsaPcmDeviceComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "alsaPcmDeviceComboBoxText"));
	return gtk_combo_box_text_get_active_text((GtkComboBoxText *)alsaPcmDeviceComboBoxText);
}

/**
 * Opens (shows) the configDialog of zhavam
 * @param menuConfigEdit
 * @param user_data
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
 * @param configCloseButton
 * @param user_data
 */
void gtkConfigCloseButton(GtkButton * configCloseButton, gpointer user_data)
{
	GtkWidget * configDialog = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "configDialog"));
	gtk_widget_hide(configDialog);
	configUpdate((zhavamConf_t *)getZhavamConf());
}

/**
 * Updates ptZhavamConf with the values taken from configDialog
 * @param ptZhavamConf
 */
void configUpdate(zhavamConf_t * ptZhavamConf)
{
	/*
	 * acrcloud
	 */
	if (ptZhavamConf->acrcloud.access_key_) free((void *)ptZhavamConf->acrcloud.access_key_);
	ptZhavamConf->acrcloud.access_key_ = malloc(strlen(gtkGetAcrCloudAccessKeyEntry())+1);
	sprintf(ptZhavamConf->acrcloud.access_key_, "%s", gtkGetAcrCloudAccessKeyEntry());

	if (ptZhavamConf->acrcloud.access_secret_) free((void *)ptZhavamConf->acrcloud.access_secret_);
	ptZhavamConf->acrcloud.access_secret_ = malloc(strlen(gtkGetAcrCloudAccessSecretEntry())+1);
	sprintf(ptZhavamConf->acrcloud.access_secret_, "%s", gtkGetAcrCloudAccessSecretEntry());

	if (ptZhavamConf->acrcloud.host_) free((void *)ptZhavamConf->acrcloud.host_);
	ptZhavamConf->acrcloud.host_ = malloc(strlen(gtkGetAcrCloudHostEntry())+1);
	sprintf(ptZhavamConf->acrcloud.host_, "%s", gtkGetAcrCloudHostEntry());

	ptZhavamConf->acrcloud.rec_type_ = recTypeDecode(gtkGetAcrCloudRecTypeComboBoxText());

	ptZhavamConf->acrcloud.timeout_ms_ = gtkGetAcrCloudTimeOutSpinButton();

	/*
	 * alsa
	 */
	ptZhavamConf->alsa.pcm_buffer_frames = gtkGetAlsaPcmBufferFramesSpinButton();

	ptZhavamConf->alsa.rate = gtkGetAlsaRateSpinButton();

	if (ptZhavamConf->alsa.pcm_dev) free((void *)ptZhavamConf->alsa.pcm_dev);
	ptZhavamConf->alsa.pcm_dev = malloc(strlen(gtkGetAlsaPcmDeviceComboBoxText())+1);
	sprintf(ptZhavamConf->alsa.pcm_dev, "%s", gtkGetAlsaPcmDeviceComboBoxText());

	ptZhavamConf->alsa.snd_pcm_format = sndPcmFormatDecode(gtkGetAlsaSndPcmFormatComboBoxText());

}

/**
 * ConfigSaveButton callback
 * Calls configUpdate(ptZhavamConf) and writeZhavamConfig(zhvHome, ptZhavamConf)
 * @param configCloseButton
 * @param user_data
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
 * @param configCloseButton
 * @param user_data
 */
void gtkConfigCleanButton(GtkButton * configCloseButton, gpointer user_data)
{
	zhavamConf_t * ptZhavamConf = (zhavamConf_t *)getZhavamConf(); //(zhavamConf_t *)user_data;
	setupZhavamConfigStruct(ptZhavamConf);
	list_t * pcmRecDevList;
	listNew(&pcmRecDevList);
	pcmRecDevList = getPCMRecDevices(pcmRecDevList);

	gtkConfigDialogSetUp(ptZhavamConf, NULL);
}

/**
 * Sets up the configDialog widgets with the values stored in ptZhavamConf
 * @param ptZhavamConf
 * @param pcmRecDevList
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
 * @param menuConfigReload
 * @param user_data
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
 * @param menuConfigSave
 * @param user_data
 */
void gtkZhavamConfigSave(GtkImageMenuItem * menuConfigSave, gpointer user_data)
{
	zhavamConf_t * ptZhavamConf = (zhavamConf_t *)getZhavamConf(); //(zhavamConf_t *)user_data;
	configUpdate(ptZhavamConf);

	char zhvHome[2*ZHVHOMELEN];
	sprintf(zhvHome, "%s/%s/%s", getenv("HOME"), ZHVDIR, ZHVFILENAME);
	writeZhavamConfig(zhvHome, ptZhavamConf);
}
