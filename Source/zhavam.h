/*
 * zhavam.h
 *
 *  Created on: 30 ene. 2018
 *      Author: ipserc
 */

#ifndef SOURCE_ZHAVAM_H_
#define SOURCE_ZHAVAM_H_

#include <gtk/gtk.h>
#include "zhavam_config.h"

#define VERSION "1.4"
#define COMPILATION "2020-02-20"

#define NORMAL_CURSOR GDK_LEFT_PTR

#define BASENAMELEN 1001
#define PATHNAMELEN 4001

#define ZHVTRACKINFOTEXTLEN 5000

#define TEXTZHAVAMDO 1001

#define ZHVHOMELEN 50
#define ZHVDIR ".zhavam"
#define ZHVFILENAME "zhavam.conf"
#define ZHVAPPNAME "zhavam"

/**
 * Status messages
 */
#define STATUS00 "Loaded successfully"
#define STATUS01 "Capturing sound from device and recognizing"
#define STATUS02 "Ready to capture again"
#define STATUS03 "acr cloud is not set. Unable to connect"
#define STATUS04 "Device driver controller is not available"
#define STATUS05 "STARTING READING from audio interface..."

#define DEV_COMBO_TEXT_LINE_LEN 50

/**
 * options
 * --nogui		-g	no GUI, command line mode
 * --loop		-l	loop mode. Press ctrl-c to quit
 * --help		-h	help
 * --config		-c	dumps the configuration on the screen.
 * --buffer		-b	writes pcm buffer (forces the no GUI mode)
 * --version	-V	prints zhavam version.
 */

typedef struct
{
	bool nogui;
	bool loop;
	bool help;
	bool config;
	bool version;
	bool buffer;
} zhvParams_t;

/*
 * Prototypes
 */
/* zhavam.c */
GtkBuilder *setGtkBuilder(void);
GtkBuilder *getGtkBuilder(void);
int doZhavam(char *devID, acr_data_t *acrResponse);
char *acrDataToText(char *trackInfoText, acr_data_t *acrResponse);
void gtkDeleteTextBuffer(GtkTextBuffer *textbuf);
void gtkRecordToggleButtonClickedCallback(GtkToggleButton *recordToggleButton, gpointer user_data);
char *gtkGetDevID(void);
void gtkSetDeviceComboBoxText(GtkComboBoxText *devicesComboBoxText, char *pcm_dev);
void gtkSetDefaultDevice(GtkComboBoxText *devicesComboBoxText, zhavamConf_t *ptZhavamConf);
gboolean gtkLoadDevicesCombo(list_t *soundDevList, zhavamConf_t *ptZhavamConf);
void gtkSetSensitiveRecordToggleButton(gboolean sensitive);
void gtkSetStatusZhvLabel(const char *text);
void gtkWarning(const char *fmtstr, ...);
void gtkSetCursor(GdkCursorType cursorType);
_Bool acrCloudSet(zhavamConf_t *ptZhavamConf);
void gtkInitDevicesComboBoxText(zhavamConf_t *ptZhavamConf);
void gtkInitZhavam(zhavamConf_t *ptZhavamConf);
void gtkCloseZhavam(void);
void gtkZhavamConfigQuit(GtkImageMenuItem *menuConfigQuit, gpointer user_data);
void gtkZhavamCopyToClipboard(GtkImageMenuItem *menuConfigCopy, gpointer user_data);
void gtkZhavamClearTrackInfoTextView(GtkImageMenuItem *menuInfoClearText, gpointer user_data);
void gtkZhavamAbout(GtkImageMenuItem *menuHelpAbout, gpointer user_data);
void gtkDialogWarningClose(GtkImageMenuItem *buttonDialogWarning, gpointer user_data);
void gtkDriverControllerComboBoxTextChange(GtkComboBoxText *driverControllerComboBoxText, gpointer user_data);
void gtkSignalsConnect(void);
void zhavamHelp(void);
zhvParams_t *setZhvParams(void);
zhvParams_t *getZhvParams(void);
zhvParams_t *readZhvParams(int argc, char *argv[]);
int zhavamCCI(void);
void zhavamGUI(void);
int main(int argc, char *argv[]);

#endif /* SOURCE_ZHAVAM_H_ */
