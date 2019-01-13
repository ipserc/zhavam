/*
 * zhavam_devices.h
 *
 *  Created on: 25 ene. 2018
 *      Author: ipserc
 */

#ifndef SOURCE_ZHAVAM_DEVICES_H_
#define SOURCE_ZHAVAM_DEVICES_H_

#include <stdbool.h>
#include <pulse/pulseaudio.h>
#include "list.h"

extern int errno;
extern char STATUS_MESSAGE[];

#define ALSA_PCM_FILE "/proc/asound/pcm"

#define PA_READY_FAILED -1
#define PA_READY_WAIT 0
#define PA_READY_OK 1

#define MAX_NUM_DEVICES 16

#define LONG_LEN 511
#define SHORT_LEN 255


// 01-00: ALC892 Analog : ALC892 Analog : playback 1 : capture 1
// soundcard number-device number:
/**
 * Alsa devices structure
 */
typedef struct {
	int devId;
	char hwDev[LONG_LEN+1];
	int sndCardNbr;
	int deviceNbr;
	char SCDid[LONG_LEN+1];
	char devName[LONG_LEN+1];
	char devSubName[LONG_LEN+1];
	char playCapture[LONG_LEN+1];
	bool capture;
} alsaDevice_t;

/**
 * PulseAudio devices structure
 */
// Field list is here: http://0pointer.de/lennart/projects/pulseaudio/doxygen/structpa__sink__info.html
typedef struct {
	uint8_t initialized;
	char name[LONG_LEN+1];
	uint32_t index;
	char description[SHORT_LEN+1];
	uint32_t card;
	char monitor_name[LONG_LEN+1];
} pulseDevice_t;

typedef struct {
	char name[LONG_LEN+1];
	char description[LONG_LEN+1];
}soundDevice_t;


enum {
	init,
	wait,
	done
};

/*
 * Prototypes
 */
/* zhavam_devices.c */
void soundPrintDevice(soundDevice_t *soundDevItem);
void alsaInitPCMDev(alsaDevice_t *pcmDev);
void alsaPrintDevice(alsaDevice_t *pcmDevItem);
list_t *setPcmRecDevList(list_t *pcmDevList, list_t *pcmRecDevList);
void alsaSplit(char *buffer, char *SCDid, char *devName, char *devSubName, char *playCapture);
list_t *alsaGetPCMDevicesList(list_t *pcmDevList);
list_t *alsaGetPCMRecDevicesList(list_t *pcmRecDevList);
void pulseInitDev(pulseDevice_t *pulseDev);
void pulsePrintDevice(pulseDevice_t *pulseDevItem);
void pa_state_cb(pa_context *c, void *userdata);
void pa_sinklist_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata);
void pa_sourcelist_cb(pa_context *c, const pa_source_info *l, int eol, void *userdata);
int pa_get_devicelist(pulseDevice_t *input, pulseDevice_t *output);
list_t *pulseGetRecDevicesList(list_t *pulseRecDevList);

#endif /* SOURCE_ZHAVAM_DEVICES_H_ */
