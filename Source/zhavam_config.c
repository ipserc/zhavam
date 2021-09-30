/*
 * zhavam_config.c
 *
 *  Created on: 13 feb. 2018
 *      Author: ipserc
 */

#include <gtk/gtk.h>

#include <stdio.h>
#include <libconfig.h>
#include <sys/stat.h>

#include "zhavam.h"
#include "zhavam_errtra.h"
#include "zhavam_config.h"

/**
 * Static "private method" to set or get the the zhavamConf_t * zhavamConf "member" variable. DO NOT CALL IT DIRECTLY
 * @param method: Selector for (0) create a new instance or returning the one created
 * @return static zhavamConf_t * zhavamConf
 */
static zhavamConf_t * zhavamConf(int method)
{
	static zhavamConf_t zhavamConf;

	if (method == 0) //SET
	{
		setupZhavamConfigStruct(&zhavamConf);
	}
	return &zhavamConf;
}

/**
 * Sets zhavamConf_t * zhavamConf pointer to the static variable. Use it to create it
 * @return static zhavamConf_t * zhavamConf
 */
zhavamConf_t * setZhavamConf()
{
	return (zhavamConf_t *)zhavamConf(0);
}

/**
 * Returns the zhavamConf_t * zhavamConf static variable
 * @return static zhavamConf_t * zhavamConf
 */
zhavamConf_t * getZhavamConf()
{
	return (zhavamConf_t *)zhavamConf(1);
}


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

	const char ** zhv_acr_opt_rec_str = getZhvAcrOptRecStr();
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

	const char ** zhv_alsa_snd_pcm_format_list = getZhvAlsaSndPcmFormatList();
	for (int i = SND_PCM_FORMAT_S8; i < IND_SND_PCM_FORMAT_LAST_ITEM; ++i) {
		gtk_combo_box_text_append_text((GtkComboBoxText *)alsaSndPcmFormatComboBoxText, zhv_alsa_snd_pcm_format_list[i]);
	}
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

	for(node_t * ptr = pcmRecDevList->head; ptr; ptr = ptr->next)
		gtk_combo_box_text_append_text((GtkComboBoxText *)alsaPcmDeviceComboBoxText, ((soundDevice_t*)(ptr->item))->description);

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
	ptZhavamConf->acrcloud.access_key_ = gtkGetAcrCloudAccessKeyEntry();
	ptZhavamConf->acrcloud.access_secret_ = gtkGetAcrCloudAccessSecretEntry();
	ptZhavamConf->acrcloud.host_ = gtkGetAcrCloudHostEntry();
	ptZhavamConf->acrcloud.rec_type_ = recTypeDecode(gtkGetAcrCloudRecTypeComboBoxText());
	ptZhavamConf->acrcloud.timeout_ms_ = gtkGetAcrCloudTimeOutSpinButton();
	/*
	 * Driver Controller
	 */
	ptZhavamConf->driverController = driverControllerDecode(gtkGetdriverControllerComboBoxText());

	switch (ptZhavamConf->driverController)
	{
	case ALSA:
		ptZhavamConf->alsa.pcm_buffer_frames = gtkGetAlsaPcmBufferFramesSpinButton();
		ptZhavamConf->alsa.rate = gtkGetAlsaRateSpinButton();
		ptZhavamConf->alsa.pcm_dev = gtkGetAlsaPcmDeviceComboBoxText();
		ptZhavamConf->alsa.snd_pcm_format = alsaSndPcmFormatDecode(gtkGetAlsaSndPcmFormatComboBoxText());
		break;
	case PULSE:
		ptZhavamConf->pulse.pcm_buffer_frames = gtkGetPulsePcmBufferFramesSpinButton();
		ptZhavamConf->pulse.rate = gtkGetPulseRateSpinButton();
		ptZhavamConf->pulse.pcm_dev = gtkGetPulsePcmDeviceComboBoxText();
		ptZhavamConf->pulse.pa_sample_format = pulsePaSampleFormatDecode(gtkGetPulsePaSampleFormatComboBoxText());
		break;
	}
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
	list_t * audioRecDevList;
	listNew(&audioRecDevList);
	switch (ptZhavamConf->driverController) {
	case ALSA:
		audioRecDevList = alsaGetPCMRecDevicesList(audioRecDevList);
		break;
	case PULSE:
		audioRecDevList = pulseGetRecDevicesList(audioRecDevList);
		break;
	default:
		break;
	}
	gtkConfigDialogSetUp(ptZhavamConf);
}

/**
 * Sets the Config Notebook Page from ptZhavamConf driver Controller
 * @param ptZhavamConf
 */
void gtkSetDriverControllerNotebookPage(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * driverControllerNotebook = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "driverControllerNotebook"));
	gtk_notebook_set_current_page ((GtkNotebook *)driverControllerNotebook,(gint) ptZhavamConf->driverController);
}

/**
 * Sets up the configDialog widgets with the values stored in ptZhavamConf
 * @param ptZhavamConf
 * @param pcmRecDevList
 */
void gtkConfigDialogSetUp(zhavamConf_t * ptZhavamConf)
{
	list_t * soundDevList = listNew(&soundDevList);

	if (!soundDevList)
	{
		sprintf(STATUS_MESSAGE, "cannot create a new list of sound devices. Allocation memory problem.");
		ERROR("%s", STATUS_MESSAGE);
	}

	gtkSetAcrCloudHostEntry(ptZhavamConf);
	gtkSetAcrCloudAccessKeyEntry(ptZhavamConf);
	gtkSetAcrCloudAccessSecretEntry(ptZhavamConf);
	gtkSetAcrCloudTimeOutSpinButton(ptZhavamConf);
	gtkSetAcrCloudRecTypeComboBoxText(ptZhavamConf);

	gtkSetDriverControllerEntry(ptZhavamConf);
	//gtkSetDriverControllerComboBoxText(ptZhavamConf);
	//gtkDriverControllerComboBoxTextChange((GtkComboBoxText *)NULL, NULL);

	gtkSetAlsaSndPcmFormatComboBoxText(ptZhavamConf);
	gtkSetAlsaRateSpinButton(ptZhavamConf);
	gtkSetAlsaPcmBufferFramesSpinButton(ptZhavamConf);
	if (alsaGetPCMRecDevicesList(soundDevList)) gtkSetAlsaPcmDeviceComboBoxText(soundDevList, ptZhavamConf);

	gtkSetPulsePaSampleFormatComboBoxText(ptZhavamConf);
	gtkSetPulseRateSpinButton(ptZhavamConf);
	gtkSetPulsePcmBufferFramesSpinButton(ptZhavamConf);
	listDestroy(soundDevList, (void *)NULL);
	soundDevList = listNew(&soundDevList);
	if (pulseGetRecDevicesList(soundDevList)) gtkSetPulsePcmDeviceComboBoxText(soundDevList, ptZhavamConf);

	gtkSetDriverControllerNotebookPage(ptZhavamConf);

	listDestroy(soundDevList, (void *)NULL);
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
	gtkConfigDialogSetUp(ptZhavamConf);
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

/**
 * Sets PulseSndPaSampleComboBoxText field with ptZhavamConf->Pulse.snd_pcm_format
 * @param ptZhavamConf
 */
void gtkSetPulsePaSampleFormatComboBoxText(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * pulsePaSampleFormatComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulsePaSampleFormatComboBoxText"));

	gtk_combo_box_text_remove_all((GtkComboBoxText *)pulsePaSampleFormatComboBoxText);

	const char ** zhv_pulse_pa_sample_format_str = getZhvPaSampleFormatStr();
	for (int i = IND_PA_SAMPLE_U8; i < IND_PA_SAMPLE_MAX; ++i)
		gtk_combo_box_text_append_text((GtkComboBoxText *)pulsePaSampleFormatComboBoxText, zhv_pulse_pa_sample_format_str[i]);
	gtk_combo_box_set_active((GtkComboBox *)pulsePaSampleFormatComboBoxText, ptZhavamConf->pulse.pa_sample_format);
}

/**
 * Gets PulseSndPaSampleComboBoxText field
 * @return PulseSndPaSampleComboBoxText value
 */
char * gtkGetPulsePaSampleFormatComboBoxText(void)
{
	GtkWidget * pulsePaSampleFormatComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulsePaSampleFormatComboBoxText"));
	return gtk_combo_box_text_get_active_text((GtkComboBoxText *)pulsePaSampleFormatComboBoxText);
}

/**
 * Sets PulseRateSpinButton value with ptZhavamConf->Pulse.rate
 * @param ptZhavamConf
 */
void gtkSetPulseRateSpinButton(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * pulseRateSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulseRateSpinButton"));
	gtk_spin_button_set_value((GtkSpinButton *)pulseRateSpinButton,(gdouble) ptZhavamConf->pulse.rate);
}

/**
 * Gets PulseRateSpinButton field
 * @return PulseRateSpinButton value
 */
int gtkGetPulseRateSpinButton(void)
{
	GtkWidget * pulseRateSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulseRateSpinButton"));
	return gtk_spin_button_get_value_as_int((GtkSpinButton *)pulseRateSpinButton);
}

/**
 * Sets PulsePcmBufferFramesSpinButton value with ptZhavamConf->Pulse.pcm_buffer_frames
 * @param ptZhavamConf
 */
void gtkSetPulsePcmBufferFramesSpinButton(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * pulsePcmBufferFramesSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulsePcmBufferFramesSpinButton"));
	gtk_spin_button_set_value((GtkSpinButton *)pulsePcmBufferFramesSpinButton,(gdouble) ptZhavamConf->pulse.pcm_buffer_frames);
}

/**
 * Gets PulsePcmBufferFramesSpinButton field
 * @return PulsePcmBufferFramesSpinButton value
 */
int gtkGetPulsePcmBufferFramesSpinButton(void)
{
	GtkWidget * pulsePcmBufferFramesSpinButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulsePcmBufferFramesSpinButton"));
	return gtk_spin_button_get_value_as_int((GtkSpinButton *)pulsePcmBufferFramesSpinButton);
}

/**
 * Sets PulsePcmDeviceComboBoxText value with ptZhavamConf->Pulse.pcm_dev
 * @param ptZhavamConf
 */
void gtkSetPulsePcmDeviceComboBoxText(list_t * pcmRecDevList, zhavamConf_t * ptZhavamConf)
{
	GtkWidget * pulsePcmDeviceComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulsePcmDeviceComboBoxText"));

	gtk_combo_box_text_remove_all((GtkComboBoxText *)pulsePcmDeviceComboBoxText);

	if (pcmRecDevList->head == NULL) return; // Empty list

	for(node_t * ptr = pcmRecDevList->head;ptr;ptr = ptr->next)
		gtk_combo_box_text_append_text((GtkComboBoxText *)pulsePcmDeviceComboBoxText, ((soundDevice_t*)(ptr->item))->description);

	gtkSetDefaultDevice((GtkComboBoxText *)pulsePcmDeviceComboBoxText, ptZhavamConf);
}

/**
 * Gets PulsePcmDeviceComboBoxText field
 * @return PulsePcmDeviceComboBoxText value
 */
char * gtkGetPulsePcmDeviceComboBoxText(void)
{
	GtkWidget * pulsePcmDeviceComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "pulsePcmDeviceComboBoxText"));
	return gtk_combo_box_text_get_active_text((GtkComboBoxText *)pulsePcmDeviceComboBoxText);
}

/**
 * "member" variable with the text names of the driverController used
 */
static const char * zhv_driverController_str[] = {
		"Alsa",
		"Pulse",
		"Unknown"
};

/**
 * get "method" to "member" variable zhv_driverController_str
 * @returns the array with driverController names
 */
const char ** getZhvDriverControllerList(void)
{
	return zhv_driverController_str;
}

/**
 * Sets the default device in the driverControllerComboBoxText according with the value stored in ptZhavamConf->alsa.pcm_dev or the first value if NULL
 * @param driverControllerComboBoxText
 * @param ptZhavamConf
 */
void gtkSetDefaultDriverController(GtkComboBoxText * driverControllerComboBoxText, zhavamConf_t * ptZhavamConf)
{
	if (ptZhavamConf->driverController != UNKNOWN_CONTROLLER)
	{
		int index = 0;
		char * comboText;
		const char * driverControllerStr = driverControllerString(ptZhavamConf->driverController);
		do
		{
			gtk_combo_box_set_active((GtkComboBox *)driverControllerComboBoxText, index++);
			comboText = gtk_combo_box_text_get_active_text((GtkComboBoxText *)driverControllerComboBoxText);
			if (comboText)
			{
				if (strstr(comboText, driverControllerStr)) break;
			}
		} while(comboText);
		if (!comboText) gtk_combo_box_set_active((GtkComboBox *)driverControllerComboBoxText, 0);
		else g_free(comboText);
	}
}

/**
 * Sets the default device in the driverControllerEntry according with the value stored in ptZhavamConf->alsa.pcm_dev or the first value if NULL
 * @param driverControllerEntry
 * @param ptZhavamConf
 */
void gtkSetDriverControllerEntry(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * driverControllerEntry = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "driverControllerEntry"));
	const char ** zhv_driverController_list = getZhvDriverControllerList();
	gtk_entry_set_text((GtkEntry *)driverControllerEntry, zhv_driverController_list[ptZhavamConf->driverController]);
}

/**
 * Sets driverControllerComboBoxText value with ptZhavamConf->driver.dev_controller
 * @param ptZhavamConf
 */
void gtkSetDriverControllerComboBoxText(zhavamConf_t * ptZhavamConf)
{
	GtkWidget * driverControllerComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "driverControllerComboBoxText"));

	gtk_combo_box_text_remove_all((GtkComboBoxText *)driverControllerComboBoxText);

	const char ** zhv_driverController_list = getZhvDriverControllerList();
	for(int i = ALSA; i < LAST_CONTROLLER; ++i)
			gtk_combo_box_text_append_text((GtkComboBoxText *)driverControllerComboBoxText, zhv_driverController_list[i]);
	gtkSetDefaultDriverController((GtkComboBoxText *)driverControllerComboBoxText, ptZhavamConf);
}

/**
 * Gets driverControllerComboBoxText field
 * @return driverControllerComboBoxText value
 */
char * gtkGetdriverControllerComboBoxText(void)
{
	GtkWidget * driverControllerComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "driverControllerComboBoxText"));
	return gtk_combo_box_text_get_active_text((GtkComboBoxText *)driverControllerComboBoxText);
}

/**
 * Looks for the driverController enumerator supported by zhavam.
 * @param driverControllerStr: String to receive the driver controller name
 * @return the driverController enumerator or UNKNOWN_CONTROLLER if not found
 */
driverController_t driverControllerDecode(const char * driverControllerStr)
{
	for(int i = ALSA; i < LAST_CONTROLLER; ++i )
		if (!strcmp(driverControllerStr, zhv_driverController_str[i])) return i;
	return UNKNOWN_CONTROLLER;
}

/**
 * Looks for the PA_SAMPLE name supported by PulseAudio.
 * @param strSndPaSample: String to receive the PA_SAMPLE name
 * @param sndPcmFormat: The enumerator for the PA Sample Format
 * @return the PA_SAMPLE name or "An invalid value" if not found
 */
const char * driverControllerString(driverController_t driverController)
{
	const char ** zhv_driverController_list = getZhvDriverControllerList();

	if (driverController >= ALSA && driverController < LAST_CONTROLLER) return zhv_driverController_list[driverController];
	else return zhv_driverController_list[LAST_CONTROLLER];
}

/**
 * Prints in the console the configuration of Zhavam.
 * @param ptZhavamConf: pointer to static zhavamConf_t zhavamConf global variable
 */
void printZhavamConf(zhavamConf_t * ptZhavamConf)
{
	puts("------------------- Zhavam Conf -------------------");
	printf("appName:%s\n", ptZhavamConf->appName);
	printf("driverController:(%d) %s\n", ptZhavamConf->driverController, driverControllerString(ptZhavamConf->driverController));
	puts("	--- acrCloud ---");
	printf("acrcloud.access_key_:%s\n", ptZhavamConf->acrcloud.access_key_);
	printf("acrcloud.access_secret_:%s\n", ptZhavamConf->acrcloud.access_secret_);
	printf("acrcloud.host_:%s\n", ptZhavamConf->acrcloud.host_);
	printf("acrcloud.rec_type_:(%d) %s\n", ptZhavamConf->acrcloud.rec_type_, recTypeString(ptZhavamConf->acrcloud.rec_type_));
	printf("acrcloud.timeout_ms_:%d\n", ptZhavamConf->acrcloud.timeout_ms_);
	puts("	--- Alsa ---");
	printf("alsa.pcm_buffer_frames:%d\n", ptZhavamConf->alsa.pcm_buffer_frames);
	printf("alsa.pcm_dev:%s\n", ptZhavamConf->alsa.pcm_dev);
	printf("alsa.rate:%d\n", ptZhavamConf->alsa.rate);
	printf("alsa.snd_pcm_format:(%d) %s\n", ptZhavamConf->alsa.snd_pcm_format, alsaSndPcmFormatString(ptZhavamConf->alsa.snd_pcm_format));
	puts("	--- Pulse ---");
	printf("pulse.pcm_buffer_frames:%d\n", ptZhavamConf->pulse.pcm_buffer_frames);
	printf("pulse.pcm_dev:%s\n", ptZhavamConf->pulse.pcm_dev);
	printf("pulse.rate:%d\n", ptZhavamConf->pulse.rate);
	printf("pulse.pa_sample_format:(%d) %s\n", ptZhavamConf->pulse.pa_sample_format, pulsePaSampleFormatString(ptZhavamConf->pulse.pa_sample_format));
	puts("---------------------------------------------------\n");
}


/**
 * Creates the file zhavam.conf from scratch
 * first set up the default values for acrcloud and alsa
 * second writes these values in a new zhavam.conf file
 * @param zhvHome
 * @param ptZhavamConf
 */
void createZhavamConf(char * zhvHome, zhavamConf_t * ptZhavamConf)
{
	WARNING("%s", WARNING04);
	gtkWarning("%s", WARNING04);
	setupZhavamConfigStruct(ptZhavamConf);
	writeZhavamConfig(zhvHome, ptZhavamConf);
}

/**
 * Loads the configuration from zhavam.conf
 * @param zhvHome
 * @param ptZhavamConf
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int configLoad(char * zhvHome, zhavamConf_t * ptZhavamConf)
{
	config_t cfg;

	config_init(&cfg);

	/* Read the file. If there is an error, report it and exit. */
	if (!config_read_file(&cfg, zhvHome))
	{
		sprintf(STATUS_MESSAGE, "%s file:%s line:%d error:%s", WARNING02, config_error_file(&cfg),
				config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		ERROR("%s", STATUS_MESSAGE);
		return EXIT_FAILURE;
	}

	const char * str;
	config_lookup_string(&cfg, "acrcloud.access_key", &(ptZhavamConf->acrcloud.access_key_));
	config_lookup_string(&cfg, "acrcloud.access_secret", &(ptZhavamConf->acrcloud.access_secret_));
	config_lookup_string(&cfg, "acrcloud.host", &(ptZhavamConf->acrcloud.host_));
	config_lookup_string(&cfg, "acrcloud.rec_type", &str);
	ptZhavamConf->acrcloud.rec_type_ = recTypeDecode(str);
	config_lookup_int(&cfg, "acrcloud.timeout_ms", &(ptZhavamConf->acrcloud.timeout_ms_));

	config_lookup_string(&cfg, "driverController", &str);
	ptZhavamConf->driverController = driverControllerDecode(str);

	config_lookup_string(&cfg, "alsa.snd_pcm_format", (const char **)&str);
	ptZhavamConf->alsa.snd_pcm_format = alsaSndPcmFormatDecode(str);
	config_lookup_string(&cfg, "alsa.pcm_dev", (const char **)(&(ptZhavamConf->alsa.pcm_dev)));
	config_lookup_int(&cfg, "alsa.pcm_buffer_frames", (int *)&(ptZhavamConf->alsa.pcm_buffer_frames));
	config_lookup_int(&cfg, "alsa.rate", (int *)&(ptZhavamConf->alsa.rate));

	config_lookup_string(&cfg, "pulse.pa_sample_format", (const char **)&str);
	ptZhavamConf->pulse.pa_sample_format = pulsePaSampleFormatDecode(str);
	config_lookup_string(&cfg, "pulse.pcm_dev", (const char **)(&(ptZhavamConf->pulse.pcm_dev)));
	config_lookup_int(&cfg, "pulse.pcm_buffer_frames", (int *)&(ptZhavamConf->pulse.pcm_buffer_frames));
	config_lookup_int(&cfg, "pulse.rate", (int *)&(ptZhavamConf->pulse.rate));

	return EXIT_SUCCESS;
}

/**
 * Initializes Zhavam ConfigStruct
 * @param ptZhavamConf
 */
void setZhavamConfigStruct(zhavamConf_t * ptZhavamConf)
{
	ptZhavamConf->acrcloud.access_key_ = NULL;
	ptZhavamConf->acrcloud.access_secret_= NULL;
	ptZhavamConf->acrcloud.host_ = NULL;
	ptZhavamConf->acrcloud.rec_type_ = -1;
	ptZhavamConf->acrcloud.timeout_ms_ = -1;

	ptZhavamConf->driverController = -1;

	ptZhavamConf->alsa.pcm_buffer_frames = 0;
	ptZhavamConf->alsa.pcm_dev = NULL;
	ptZhavamConf->alsa.rate = 0;
	ptZhavamConf->alsa.snd_pcm_format = -1;

	ptZhavamConf->pulse.pcm_buffer_frames = 0;
	ptZhavamConf->pulse.pcm_dev = NULL;
	ptZhavamConf->pulse.rate = 0;
	ptZhavamConf->pulse.pa_sample_format = -1;
}

/**
 * Sets up Zhavam ConfigStruct to the default values
 * @param ptZhavamConf
 */
void setupZhavamConfigStruct(zhavamConf_t * ptZhavamConf)
{
	ptZhavamConf->acrcloud.access_key_ = NULL;
	ptZhavamConf->acrcloud.access_secret_= NULL;
	ptZhavamConf->acrcloud.host_ = NULL;
	ptZhavamConf->acrcloud.rec_type_ = acr_opt_rec_audio;
	ptZhavamConf->acrcloud.timeout_ms_ = 5000;

	ptZhavamConf->driverController = PULSE;

	ptZhavamConf->alsa.pcm_buffer_frames = 153600;
	ptZhavamConf->alsa.pcm_dev = NULL;
	ptZhavamConf->alsa.rate = 44100;
	ptZhavamConf->alsa.snd_pcm_format = SND_PCM_FORMAT_S16;

	ptZhavamConf->pulse.pcm_buffer_frames = 153600;
	ptZhavamConf->pulse.pcm_dev = NULL;
	ptZhavamConf->pulse.rate = 44100;
	ptZhavamConf->pulse.pa_sample_format = IND_PA_SAMPLE_S16LE;
}

/**
 * Writes Zhavam Configuration kept in ptZhavamConf to zhavam.conf
 * @param zhavamHome
 * @param ptZhavamConf
 */
void writeZhavamConfig(char * zhavamHome, zhavamConf_t * ptZhavamConf)
{
	FILE * fp;

	if((fp = fopen(zhavamHome, "w")))
	{
		fprintf(fp, "# ######################\n");
		fprintf(fp, "#\n");
		fprintf(fp, "# Zhavam config file\n");
		fprintf(fp, "#\n");
		fprintf(fp, "# ######################\n\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "# acrcloud config section\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "acrcloud:\n");
		fprintf(fp, "{\n");
		fprintf(fp, "	host = \"%s\";\n", ptZhavamConf->acrcloud.host_? ptZhavamConf->acrcloud.host_: "");
		fprintf(fp, "	access_key = \"%s\";\n", ptZhavamConf->acrcloud.access_key_? ptZhavamConf->acrcloud.access_key_ : "");
		fprintf(fp, "	access_secret = \"%s\";\n", ptZhavamConf->acrcloud.access_secret_ ? ptZhavamConf->acrcloud.access_secret_ : "");
		fprintf(fp, "	timeout_ms = %d;\n", ptZhavamConf->acrcloud.timeout_ms_);
		fprintf(fp, "	rec_type = \"%s\";\n", recTypeString(ptZhavamConf->acrcloud.rec_type_));
		fprintf(fp, "};\n\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "# driver controller config section\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "driverController = \"%s\";\n\n", driverControllerString(ptZhavamConf->driverController));
		fprintf(fp, "# ######################\n");
		fprintf(fp, "# alsa config section\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "alsa:\n");
		fprintf(fp, "{\n");
		fprintf(fp, "	snd_pcm_format = \"%s\";\n", alsaSndPcmFormatString(ptZhavamConf->alsa.snd_pcm_format));
		fprintf(fp, "	rate = %d;\n", ptZhavamConf->alsa.rate);
		fprintf(fp, "	pcm_buffer_frames = %d;\n", ptZhavamConf->alsa.pcm_buffer_frames);
		fprintf(fp, "	pcm_dev = \"%s\";\n", ptZhavamConf->alsa.pcm_dev ? ptZhavamConf->alsa.pcm_dev : "");
		fprintf(fp, "};\n\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "# pulse config section\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "pulse:\n");
		fprintf(fp, "{\n");
		fprintf(fp, "	pa_sample_format = \"%s\";\n", pulsePaSampleFormatString(ptZhavamConf->pulse.pa_sample_format));
		fprintf(fp, "	rate = %d;\n", ptZhavamConf->pulse.rate);
		fprintf(fp, "	pcm_buffer_frames = %d;\n", ptZhavamConf->pulse.pcm_buffer_frames);
		fprintf(fp, "	pcm_dev = \"%s\";\n", ptZhavamConf->pulse.pcm_dev ? ptZhavamConf->pulse.pcm_dev : "");
		fprintf(fp, "};\n");

		fclose(fp);
	}
	else {
		WARNING(WARNING05, zhavamHome);
		if (getGtkBuilder()) gtkWarning(WARNING05, zhavamHome);
	}
}

/**
 * Try to read the config file zhavam.conf
 * If this file doesn't exist calls createZhavamConf with the full path to create the file
 * If the file exists loads its content
 * @param ptZhavamConf
 * @parm zhavamConf_t * ptZhavamConf
 */
void zhavamConfig(zhavamConf_t * ptZhavamConf)
{
	struct stat s;
	char zhvHome[2*ZHVHOMELEN];
	char home[ZHVHOMELEN];

	setZhavamConfigStruct(ptZhavamConf);

	sprintf(home, "%s", getenv("HOME"));
	sprintf(zhvHome, "%s/%s", home, ZHVDIR);
	if (stat(zhvHome, &s) == -1) {
		statErrorMngr(errno);
	    if (errno == ENOENT) // does not exist
	    {
	    	if (mkdir(zhvHome, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) == -1) mkdirErrorMngr(errno);
	    	sprintf(zhvHome, "%s/%s", zhvHome, ZHVFILENAME);
	    	createZhavamConf(zhvHome, ptZhavamConf);
	    }
	} else if (!S_ISDIR(s.st_mode)) ERROR(ERROR01, zhvHome); // exists but it is not a dir

	sprintf(zhvHome, "%s/%s", zhvHome, ZHVFILENAME);
	if (stat(zhvHome, &s) == -1) {
		statErrorMngr(errno);
	    if (errno == ENOENT) // does not exist
	    	return createZhavamConf(zhvHome, ptZhavamConf);
	} else if (!S_ISREG(s.st_mode)) ERROR(ERROR02, zhvHome); // exists but it is not a REGULAR file
	configLoad(zhvHome, ptZhavamConf);
}

