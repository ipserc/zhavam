/*
 * zhavam_alsa.c
 *
 *  Created on: 6 ene. 2018
 *      Author: ipserc
 *
 *      ./zhavam hw:1,0
 *      export LD_LIBRARY_PATH=/usr/local/lib/acrcloud
 *
 *      https://github.com/acrcloud/acrcloud_sdk_linux_c
 */

#include <gtk/gtk.h>

#include <errno.h>
#include <libconfig.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "jsmn.h"
#include "list.h"

#include "zhavam.h"
#include "zhavam_acrcloud.h"
#include "zhavam_alsa.h"
#include "zhavam_config.h"
#include "zhavam_devices.h"
#include "zhavam_errtra.h"
#include "zhavam_glade.h"
#include "zhavam_jsonparser.h"

/**
 * Status messages
 */
#define STATUS00 "loaded successfully"
#define STATUS01 "Capturing sound from device and recognizing"
#define STATUS02 "Ready to capture again"
#define STATUS03 "acr cloud is not set. Unable to connect"

/**
 * Global Variables GV_
 * No longer required, See "methods" below
 */
//GtkBuilder * GV_builder;
//zhavamConf_t GV_zhavamConf;

/**
 * Static "private method" to create or get the GtkBuilder * gtkbuilder "member" variable. DO NOT CALL IT DIRECTLY
 * @param int method
 * @return static GtkBuilder * gtkbuilder
 */
static GtkBuilder * gtkbuilder(int method)
{
	static GtkBuilder * gtkbuilder;

	if (method == 0)	//NEW
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
 * "Instantiates" a new GtkBuilder * gtkbuilder pointer. Use it to create it
 * @return static GtkBuilder * gtkbuilder
 */
GtkBuilder * newGtkBuilder(void)
{
	return gtkbuilder(0);
}

/**
 * Returns the GtkBuilder * gtkbuilder "member" variable
 * @return static GtkBuilder * gtkbuilder
 */
GtkBuilder * getGtkBuilder(void)
{
	return gtkbuilder(1);
}

/**
 * Static "private method" to create or get the zhavamConf_t * zhavamConf "member" variable. DO NOT CALL IT DIRECTLY
 * @param int method
 * @return static zhavamConf_t * zhavamConf
 */
static zhavamConf_t * zhavamConf(int method)
{
	static zhavamConf_t zhavamConf;

	if (method == 0) //NEW
	{
		initZhavamConfigStruct(&zhavamConf);
	}
	return &zhavamConf;
}

/**
 * "Instantiates" a new zhavamConf_t * zhavamConf "member" pointer. Use it to create it
 * @return static zhavamConf_t * zhavamConf
 */
zhavamConf_t * newZhavamConf()
{
	return (zhavamConf_t *)zhavamConf(0);
}

/**
 * Returns the zhavamConf_t * zhavamConf "member" variable
 * @return static zhavamConf_t * zhavamConf
 */
zhavamConf_t * getZhavamConf()
{
	return (zhavamConf_t *)zhavamConf(1);
}

/**
 * Does all the magic. does Zhavam.
 * opens the device, Sets up Audio Device, PCM prepare, Start Record and recognize the song, Close the device and present the result
 * @param char * devID
 * @param acr_data_t * acrResponse
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int doZhavam(char * devID, acr_data_t * acrResponse)
{
	zhavamConf_t * zhavamConf = getZhavamConf();
	snd_pcm_t * capture_handle = NULL;
	snd_pcm_hw_params_t * hw_params = NULL;
	snd_pcm_format_t format = zhavamConf->alsa.snd_pcm_format; //SND_PCM_FORMAT_S16_LE;
	unsigned int rate = zhavamConf->alsa.rate; //44100;
	unsigned int pcm_buffer_frames = zhavamConf->alsa.pcm_buffer_frames;
	acrcloud_config acrConfig = zhavamConf->acrcloud;

	if (!acrCloudSet(zhavamConf))
	{
		WARNING("%s", WARNING06);
		gtkWarning("%s", WARNING06);
		gtkSetStatusZhvLabel(STATUS03);
		gtkSetCursor(NORMAL_CURSOR);
		return EXIT_FAILURE;
	}
	gtkSetStatusZhvLabel(STATUS01);
	gtkSetCursor(GDK_WATCH);
	if (openDevice(devID, &capture_handle, &hw_params) < 0) return EXIT_FAILURE;
	if (setupAudioDevice(devID, capture_handle, hw_params, format, &rate) < 0) return EXIT_FAILURE;
	if (pcmPrepare(capture_handle) < 0) return EXIT_FAILURE;
	if (startRecord(capture_handle, hw_params, format, rate, pcm_buffer_frames, acrConfig, acrResponse) < 0) return EXIT_FAILURE;
	if (closeDevice(capture_handle) < 0) return EXIT_FAILURE;
	gtkSetStatusZhvLabel(STATUS02);
	gtkSetCursor(NORMAL_CURSOR);
	return EXIT_SUCCESS;
}

/**
 * Converts the ACR response from the acr_data_t structure to a text to be shown in zhavam track info multiline text box,
 * @param char * trackInfoText
 * @param acr_data_t * acrResponse
 * @return char * trackInfoText
 */
char * acrDataToText(char * trackInfoText, acr_data_t * acrResponse)
{
	char * ptr = trackInfoText;

	if (acrResponse->status.code[0] != '0')
	{
		ptr += sprintf(ptr, "status msg:%s\n", acrResponse->status.msg);
		ptr += sprintf(ptr, "status code:%s\n", acrResponse->status.code);
		ptr += sprintf(ptr, "status version:%s\n", acrResponse->status.version);
	}
	else
	{
		//ptr += sprintf(ptr, "------- TRACK INFO %d -------\n", m+1);
		ptr += sprintf(ptr, "Title:%s\n", acrResponse->metadata.music.title);
		for(int i = 0; i < MAX_ITEMS; ++i) {
			if (!acrResponse->metadata.music.artists[i][0]) break;
			ptr += sprintf(ptr, "Artists[%d]:%s\n", i, acrResponse->metadata.music.artists[i]);
		}
		ptr += sprintf(ptr, "Album:%s\n", acrResponse->metadata.music.album);
		for(int i = 0; i < MAX_ITEMS; ++i) {
			if (!acrResponse->metadata.music.genres[i][0]) break;
			ptr += sprintf(ptr, "Genres[%d]:%s\n", i, acrResponse->metadata.music.genres[i]);
		}
		ptr += sprintf(ptr, "Label:%s\n", acrResponse->metadata.music.label);
		ptr += sprintf(ptr, "Release date:%s\n", acrResponse->metadata.music.release_date);
		ptr += sprintf(ptr, "Spotify album id:%s\n", acrResponse->metadata.music.external_metadata.spotify.album_id);
		ptr += sprintf(ptr, "Spotify track id:%s\n", acrResponse->metadata.music.external_metadata.spotify.track_id);
		ptr += sprintf(ptr, "Youtube video id:%s\n", acrResponse->metadata.music.external_metadata.youtube_vid);
		//printf("External ids:%s\n", acrResponse->metadata.music[m].external_ids);
		//printf("Play offset_ms:%s\n", acrResponse->metadata.music[m].play_offset_ms);
		//printf("External metadata:%s\n", acrResponse->metadata.music[m].external_metadata);
		//printf("Duration_ms:%s\n", acrResponse->metadata.music[m].duration_ms);
		//printf("Acrid:%s\n", acrResponse->metadata.music[m].acrid);
		//printf("Result from:%s\n", acrResponse->metadata.music[m].result_from);
		//printf("Score:%s\n", acrResponse->metadata.music[m].score);
		//printf("Timestamp utc:%s\n", acrResponse->metadata.timestamp_utc);
		//printf("Cost time:%s\n", acrResponse->cost_time);
		//printf("Result type:%s\n", acrResponse->result_type);
	}
	return trackInfoText;
}

/**
 * Cleans up the track info text buffer
 * @param GtkTextBuffer * textbuf
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
 * @param GtkToggleButton * recordToggleButton
 * @param gpointer user_data
 */
void gtkRecordToggleButtonClickedCallback(GtkToggleButton * recordToggleButton, gpointer user_data)
{
	acr_data_t acrResponse;
	char trackInfoText[5000];
	GtkTextBuffer * trackInfoTextBuffer = (GtkTextBuffer*)gtk_builder_get_object(getGtkBuilder(), "trackInfoTextBuffer");

	gtkDeleteTextBuffer(trackInfoTextBuffer);
	if (!gtk_toggle_button_get_active(recordToggleButton)) return;
	doZhavam(gtkGetDevID(), &acrResponse);
	gtk_toggle_button_set_active((GtkToggleButton *)recordToggleButton, FALSE);

	acrDataToText(trackInfoText, &acrResponse);
	gtk_text_buffer_set_text((GtkTextBuffer*)trackInfoTextBuffer, trackInfoText, -1);
}

/**
 * Gets the DevID from devicesComboBoxText ans stores it in zhavamConf->alsa.pcm_dev
 * return char * zhavamConf-> alsa.pcm_dev
 */
char * gtkGetDevID(void)
{
	zhavamConf_t * zhavamConf = getZhavamConf();
	GtkComboBoxText * devicesComboBoxText = (GtkComboBoxText*)GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "devicesComboBoxText"));
	sprintf(zhavamConf->alsa.pcm_dev, "%s", gtk_combo_box_get_active_id((GtkComboBox *)devicesComboBoxText));
	return zhavamConf->alsa.pcm_dev;
}

/**
 * Sets the default device in the devicesComboBoxText according with the value stored in ptZhavamConf->alsa.pcm_dev or the first value if NULL
 * @param GtkComboBoxText * devicesComboBoxText
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkSetDefaultDevice(GtkComboBoxText * devicesComboBoxText, zhavamConf_t * ptZhavamConf)
{
	if (ptZhavamConf->alsa.pcm_dev != NULL)
	{
		int index = 0;
		char * comboText;
		do
		{
			gtk_combo_box_set_active((GtkComboBox *)devicesComboBoxText, index++);
			comboText = gtk_combo_box_text_get_active_text((GtkComboBox *)devicesComboBoxText);
			if (comboText)
			{
				if (strstr(comboText, ptZhavamConf->alsa.pcm_dev)) break;
			}
		} while(comboText);
		if (!comboText) gtk_combo_box_set_active((GtkComboBox *)devicesComboBoxText, 0);
		else g_free(comboText);
	}
}

/**
 * Loads the Record Devices found into the devicesComboBoxText and shows the default device
 * @param list_t * pcmRecDevList
 * @param zhavamConf_t * ptZhavamConf
 * @return TRUE if there is a default dev, else FALSE
 */
gboolean gtkLoadDevicesCombo(list_t * pcmRecDevList, zhavamConf_t * ptZhavamConf)
{
	char devComboTextLine[MAX_LEN_DEV_NAME] = "";

	GtkComboBoxText * devicesComboBoxText = (GtkComboBoxText*)GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "devicesComboBoxText"));
	gtk_combo_box_text_remove_all(devicesComboBoxText);

	if (pcmRecDevList->head == NULL) return FALSE; // Empty list

	for(node_t * ptr = pcmRecDevList->head;ptr;ptr = ptr->next)
	{
		sprintf(devComboTextLine, "%s - %s", ((pcmDev_t*)(ptr->item))->hwDev, ((pcmDev_t*)(ptr->item))->devName );
		gtk_combo_box_text_append((GtkComboBoxText*)devicesComboBoxText, ((pcmDev_t*)(ptr->item))->hwDev, devComboTextLine);
	}
	gtk_combo_box_set_active((GtkComboBox *)(GtkComboBoxText*)devicesComboBoxText, 0);

	gtkSetDefaultDevice(devicesComboBoxText, ptZhavamConf);
	return TRUE;
}

/**
 * Sets Sensitive RecordToggleButton status
 * @param gboolean sensitive
 */
void gtkSetSensitiveRecordToggleButton(gboolean sensitive)
{
	GtkWidget * recordToggleButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "recordToggleButton"));
	gtk_widget_set_sensitive(recordToggleButton, sensitive);
}

/**
 * Sets StatusZhvLabel with the text passed
 * @param const char * text
 */
void gtkSetStatusZhvLabel(const char * text)
{
	GtkLabel * statusZhvLabel = GTK_LABEL(gtk_builder_get_object(getGtkBuilder(), "statusZhvLabel"));
	gtk_label_set_text(statusZhvLabel, text);
	while (gtk_events_pending()) gtk_main_iteration();
}

/**
 * Opens the warning dialog with the message passed as parameter
 * @param const char * fmtstr
 * @param ...
 */
void gtkWarning(const char * fmtstr,...)
{
	char fullWarningMsg[WARNINGMSGLEN];
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
 * @param zhavamConf_t * ptZhavamConf
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
 * Required tasks for zhavam initialization
 * @param zhavamConf_t * ptZhavamConf
 */
void gtkInitZhavam(zhavamConf_t * ptZhavamConf)
{
	char statusActivateMsg[TEXTZHAVAMDO];

	GtkWidget * recordToggleButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "recordToggleButton"));
	gtk_toggle_button_set_active((GtkToggleButton *)recordToggleButton, FALSE);

	if (acrCloudSet(ptZhavamConf)) sprintf(statusActivateMsg, "%s %s %s", "Zhavam", VERSION, STATUS00);
	else sprintf(statusActivateMsg, "%s %s %s", "Zhavam", VERSION, STATUS03);
	gtkSetStatusZhvLabel(statusActivateMsg);

	list_t * pcmRecDevList;
	pcmRecDevList = listNew(pcmRecDevList);
	getPCMRecDevices(pcmRecDevList);
	// Load the capture sound devices in the devices combobox
	if (!gtkLoadDevicesCombo(pcmRecDevList, ptZhavamConf))
	{
		gtkSetSensitiveRecordToggleButton(FALSE);
		sprintf(statusActivateMsg, "%s", WARNING01);
		gtkSetStatusZhvLabel(statusActivateMsg);
		gtkWarning("%s", WARNING01);
	}
	else gtkSetSensitiveRecordToggleButton(TRUE);

	gtkConfigDialogSetUp(ptZhavamConf, pcmRecDevList);

	listDestroy(pcmRecDevList);
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
 * @param GtkImageMenuItem * menuConfigQuit
 * @param  gpointer user_data
 */
void gtkZhavamConfigQuit(GtkImageMenuItem * menuConfigQuit, gpointer user_data)
{
	gtkCloseZhavam();
}

/**
 * Zhavam Copy To Clipboard menu callback
 * @param GtkImageMenuItem * menuConfigCopy
 * @param gpointer user_data
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
 * @param GtkImageMenuItem * menuInfoClearText
 * @param gpointer user_data
 */
void gtkZhavamClearTrackInfoTextView(GtkImageMenuItem * menuInfoClearText, gpointer user_data)
{
	GtkTextBuffer * trackInfoTextBuffer = (GtkTextBuffer*)gtk_builder_get_object(getGtkBuilder(), "trackInfoTextBuffer");
	gtkDeleteTextBuffer(trackInfoTextBuffer);
}

/**
 * Zhavam About menu callback to opens the about dialog
 * @param GtkImageMenuItem * menuHelpAbout
 * @param gpointer user_data
 */
void gtkZhavamAbout(GtkImageMenuItem * menuHelpAbout, gpointer user_data)
{
	GtkWidget * aboutZhavam = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "aboutZhavam"));
	gtk_widget_show_all(aboutZhavam);
}

/**
 * DialogWarningClose callback
 * @param GtkImageMenuItem * buttonDialogWarning
 * @param gpointer user_data
 */
void gtkDialogWarningClose(GtkImageMenuItem * buttonDialogWarning, gpointer user_data)
{
	GtkWidget * dialogWarning = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "dialogWarning"));
	gtk_widget_hide(dialogWarning);
}

/**
 * Signals Connect function
 */
void gtkSignalsConnect(void)
{
	GtkWidget * recordToggleButton = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "recordToggleButton"));
	g_signal_connect(recordToggleButton, "clicked", (GCallback)gtkRecordToggleButtonClickedCallback, NULL );

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
}

/**
 * Creates the file zhavam.conf from scratch
 * first set up the default values for acrcloud and alsa
 * second writes these values in a new zhavam.conf file
 * @param char * zhvHome
 * @param zhavamConf_t * ptZhavamConf
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
 * @param char * zhvHome
 * @param zhavamConf_t * ptZhavamConf
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

	char * str;
	config_lookup_string(&cfg, "acrcloud.access_key", &(ptZhavamConf->acrcloud.access_key_));
	config_lookup_string(&cfg, "acrcloud.access_secret", &(ptZhavamConf->acrcloud.access_secret_));
	config_lookup_string(&cfg, "acrcloud.host", &(ptZhavamConf->acrcloud.host_));
	config_lookup_string(&cfg, "acrcloud.rec_type", &str);
	ptZhavamConf->acrcloud.rec_type_ = recTypeDecode(str);
	config_lookup_int(&cfg, "acrcloud.timeout_ms", &(ptZhavamConf->acrcloud.timeout_ms_));

	config_lookup_string(&cfg, "alsa.snd_pcm_format", &(str));
	ptZhavamConf->alsa.snd_pcm_format = sndPcmFormatDecode(str);
	config_lookup_string(&cfg, "alsa.pcm_dev", &(ptZhavamConf->alsa.pcm_dev));
	config_lookup_int(&cfg, "alsa.pcm_buffer_frames", &(ptZhavamConf->alsa.pcm_buffer_frames));
	config_lookup_int(&cfg, "alsa.rate", &(ptZhavamConf->alsa.rate));

	return EXIT_SUCCESS;
}

/**
 * Initializes Zhavam ConfigStruct
 * @param zhavamConf_t * ptZhavamConf
 */
void initZhavamConfigStruct(zhavamConf_t * ptZhavamConf)
{
	ptZhavamConf->acrcloud.access_key_ = NULL;
	ptZhavamConf->acrcloud.access_secret_= NULL;
	ptZhavamConf->acrcloud.host_ = NULL;
	ptZhavamConf->acrcloud.rec_type_ = -1;
	ptZhavamConf->acrcloud.timeout_ms_ = -1;

	ptZhavamConf->alsa.pcm_buffer_frames = 0;
	ptZhavamConf->alsa.pcm_dev = NULL;
	ptZhavamConf->alsa.rate = 0;
	ptZhavamConf->alsa.snd_pcm_format = -1;
}

/**
 * Sets up Zhavam ConfigStruct to the default values
 * @param zhavamConf_t * ptZhavamConf
 */
void setupZhavamConfigStruct(zhavamConf_t * ptZhavamConf)
{
	ptZhavamConf->acrcloud.access_key_ = NULL;
	ptZhavamConf->acrcloud.access_secret_= NULL;
	ptZhavamConf->acrcloud.host_ = NULL;
	ptZhavamConf->acrcloud.rec_type_ = acr_opt_rec_audio;
	ptZhavamConf->acrcloud.timeout_ms_ = 5000;

	ptZhavamConf->alsa.pcm_buffer_frames = 153600;
	ptZhavamConf->alsa.pcm_dev = NULL;
	ptZhavamConf->alsa.rate = 44100;
	ptZhavamConf->alsa.snd_pcm_format = SND_PCM_FORMAT_S16;
}

/**
 * Writes Zhavam Configuration kept in ptZhavamConf to zhavam.conf
 * @param char * zhavamHome
 * @param zhavamConf_t * ptZhavamConf
 */
void writeZhavamConfig(char * zhavamHome, zhavamConf_t * ptZhavamConf)
{
	FILE * fp;

	if((fp = fopen(zhavamHome, "w")))
	{
		char str[100];
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
		fprintf(fp, "	rec_type = \"%s\";\n", recTypeString(str, ptZhavamConf->acrcloud.rec_type_));
		fprintf(fp, "};\n\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "# alsa config section\n");
		fprintf(fp, "# ######################\n");
		fprintf(fp, "alsa:\n");
		fprintf(fp, "{\n");
		fprintf(fp, "	snd_pcm_format = \"%s\";\n", pcmFormatString(str, ptZhavamConf->alsa.snd_pcm_format));
		fprintf(fp, "	rate = %d;\n", ptZhavamConf->alsa.rate);
		fprintf(fp, "	pcm_buffer_frames = %d;\n", ptZhavamConf->alsa.pcm_buffer_frames);
		fprintf(fp, "	pcm_dev = \"%s\";\n", ptZhavamConf->alsa.pcm_dev ? ptZhavamConf->alsa.pcm_dev : "");
		fprintf(fp, "};\n");
		fclose(fp);
	}
	else {
		WARNING(WARNING05, zhavamHome);
		gtkWarning(WARNING05, zhavamHome);
	}
}

/**
 * Try to read the config file zhavam.conf
 * If this file doesn't exist calls createZhavamConf with the full path to create the file
 * If the file exists loads its content
 * @parm zhavamConf_t * ptZhavamConf
 */
void zhavamConfig(zhavamConf_t * ptZhavamConf)
{
	struct stat s;
	char zhvHome[2*ZHVHOMELEN];
	char home[ZHVHOMELEN];

	initZhavamConfigStruct(ptZhavamConf);

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

/**
 * main function
 * @param int argc
 * @param char * argv[]
 */
int main(int argc, char * argv[])
{
	zhavamConf_t * ptZhavamConf = newZhavamConf();

	if (argc > 1)
	{
		//@TODO hay que reconstruir todo el modo comando. Por ahora lo anulo
		return 0;
		ptZhavamConf->alsa.pcm_dev = argv[1];
		zhavamConfig(ptZhavamConf);
		ptZhavamConf->alsa.pcm_dev = argv[1];
		// Console User Interface (CUI)
		// char * devID = argv[1];
		acr_data_t acrResponse;
		int retVal;
		retVal = doZhavam(ptZhavamConf->alsa.pcm_dev, &acrResponse);
		printAcrData(&acrResponse);
		return retVal;
	}
	// else Graphic User Interface GUI
	zhavamConfig(ptZhavamConf);
	char statusActivateMsg[TEXTZHAVAMDO];
	GtkWidget * zhavamMainWindow;
	GError * error = NULL;

	gtk_init(&argc, &argv);

	// Create new GtkBuilder object
	newGtkBuilder();

	// Get main window pointer from UI
	zhavamMainWindow = GTK_WIDGET(gtk_builder_get_object(getGtkBuilder(), "zhavamMainWindow"));

	gtkInitZhavam(ptZhavamConf);

	// Connect signals
	gtkSignalsConnect();
	gtk_builder_connect_signals(getGtkBuilder(), NULL);

	/* Show window. All other widgets are automatically shown by GtkBuilder */
	gtk_widget_show(zhavamMainWindow);

	gtkSetCursor(NORMAL_CURSOR);
	gtk_main();

	return EXIT_SUCCESS;
}
