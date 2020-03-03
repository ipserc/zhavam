/*
 * zhavam_alsa.c
 *
 *  Created on: 6 ene. 2018-02 feb 2020
 *      Author: ipserc
 *
 *      ./zhavam hw:1,0-1.4
 *      export LD_LIBRARY_PATH=/usr/local/lib/acrcloud
 *
 *      https://github.com/acrcloud/acrcloud_sdk_linux_c
 */

#include <gtk/gtkx.h>
#include <gtk/gtk.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <libgen.h>

#include "zhavam.h"
#include "zhavam_glade.h"
#include "zhavam_config.h"
#include "zhavam_acrcloud.h"
#include "zhavam_alsa.h"
#include "zhavam_pulse.h"
#include "zhavam_devices.h"
#include "zhavam_errtra.h"

/**
 * Global Variables GV_
 * No longer required, See "methods" below
 */
//GtkBuilder * GV_builder;
//zhavamConf_t GV_zhavamConf;

/**
 * Static "private method" to set or get the GtkBuilder * gtkbuilder static variable. DO NOT CALL IT DIRECTLY
 * @param method Selector for (0) set or get the GtkBuilder
 * @return static GtkBuilder * gtkbuilder
 */
/* private */static GtkBuilder * gtkbuilder(int method)
{
	static GtkBuilder * gtkbuilder = NULL;

	if (method == 0)	//SET
	{
		/*
		 * To load the GUI from the file ../config/zhavam.glade use this
		 */
		//gtkbuilder = gtk_builder_new_from_file("../config/zhavam.glade");
		/*
		 * To load the GUI from zhavam_glade.c use this
		 */
		gtkbuilder = gtk_builder_new_from_string(getGtkSource(), -1);
	}
	return gtkbuilder;
}

/**
 * Sets GtkBuilder * gtkbuilder pointer. Use it to create it
 * @return static GtkBuilder * gtkbuilder
 */
GtkBuilder * setGtkBuilder(void)
{
	return gtkbuilder(0);
}

/**
 * Returns the GtkBuilder * gtkbuilder static variable
 * @return static GtkBuilder * gtkbuilder
 */
GtkBuilder * getGtkBuilder(void)
{
	return gtkbuilder(1);
}

/**
 * Does all the magic. does Zhavam.
 * opens the device, Sets up Audio Device, PCM prepare, Start Record and recognize the song, Close the device and present the result
 * @param devID ID of the recording device
 * @param acrResponse The structure in which the information of the song is stored
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int doZhavam(char * devID, acr_data_t * acrResponse)
{
	zhavamConf_t * zhavamConf = getZhavamConf();
	acrcloud_config acrConfig = zhavamConf->acrcloud;

	if (!acrCloudSet(zhavamConf))
	{
		if (getGtkBuilder())
		{
			gtkSetCursor(NORMAL_CURSOR);
			gtkWarning("%s", WARNING06);
			gtkSetStatusZhvLabel(STATUS03);
		}
		else
		{
			WARNING("%s", WARNING06);
			TRACE("%s", STATUS03);
		}
		return EXIT_FAILURE;
	}
	else
	{
		if (getGtkBuilder())
		{
			gtkZhavamClearTrackInfoTextView(NULL, NULL);
			gtkSetStatusZhvLabel(STATUS01);
			gtkSetCursor(GDK_WATCH);
		}
		else TRACE("%s", STATUS01);
	}
	switch (zhavamConf->driverController) {
	case ALSA: {
		snd_pcm_t * capture_handle = NULL;
		snd_pcm_hw_params_t * hw_params = NULL;
		if (openDevice(devID, &capture_handle, &hw_params) < 0) return EXIT_FAILURE;
		if (setupAudioDevice(devID, capture_handle, hw_params, zhavamConf->alsa.snd_pcm_format, &zhavamConf->alsa.rate) < 0) return EXIT_FAILURE;
		if (pcmPrepare(capture_handle) < 0) return EXIT_FAILURE;
		if (alsaStartRecord(capture_handle, hw_params, zhavamConf->alsa.snd_pcm_format, zhavamConf->alsa.rate, zhavamConf->alsa.pcm_buffer_frames, acrConfig, acrResponse) < 0) return EXIT_FAILURE;
		if (closeDevice(capture_handle) < 0) return EXIT_FAILURE;
		}
		break;
	case PULSE: {
		pa_simple * ptrPaSimple;
		if (!(ptrPaSimple = pulseServerConnect(devID))) return EXIT_FAILURE;
		if (pulseStartRecord(ptrPaSimple, acrConfig, acrResponse) < 0) return EXIT_FAILURE;
		}
		break;
	default:
		WARNING("%s", WARNING17);
		if (!acrCloudSet(zhavamConf))
		{
			if (getGtkBuilder())
			{
				gtkSetCursor(NORMAL_CURSOR);
				gtkWarning("%s", WARNING17);
				gtkSetStatusZhvLabel(STATUS04);
			}
			TRACE("%s", STATUS04);
		}
		return EXIT_FAILURE;
	}
	if (!acrCloudSet(zhavamConf))
	{
		if (getGtkBuilder())
		{
			gtkSetStatusZhvLabel(STATUS02);
			gtkSetCursor(NORMAL_CURSOR);
		}
		TRACE("%s", STATUS02);
	}
	return EXIT_SUCCESS;
}

/**
 * Converts the ACR response from the acr_data_t structure to a text to be shown in zhavam track info multiline text box,
 * @param trackInfoText String with all the info of the track. This text is displayed in the info text view of the application
 * @param acrResponse The structure in which the information of the song is stored
 * @return A pointer to trackInfoText
 */
char * acrDataToText(char * trackInfoText, acr_data_t * acrResponse)
{
	char * ptr = trackInfoText;

	if (acrResponse->status.code[0] != '0')
	{
		ptr += sprintf(ptr, "status msg: %s\n", acrResponse->status.msg);
		ptr += sprintf(ptr, "status code: %s\n", acrResponse->status.code);
		ptr += sprintf(ptr, "status version: %s\n", acrResponse->status.version);
	}
	else
	{
		ptr += sprintf(ptr, "Title: %s\n", acrResponse->metadata.music.title);
		ptr += sprintf(ptr, "Artist: %s", acrResponse->metadata.music.artists[0]);
		for(int i = 1; i < MAX_ITEMS; ++i) {
			if (!acrResponse->metadata.music.artists[i][0]) break;
			ptr += sprintf(ptr, ", %s", acrResponse->metadata.music.artists[i]);
		}
		ptr += sprintf(ptr, "\n");
		ptr += sprintf(ptr, "Album: %s\n", acrResponse->metadata.music.album);
		ptr += sprintf(ptr, "Genre: %s", acrResponse->metadata.music.genres[0]);
		for(int i = 1; i < MAX_ITEMS; ++i) {
			if (!acrResponse->metadata.music.genres[i][0]) break;
			ptr += sprintf(ptr, ", %s", acrResponse->metadata.music.genres[i]);
		}
		ptr += sprintf(ptr, "\n");

		ptr += sprintf(ptr, "Label: %s\n", acrResponse->metadata.music.label);
		ptr += sprintf(ptr, "Release date: %s\n", acrResponse->metadata.music.release_date);
		ptr += sprintf(ptr, "Deezer album id: %s\n", acrResponse->metadata.music.external_metadata.deezer.album_id);
		ptr += sprintf(ptr, "Deezer track id: %s\n", acrResponse->metadata.music.external_metadata.deezer.track_id);
		ptr += sprintf(ptr, "Spotify album id: %s\n", acrResponse->metadata.music.external_metadata.spotify.album_id);
		ptr += sprintf(ptr, "Spotify track id: %s\n", acrResponse->metadata.music.external_metadata.spotify.track_id);
		ptr += sprintf(ptr, "Youtube video id: %s\n", acrResponse->metadata.music.external_metadata.youtube_vid);
		//printf("External ids: %s\n", acrResponse->metadata.music[m].external_ids);
		//printf("Play offset_ms: %s\n", acrResponse->metadata.music[m].play_offset_ms);
		//printf("External metadata: %s\n", acrResponse->metadata.music[m].external_metadata);
		//printf("Duration_ms: %s\n", acrResponse->metadata.music[m].duration_ms);
		//printf("Acrid: %s\n", acrResponse->metadata.music[m].acrid);
		//printf("Result from: %s\n", acrResponse->metadata.music[m].result_from);
		//printf("Score: %s\n", acrResponse->metadata.music[m].score);
		//printf("Timestamp utc: %s\n", acrResponse->metadata.timestamp_utc);
		//printf("Cost time: %s\n", acrResponse->cost_time);
		//printf("Result type: %s\n", acrResponse->result_type);
	}
	return trackInfoText;
}

/**
 * Cleans up the track info text buffer
 * @param textbuf The text buffer to clean
 */
void gtkDeleteTextBuffer(GtkTextBuffer * textbuf)
{
	GtkTextIter	start, end;

	if (!textbuf) return;
	gtk_text_buffer_get_bounds(textbuf, &start, &end);
	gtk_text_buffer_delete(textbuf, &start, &end);
	while (g_main_context_iteration(NULL, FALSE));
}

/**
 * Record ToggleButtonClicked Callback
 * @param recordToggleButton
 * @param data
 */
void gtkRecordToggleButtonClickedCallback(GtkToggleButton * recordToggleButton, gpointer user_data)
{
	acr_data_t acrResponse;
	char trackInfoText[ZHVTRACKINFOTEXTLEN];
	GtkTextBuffer * trackInfoTextBuffer = (GtkTextBuffer*)gtk_builder_get_object(getGtkBuilder(), "trackInfoTextBuffer");

	gtkDeleteTextBuffer(trackInfoTextBuffer);
	if (!gtk_toggle_button_get_active(recordToggleButton)) return;
	doZhavam(gtkGetDevID(), &acrResponse);
	gtk_toggle_button_set_active((GtkToggleButton *)recordToggleButton, FALSE);

	acrDataToText(trackInfoText, &acrResponse);
	gtk_text_buffer_set_text((GtkTextBuffer*)trackInfoTextBuffer, trackInfoText, -1);
	gtkSetCursor(NORMAL_CURSOR);
}

/**
 * Gets the DevID from devicesComboBoxText and stores it in zhavamConf->alsa.pcm_dev
 * return zhavamConf-> alsa.pcm_dev
 */
char * gtkGetDevID(void)
{
	zhavamConf_t * zhavamConf = getZhavamConf();
	GtkComboBoxText * devicesComboBoxText = (GtkComboBoxText*)GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "devicesComboBoxText"));
	sprintf(zhavamConf->alsa.pcm_dev, "%s", gtk_combo_box_get_active_id((GtkComboBox *)devicesComboBoxText));
	return zhavamConf->alsa.pcm_dev;
}

/**
 * Sets the device in the devicesComboBoxText according with the value stored in ptZhavamConf->alsa.pcm_dev or the first value if NULL
 * @param devicesComboBoxText
 * @param pcm_dev
 */
void gtkSetDeviceComboBoxText(GtkComboBoxText * devicesComboBoxText, char * pcm_dev)
{
	if (pcm_dev)
	{
		int index = 0;
		char * comboText;
		do
		{
			gtk_combo_box_set_active((GtkComboBox *)devicesComboBoxText, index++);
			comboText = gtk_combo_box_text_get_active_text((GtkComboBoxText *)devicesComboBoxText);
			if (comboText)
			{
				if (strstr(comboText, pcm_dev)) break;
			}
		} while(comboText);
		if (!comboText) gtk_combo_box_set_active((GtkComboBox *)devicesComboBoxText, 0);
		else g_free(comboText);
	}
}

/**
 * Sets the default device in the devicesComboBoxText according with the value stored in ptZhavamConf->alsa.pcm_dev or the first value if NULL
 * @param devicesComboBoxText
 * @param ptZhavamConf
 */
void gtkSetDefaultDevice(GtkComboBoxText * devicesComboBoxText, zhavamConf_t * ptZhavamConf)
{
	switch (ptZhavamConf->driverController)
	{
	case ALSA:
		gtkSetDeviceComboBoxText(devicesComboBoxText, ptZhavamConf->alsa.pcm_dev);
		break;
	case PULSE:
		gtkSetDeviceComboBoxText(devicesComboBoxText, ptZhavamConf->pulse.pcm_dev);
		break;
	default:
		gtk_combo_box_set_active((GtkComboBox *)devicesComboBoxText, 0);
	}
}

/**
 * Loads the Record Devices found into the devicesComboBoxText and shows the default device
 * @param pcmRecDevList
 * @param ptZhavamConf
 * @return TRUE if there is a default dev, else FALSE
 */
gboolean gtkLoadDevicesCombo(list_t * soundDevList, zhavamConf_t * ptZhavamConf)
{
	char devComboTextLine[2*LONG_LEN+4] = "";
	char devComboTextLineFormat[10];

	GtkComboBoxText * devicesComboBoxText = (GtkComboBoxText*)GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "devicesComboBoxText"));
	gtk_combo_box_text_remove_all(devicesComboBoxText);

	if (soundDevList->head == NULL) return FALSE; // Empty list

	sprintf(devComboTextLineFormat, "%s%is", "%.", DEV_COMBO_TEXT_LINE_LEN);

	for(node_t * ptr = soundDevList->head; ptr; ptr = ptr->next)
	{
		sprintf(devComboTextLine, devComboTextLineFormat, ((soundDevice_t*)(ptr->item))->description);
		gtk_combo_box_text_append((GtkComboBoxText*)devicesComboBoxText, ((soundDevice_t*)(ptr->item))->name, devComboTextLine);
	}
	gtk_combo_box_set_active((GtkComboBox *)(GtkComboBoxText*)devicesComboBoxText, 0);

	gtkSetDefaultDevice(devicesComboBoxText, ptZhavamConf);
	return TRUE;
}

/**
 * Sets Sensitive RecordToggleButton status
 * @param sensitive: true to set the button in sensitive mode, false set normal.
 */
void gtkSetSensitiveRecordToggleButton(gboolean sensitive)
{
	GtkWidget * recordToggleButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "recordToggleButton"));
	gtk_widget_set_sensitive(recordToggleButton, sensitive);
}

/**
 * Sets StatusZhvLabel with the text passed
 * @param text The text to set in the Status Label
 */
void gtkSetStatusZhvLabel(const char * text)
{
	GtkLabel * statusZhvLabel = GTK_LABEL(gtk_builder_get_object(getGtkBuilder(), "statusZhvLabel"));
	gtk_label_set_text(statusZhvLabel, text);
	while (gtk_events_pending()) gtk_main_iteration();
}

/**
 * Opens the warning dialog with the message passed as parameter
 * @param fmtstr Format string as in printf
 * @param ...: Rest of parameters
 */
void gtkWarning(const char * fmtstr,...)
{
	char WarningMsg[WARNINGMSGLEN];
	va_list arglist;

	va_start(arglist, fmtstr);
	vsprintf(WarningMsg, fmtstr, arglist);
	va_end(arglist);
	gtkSetStatusZhvLabel(WarningMsg);
	GtkWidget * dialogWarning = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "dialogWarning"));
	GtkWidget * labelDialogWarning = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "labelDialogWarning"));
	gtk_label_set_label((GtkLabel *)labelDialogWarning, WarningMsg);
	gtk_widget_show_all(dialogWarning);
}

/**
 * Sets the cursor with one of the types defined in http://developer.gimp.org/api/2.0/gdk/gdk-Cursors.html
 * Typically normal cursor or hour glass
 * @param cursorType the type of cursor as in http://developer.gimp.org/api/2.0/gdk/gdk-Cursors.html
 */
void gtkSetCursor(GdkCursorType cursorType)
{
	GdkScreen * screen = gdk_screen_get_default();
	GdkWindow * win = gdk_screen_get_root_window(screen);
	GdkDisplay * display = gdk_display_get_default();
	GdkCursor * cursor = gdk_cursor_new_for_display(display, cursorType); //http://developer.gimp.org/api/2.0/gdk/gdk-Cursors.html
	gdk_window_set_cursor(win, cursor);
	while (gtk_events_pending()) gtk_main_iteration();
}

/**
 * Looks whether the ACR Cloud access data is set
 * @param ptZhavamConf
 * @return true if all the required data are set, or false in other case
 */
bool acrCloudSet(zhavamConf_t * ptZhavamConf)
{
	if (!ptZhavamConf->acrcloud.host_		||
		!ptZhavamConf->acrcloud.access_key_	||
		!ptZhavamConf->acrcloud.access_secret_ ) return false;
	if (!ptZhavamConf->acrcloud.host_[0]		||
		!ptZhavamConf->acrcloud.access_key_[0]	||
		!ptZhavamConf->acrcloud.access_secret_[0]) return false;
	if (!strcmp(ptZhavamConf->acrcloud.host_, "") 		||
		!strcmp(ptZhavamConf->acrcloud.access_key_, "") ||
		!strcmp(ptZhavamConf->acrcloud.access_secret_, "")) return false;
	return true;
}

/**
 * Initializes the devicesComboText with the devices given by the driver controller
 * @param ptZhavamConf
 */
void gtkInitDevicesComboBoxText(zhavamConf_t * ptZhavamConf)
{
	char statusActivateMsg[TEXTZHAVAMDO];

	list_t * soundDevList = listNew(&soundDevList);
	switch (ptZhavamConf->driverController) {
	case ALSA:
		soundDevList = alsaGetPCMRecDevicesList(soundDevList); //getPCMRecDevices(pcmRecDevList);
		break;
	case PULSE:
		soundDevList = pulseGetRecDevicesList(soundDevList);
		break;
	default:
		sprintf(statusActivateMsg, "%s", WARNING17);
		gtkSetStatusZhvLabel(statusActivateMsg);
		gtkWarning("%s", WARNING17);
		break;
	}

	// Load the capture sound devices in the devices combo box
	if (!gtkLoadDevicesCombo(soundDevList, ptZhavamConf))
	{
		gtkSetSensitiveRecordToggleButton(FALSE);
		sprintf(statusActivateMsg, "%s", WARNING01);
		gtkSetStatusZhvLabel(statusActivateMsg);
		gtkWarning("%s", WARNING01);
	}
	else gtkSetSensitiveRecordToggleButton(TRUE);

	gtkConfigDialogSetUp(ptZhavamConf);

	if (soundDevList) listDestroy(soundDevList, (void *)NULL);
}

/**
 * Required tasks for zhavam initialization
 * @param ptZhavamConf
 */
void gtkInitZhavam(zhavamConf_t * ptZhavamConf)
{
	char statusActivateMsg[TEXTZHAVAMDO];

	GtkWidget * recordToggleButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "recordToggleButton"));
	gtk_toggle_button_set_active((GtkToggleButton *)recordToggleButton, FALSE);

	gtkInitDevicesComboBoxText(ptZhavamConf);

	if (acrCloudSet(ptZhavamConf)) sprintf(statusActivateMsg, "%s %s %s", "Zhavam", VERSION, STATUS00);
	else sprintf(statusActivateMsg, "%s %s %s", "Zhavam", VERSION, STATUS03);
	gtkSetStatusZhvLabel(statusActivateMsg);
}

/**
 * Required tasks for closing Zhavam
 */
void gtkCloseZhavam(void)
{
	gtkSetCursor(NORMAL_CURSOR);
	gtk_main_quit();
}

/**
 * Zhavam Config Quit menu callback
 * @param menuConfigQuit
 * @param user_data
 */
void gtkZhavamConfigQuit(GtkImageMenuItem * menuConfigQuit, gpointer user_data)
{
	gtkCloseZhavam();
}

/**
 * Zhavam Copy To Clipboard menu callback
 * @param menuConfigCopy
 * @param user_data
 */
void gtkZhavamCopyToClipboard(GtkImageMenuItem * menuConfigCopy, gpointer user_data)
{
	GtkTextBuffer * trackInfoTextBuffer = (GtkTextBuffer*)gtk_builder_get_object(getGtkBuilder(), "trackInfoTextBuffer");

	GtkTextIter	startIter, endIter;

	if (!trackInfoTextBuffer) return;
	gtk_text_buffer_get_bounds(trackInfoTextBuffer, &startIter, &endIter);

	char * textBuffer = gtk_text_buffer_get_text(trackInfoTextBuffer, &startIter, &endIter, false);
	if (!textBuffer) return;
	GtkClipboard * gtkClipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text (gtkClipboard, textBuffer, strlen(textBuffer));
}

/**
 * Zhavam Clear TrackInfo TextView menu callback
 * @param menuInfoClearText
 * @param user_data
 */
void gtkZhavamClearTrackInfoTextView(GtkImageMenuItem * menuInfoClearText, gpointer user_data)
{
	GtkTextBuffer * trackInfoTextBuffer = (GtkTextBuffer*)gtk_builder_get_object(getGtkBuilder(), "trackInfoTextBuffer");
	gtkDeleteTextBuffer(trackInfoTextBuffer);
}

/**
 * Zhavam About menu callback to opens the about dialog
 * @param menuHelpAbout
 * @param user_data
 */
void gtkZhavamAbout(GtkImageMenuItem * menuHelpAbout, gpointer user_data)
{
	GtkWidget * aboutZhavam = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "aboutZhavam"));
	gtk_widget_show_all(aboutZhavam);
}

/**
 * DialogWarningClose callback
 * @param buttonDialogWarning
 * @param user_data
 */
void gtkDialogWarningClose(GtkImageMenuItem * buttonDialogWarning, gpointer user_data)
{
	GtkWidget * dialogWarning = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "dialogWarning"));
	gtk_widget_hide(dialogWarning);
}

/**
 * callback DriverControllerComboBoxTextChange callback
 * @param driverControllerComboBoxText
 * @param user_data
 */
void gtkDriverControllerComboBoxTextChange(GtkComboBoxText * driverControllerComboBoxText, gpointer user_data)
{
	GtkWidget * driverControllerNotebook = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "driverControllerNotebook"));
	zhavamConf_t * ptZhavamConf = getZhavamConf();

	switch (driverControllerDecode(gtkGetdriverControllerComboBoxText())) {
	case ALSA:
		gtk_notebook_set_current_page((GtkNotebook *)driverControllerNotebook, ALSA);
		ptZhavamConf->driverController = ALSA;
		break;
	case PULSE:
		gtk_notebook_set_current_page((GtkNotebook *)driverControllerNotebook, PULSE);
		ptZhavamConf->driverController = PULSE;
		break;
	default:
		TRACE("driverControllerNotebook NOT FOUND %s", gtkGetdriverControllerComboBoxText());
		ptZhavamConf->driverController = UNKNOWN_CONTROLLER;
		break;
	}
	gtkInitDevicesComboBoxText(ptZhavamConf);
}

/**
 * Signals Connect function
 */
void gtkSignalsConnect(void)
{
	GtkWidget * recordToggleButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "recordToggleButton"));
	g_signal_connect(recordToggleButton, "clicked", (GCallback)gtkRecordToggleButtonClickedCallback, (gpointer)NULL );

	GtkWidget * zhavamMainWindow = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "zhavamMainWindow"));
	g_signal_connect(G_OBJECT(zhavamMainWindow), "destroy", G_CALLBACK(gtkCloseZhavam), (gpointer)NULL);

	GtkWidget * menuConfigEdit = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "menuConfigEdit"));
	g_signal_connect(G_OBJECT(menuConfigEdit), "activate", G_CALLBACK(gtkZhavamConfigEdit), (gpointer)NULL);

	GtkWidget * configCloseButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "configCloseButton"));
	g_signal_connect(G_OBJECT(configCloseButton), "clicked", G_CALLBACK(gtkConfigCloseButton), (gpointer)NULL);

	GtkWidget * configCleanButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "configCleanButton"));
	g_signal_connect(G_OBJECT(configCleanButton), "clicked", G_CALLBACK(gtkConfigCleanButton), (gpointer)NULL);

	GtkWidget * configSaveButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "configSaveButton"));
	g_signal_connect(G_OBJECT(configSaveButton), "clicked", G_CALLBACK(gtkConfigSaveButton), (gpointer)NULL);

	GtkWidget * menuConfigReload = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "menuConfigReload"));
	g_signal_connect(G_OBJECT(menuConfigReload), "activate", G_CALLBACK(gtkZhavamConfigReload), (gpointer)NULL);

	GtkWidget * menuConfigSave = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "menuConfigSave"));
	g_signal_connect(G_OBJECT(menuConfigSave), "activate", G_CALLBACK(gtkZhavamConfigSave), (gpointer)NULL);

	GtkWidget * menuConfigQuit = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "menuConfigQuit"));
	g_signal_connect(G_OBJECT(menuConfigQuit), "activate", G_CALLBACK(gtkZhavamConfigQuit), (gpointer)NULL);

	GtkWidget * menuInfoCopy = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "menuInfoCopy"));
	g_signal_connect(G_OBJECT(menuInfoCopy), "activate", G_CALLBACK(gtkZhavamCopyToClipboard), (gpointer)NULL);

	GtkWidget * menuInfoClearText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "menuInfoClearText"));
	g_signal_connect(G_OBJECT(menuInfoClearText), "activate", G_CALLBACK(gtkZhavamClearTrackInfoTextView), (gpointer)NULL);

	GtkWidget * menuHelpAbout = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "menuHelpAbout"));
	g_signal_connect(G_OBJECT(menuHelpAbout), "activate", G_CALLBACK(gtkZhavamAbout), (gpointer)NULL);

	GtkWidget * buttonDialogWarning = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "buttonDialogWarning"));
	g_signal_connect(G_OBJECT(buttonDialogWarning), "clicked", G_CALLBACK(gtkDialogWarningClose), (gpointer)NULL);

	GtkWidget * driverControllerComboBoxText = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "driverControllerComboBoxText"));
	g_signal_connect(G_OBJECT(driverControllerComboBoxText), "changed", G_CALLBACK(gtkDriverControllerComboBoxTextChange), (gpointer)NULL);
}

/**
 * Prints the usage on screen
 */
void zhavamHelp(void)
{
	puts("Usage: zhavam [OPTION]... ");
	puts("Music recognition application for Linux.\n");
	puts("Abstract");
	puts("zhavam connects with Alsa or Pulse Drivers to record a small part of");
	puts("the song and then sends it to ACRCloud. ACRCloud responds with the");
	puts("song data in a Json message. The data recovered from the service");
	puts("includes the title of the song, the artist, the album, the genre of");
	puts("the year, etc.");
	puts("zhavam analyzes this message and presents the most relevant information");
	puts("in the Track Information Text View.\n");
	puts("Options");
	puts("Do not use any option to display the graphical interface.");
	puts("	-g, --nogui	no GUI, command line mode.");
	puts("	-l, --loop	loop mode (forces the no GUI mode)");
	puts("	-h, --help	shows this help.");
	puts("	-c, --config	dumps the configuration on the screen.");
	puts("	-b, --buffer	writes pcm buffer to a file (forces the no GUI mode)");
	puts("	-V, --version	prints zhavam version.");
	puts("zhavam on Github at https://github.com/ipserc/zhavam");
	puts("You can report any bug at zhavam's Github homepage");
}

/**
 * options
 * --nogui		-g	no GUI, command line mode
 * --loop		-l	loop mode. Press ctrl-c to quit
 * --help		-h	help
 * --config		-c	dumps the configuration on the screen.
 * --buffer		-b	writes pcm buffer (forces the no GUI mode)
 * --version	-V	prints zhavam version.
 */
/**
 * Static "private method" to create or get the zhvParams_t * zhvParams static variable. DO NOT CALL IT DIRECTLY
 * @param method Selector for (0) create a new instance or returning the one created
 * @return A pointer to static zhvParams_t zhvParams
 */
/* private */static zhvParams_t * zhvParams(int method)
{
	static zhvParams_t zhvParams;

	if (method == 0)	//SET
	{
		zhvParams.nogui 	= false;
		zhvParams.loop 		= false;
		zhvParams.help 		= false;
		zhvParams.config	= false;
		zhvParams.buffer	= false;
		zhvParams.version	= false;
	}
	return (zhvParams_t *)&zhvParams;
}

/**
 * Sets zhvParams_t zhvParams pointer. Use it to create it
 * @return A pointer to static zhvParams_t zhvParams
 */
zhvParams_t * setZhvParams(void)
{
	return zhvParams(0);
}

/**
 * Returns a pointer to static zhvParams_t zhvParams static variable
 * @return A pointer to static zhvParams_t zhvParams
 */
zhvParams_t * getZhvParams(void)
{
	return zhvParams(1);
}

/**
 * Reads the params passed from the command line
 */
 zhvParams_t * readZhvParams(int argc, char *argv[])
{
    static struct option long_options[] =
      {
        {"nogui",	no_argument,	0,	'g'},
		{"loop",	no_argument,	0,	'l'},
        {"help",  	no_argument,	0,	'h'},
        {"config",	no_argument,	0,	'c'},
        {"buffer",	no_argument,	0,	'b'},
        {"version",	no_argument,	0,	'V'},
        {0, 0, 0, 0}
      };
    int c, option_index = 0;
    zhvParams_t * zhvParams = setZhvParams();
    while ((c = getopt_long (argc, argv, "glhcbV", long_options, &option_index)) != -1)
    {
 		switch (c)
		{
		  case 'h':
			  zhvParams->help = true;
			  break;
		  case 'c':
			  zhvParams->config = true;
			  break;
		  case 'l':
			  zhvParams->loop = true;
			  break;
		  case 'V':
		  	  zhvParams->version = true;
		  	  break;
		  case 'b':
			  zhvParams->buffer = true; // @suppress("No break at end of case")
			  /* no break */
		  case 'g':
			  zhvParams->nogui = true;
			  break;
		}
    }
    return zhvParams;
}

 /**
  * Gest the Sound Device controller name to connect with the Linux sound system via API
  * @param devName The string to store the device name
  * @return Empty device name if the device is not recognized, otherwise the device name
  */
 char * getDevControllerName(char * devName)
 {
 	list_t * soundDevList = listNew(&soundDevList);
 	char * pcmDevice;

 	*devName = '\0';
 	switch (getZhavamConf()->driverController) {
 	case ALSA:
 		soundDevList = alsaGetPCMRecDevicesList(soundDevList);
 		pcmDevice = getZhavamConf()->alsa.pcm_dev;
 		break;
 	case PULSE:
 		soundDevList = pulseGetRecDevicesList(soundDevList);
 		pcmDevice = getZhavamConf()->pulse.pcm_dev;
 		break;
 	default:
 		if (soundDevList) listDestroy(soundDevList, (void *)NULL);
 		WARNING("%s", WARNING17);
 		return devName;
 	}

 	for(node_t * ptr = soundDevList->head; ptr; ptr = ptr->next)
 	{
 		if (!strcmp(((soundDevice_t*)(ptr->item))->description, pcmDevice))
 		{
 			sprintf(devName, "%s", ((soundDevice_t*)(ptr->item))->name);
 			break;
 		}
  	}

 	if (soundDevList) listDestroy(soundDevList, (void *)NULL);
 	return devName;
 }

 /**
  * Runs zhavam in Console Line Interface mode
  * @return The result of doing music recognition
  */
 int zhavamCCI(void)
 {
		acr_data_t acrResponse;
		int retVal;
		char car;
		char trackInfoText[ZHVTRACKINFOTEXTLEN];

		if (getZhvParams()->loop) puts("zhavam is working in loop mode. Press Ctrl-C to quit");
		char devID[LONG_LEN+1];
		if (!getDevControllerName(devID))
		{
			ERROR("%s", ERROR17);
		}

		TRACE("devID:%s", devID);

		do {
			retVal = doZhavam(devID, &acrResponse);
			puts("---- TRACK INFO ----");
			puts(acrDataToText(trackInfoText, &acrResponse));
			puts("--------------------");
			if (getZhvParams()->loop)
			{
				puts("Press Enter to scan again, 'Q' to quit.");
				car = getchar();
				//scanf("%c", &car);
				if (toupper(car) == 'Q') getZhvParams()->loop = false;
			}
		} while(getZhvParams()->loop);
		return retVal;
 }

 /**
  * Runs zhavam in Graphic User interface mode
  * @return The result of doing music recognition
  */
void zhavamGUI(void)
 {
		GtkWidget * zhavamMainWindow;
		// initializes the GtkBuilder static var
		setGtkBuilder();
		// Get main window pointer from UI
		zhavamMainWindow = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "zhavamMainWindow"));
		gtkInitZhavam(getZhavamConf());
		// Connect signals
		gtkSignalsConnect();
		gtk_builder_connect_signals(getGtkBuilder(), NULL);
		/* Show window. All other widgets are automatically shown by GtkBuilder */
		gtk_widget_show(zhavamMainWindow);
		gtkSetCursor(NORMAL_CURSOR);
		gtk_main();
 }

/**
 * main function
 * @param argc
 * @param argv[]
 */
int main(int argc, char * argv[])
{
	zhavamConf_t * ptZhavamConf = setZhavamConf();

	zhavamConfig(ptZhavamConf);
	sprintf(ptZhavamConf->appName, "%s", basename(argv[0]));

	/* ***************************************************** */
	/* 				COMMAND CONSOLE INTERFACE				 */
	/* ***************************************************** */
	if (argc > 1)
	{
		zhvParams_t * zhvParams = readZhvParams(argc, argv);

		if (zhvParams->help)
		{
			zhavamHelp();
			return EXIT_SUCCESS;
		}

		if (zhvParams->config)
		{
			printZhavamConf(getZhavamConf());
			return EXIT_SUCCESS;
		}

		if (zhvParams->version)
		{
			printf("zhavam Version:%s Compilation:%s\n", VERSION, COMPILATION);
			return EXIT_SUCCESS;
		}

		if (zhvParams->buffer || zhvParams->loop || zhvParams->nogui)
		{
			return zhavamCCI();
		}
		TRACE("%s", "option not recognized");
		return EXIT_SUCCESS;
	}
	/* ***************************************************** */
	/* 				GRAPHICAL USER INTERFACE				 */
	/* ***************************************************** */
	gtk_init(&argc, &argv);
	zhavamGUI();
	return EXIT_SUCCESS;
}
