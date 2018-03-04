/*
 * zhavam_devices.h
 *
 *  Created on: 25 ene. 2018
 *      Author: ipserc
 */

#ifndef SOURCE_ZHAVAM_DEVICES_H_
#define SOURCE_ZHAVAM_DEVICES_H_

#include "list.h"
#include <stdbool.h>

extern int errno;
extern char STATUS_MESSAGE[];

#define MAX_LEN_DEV_NAME 30
#define MAX_LEN_PCM_LINE 200
#define PCM_FILE "/proc/asound/pcm"

// 01-00: ALC892 Analog : ALC892 Analog : playback 1 : capture 1
// soundcard number-device number:
/**
 * PCM devices structure
 */
typedef struct {
	int devId;
	char hwDev[MAX_LEN_DEV_NAME];
	int sndCardNbr;
	int deviceNbr;
	char SCDid[MAX_LEN_DEV_NAME];
	char devName[MAX_LEN_DEV_NAME];
	char devSubName[MAX_LEN_DEV_NAME];
	char playCapture[MAX_LEN_DEV_NAME];
	bool capture;
} pcmDev_t;

/*
 * Prototypes
 */
void initPCMDev(pcmDev_t *pcmDev);
void printDevice(pcmDev_t *pcmDevItem);
void split(char *buffer, char *SCDid, char *devName, char *devSubName, char *playCapture);
list_t *getPCMDevices(list_t *pcmDevList);
list_t *getPCMRecDevices(list_t *pcmRecDevList);
list_t *setPcmRecDevList(list_t *pcmDevList, list_t *pcmRecDevList);

#endif /* SOURCE_ZHAVAM_DEVICES_H_ */
