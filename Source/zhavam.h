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

#define VERSION "1.2"
#define COMPILATION "2018-06-02"

#define NORMAL_CURSOR GDK_LEFT_PTR

#define BASENAMELEN 1001
#define PATHNAMELEN 4001

#define TEXTZHAVAMDO 1001

#define ZHVHOMELEN 50
#define ZHVDIR ".zhavam"
#define ZHVFILENAME "zhavam.conf"

/*
 * Prototypes
 */
GtkBuilder *newGtkBuilder(void);
GtkBuilder *getGtkBuilder(void);
zhavamConf_t *newZhavamConf(void);
zhavamConf_t *getZhavamConf(void);
int doZhavam(char *devID, acr_data_t *acrResponse);
char *acrDataToText(char *trackInfoText, acr_data_t *acrResponse);
void gtkDeleteTextBuffer(GtkTextBuffer *textbuf);
void gtkRecordToggleButtonClickedCallback(GtkToggleButton *recordToggleButton, gpointer user_data);
char *gtkGetDevID(void);
void gtkSetDefaultDevice(GtkComboBoxText *devicesComboBoxText, zhavamConf_t *ptZhavamConf);
gboolean gtkLoadDevicesCombo(list_t *pcmRecDevList, zhavamConf_t *ptZhavamConf);
void gtkSetSensitiveRecordToggleButton(gboolean sensitive);
void gtkSetStatusZhvLabel(const char *text);
void gtkWarning(const char *fmtstr, ...);
void gtkSetCursor(GdkCursorType cursorType);
_Bool acrCloudSet(zhavamConf_t *ptZhavamConf);
void gtkInitZhavam(zhavamConf_t *ptZhavamConf);
void gtkCloseZhavam(void);
void gtkZhavamConfigQuit(GtkImageMenuItem *menuConfigQuit, gpointer user_data);
void gtkZhavamCopyToClipboard(GtkImageMenuItem *menuConfigCopy, gpointer user_data);
void gtkZhavamClearTrackInfoTextView(GtkImageMenuItem *menuInfoClearText, gpointer user_data);
void gtkZhavamAbout(GtkImageMenuItem *menuHelpAbout, gpointer user_data);
void gtkDialogWarningClose(GtkImageMenuItem *buttonDialogWarning, gpointer user_data);
void gtkSignalsConnect(void);
void createZhavamConf(char *zhvHome, zhavamConf_t *ptZhavamConf);
int configLoad(char *zhvHome, zhavamConf_t *ptZhavamConf);
void initZhavamConfigStruct(zhavamConf_t *ptZhavamConf);
void setupZhavamConfigStruct(zhavamConf_t *ptZhavamConf);
void writeZhavamConfig(char *zhavamHome, zhavamConf_t *ptZhavamConf);
void zhavamConfig(zhavamConf_t *ptZhavamConf);
int main(int argc, char *argv[]);

#endif /* SOURCE_ZHAVAM_H_ */
