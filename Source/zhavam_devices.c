/*
 * zhavam_devices.c
 *
 *  Created on: 23 ene. 2018
 *      Author: ipserc
 */

#include <stdio.h>
#include <string.h>

#include "jsmn.h"
#include "list.h"
#include "acrcloud_recognizer.h"

#include "zhavam.h"
#include "zhavam_alsa.h"
#include "zhavam_config.h"
#include "zhavam_devices.h"
#include "zhavam_errtra.h"
#include "zhavam_jsonparser.h"
#include "zhavam_acrcloud.h"

/**
 * Initializes the pcmDev_t structure with the initial data
 * @param pcmDev_t * pcmDev
 */
void initPCMDev(pcmDev_t * pcmDev)
{
	pcmDev->hwDev[0] = '\0';
	pcmDev->sndCardNbr = -1;
	pcmDev->deviceNbr = -1;
	pcmDev->SCDid[0] = '\0';
	pcmDev->devName[0] = '\0';
	pcmDev->devSubName[0] = '\0';
	pcmDev->playCapture[0] = '\0';
	pcmDev->capture = false;
}

/**
 * Prints the content of pcmDevItem
 * Used for debug & trace purposes
 * @param pcmDev_t * pcmDevItem
 */
void printDevice(pcmDev_t * pcmDevItem)
{
	printf("----------------------\n");
	printf("PCM Dev devId      :%d\n", pcmDevItem->devId);
	printf("PCM Dev SCDid      :%s\n", pcmDevItem->SCDid);
	printf("PCM Dev hwDev      :%s\n", pcmDevItem->hwDev);
	printf("PCM Dev sndCardNbr :%d\n", pcmDevItem->sndCardNbr);
	printf("PCM Dev deviceNbr  :%d\n", pcmDevItem->deviceNbr);
	printf("PCM Dev devName    :%s\n", pcmDevItem->devName);
	printf("PCM Dev devSubName :%s\n", pcmDevItem->devSubName);
	printf("PCM Dev playCapture:%s\n", pcmDevItem->playCapture);
	printf("PCM Dev capture    :%s\n", pcmDevItem->capture ? "TRUE" : "FALSE");

}

/* ************************************************
void printAllDevices(pcmDev_t * pcmDev)
{
	pcmDev_t * pcmDevItem;
	for(int i = 0; i < MAX_NUN_DEVS; ++i)
	{
		printf("----------------------\n");
		printf("PCM Dev list nbr---:%d\n", i);
		pcmDevItem = &pcmDev[i];
		printDevice(pcmDevItem);
		if (i+1 < MAX_NUN_DEVS)
			if (pcmDev[i+1].sndCardNbr == -1) break;
	}
}
************************************************ */

/**
 * Splits the device information in separated fields: SCDid, devName, devSubName, playCapture
 * @param char * buffer
 * @param char * SCDid
 * @param char * devName
 * @param char * devSubName
 * @param char * playCapture
 */
void split(char * buffer, char * SCDid, char * devName, char * devSubName, char * playCapture)
{
	char * ptr = buffer;
	int i = 0;

	while (*ptr != ':') SCDid[i++] = *ptr++;
	SCDid[i] = '\0';

	ptr += 2; i = 0;
	while (*ptr != ':') devName[i++] = *ptr++;
	devName[i] = '\0';

	ptr += 2; i = 0;
	while (*ptr != ':') devSubName[i++] = *ptr++;
	devSubName[i] = '\0';

	ptr += 2;
	sprintf(playCapture, "%s", ptr);
}

/**
 * Gets the PCM devices stored in /proc/asound/pcm
 * Kept for historic purposes
 * @param list_t * pcmDevList
 * @return list_t * pcmDevList
 */
list_t * getPCMDevices(list_t * pcmDevList)
{
	FILE * file;
	pcmDev_t pcmDev;

	char buffer[MAX_LEN_PCM_LINE];
	if (!(file = fopen(PCM_FILE, "r"))) {
		sprintf(STATUS_MESSAGE, "cannot open the PCM file %s", PCM_FILE);
		ERROR("%s", STATUS_MESSAGE);
		return pcmDevList;
	}

	while (fgets(buffer, MAX_LEN_PCM_LINE, file))
	{
		int buflen = strlen(buffer)-1; buflen = buflen >= 0 ? buflen : 0;
		buffer[buflen] = '\0';
		printf("buffer:%s:\n", buffer);
		initPCMDev(&pcmDev);
		split(buffer, pcmDev.SCDid, pcmDev.devName, pcmDev.devSubName, pcmDev.playCapture);
		sscanf(pcmDev.SCDid, "%d-%d", &pcmDev.sndCardNbr, &pcmDev.deviceNbr);
		pcmDev.devId = pcmDev.sndCardNbr * 100 + pcmDev.deviceNbr;
		sprintf(pcmDev.hwDev, "hw:%d,%d", pcmDev.sndCardNbr, pcmDev.deviceNbr);
		pcmDev.capture = strstr(pcmDev.playCapture, "capture") ? true : false;
		pcmDevList = listAppend(pcmDevList, &pcmDev, sizeof(pcmDev_t));
	}
	return pcmDevList;
}

/**
 * Gets the PCM RECORDING devices stored in /proc/asound/pcm
 * @param list_t * pcmDevList
 * @return list_t * pcmDevList
 */
list_t * getPCMRecDevices(list_t * pcmRecDevList)
{
	FILE * file;
	pcmDev_t pcmDev;

	char buffer[MAX_LEN_PCM_LINE];
	if (!(file = fopen(PCM_FILE, "r"))) {
		sprintf(STATUS_MESSAGE, "cannot open the PCM file %s", PCM_FILE);
		ERROR("%s", STATUS_MESSAGE);
		return pcmRecDevList;
	}

	while (fgets(buffer, MAX_LEN_PCM_LINE, file))
	{
		int buflen = strlen(buffer)-1; buflen = buflen >= 0 ? buflen : 0;
		buffer[buflen] = '\0';
		initPCMDev(&pcmDev);
		split(buffer, pcmDev.SCDid, pcmDev.devName, pcmDev.devSubName, pcmDev.playCapture);
		sscanf(pcmDev.SCDid, "%d-%d", &pcmDev.sndCardNbr, &pcmDev.deviceNbr);
		pcmDev.devId = pcmDev.sndCardNbr * 100 + pcmDev.deviceNbr;
		sprintf(pcmDev.hwDev, "hw:%d,%d", pcmDev.sndCardNbr, pcmDev.deviceNbr);
		pcmDev.capture = strstr(pcmDev.playCapture, "capture") ? true : false;
		if (pcmDev.capture)
			pcmRecDevList = listAppend(pcmRecDevList, &pcmDev, sizeof(pcmDev_t));
	}
	return pcmRecDevList;
}

/**
 * Sets the pcmRecDevList from the values kept in the pcmDevList
 * No longer used, kept for historic purposes
 * @param list_t * pcmDevList
 * @param list_t * pcmRecDevList
 * @return list_t * pcmRecDevList
 */
list_t *  setPcmRecDevList(list_t * pcmDevList, list_t * pcmRecDevList)
{
	bool captureDevs = false;
	for(node_t * ptr = pcmDevList->head;ptr;ptr = ptr->next)
	{
		if (((pcmDev_t*)(ptr->item))->capture)
		{
			captureDevs = true;
			pcmRecDevList = listAppend(pcmRecDevList, ptr->item, sizeof(pcmDev_t));
		}
	}
	return pcmRecDevList;
}


