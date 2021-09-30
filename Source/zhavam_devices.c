/*
 * zhavam_devices.c
 *
 *  Created on: 23 ene. 2018
 *      Author: ipserc
 *
 *      This functions were taken from https://gist.github.com/andrewrk/6470f3786d05999fcb48
 *      	void pa_state_cb(pa_context *c, void *userdata);
 *			void pa_sinklist_cb(pa_context *c, const pa_sink_info *l, int eol, void *userdata);
 *			void pa_sourcelist_cb(pa_context *c, const pa_source_info *l, int eol, void *userdata);
 *			int pa_get_devicelist(pa_devicelist_t *input, pa_devicelist_t *output);
 */

#include <stdio.h>
#include <string.h>

#include "acrcloud_recognizer.h"
#include "zhavam_devices.h"
#include "zhavam_errtra.h"

/**
 * Prints to the screen the soundDevice_t structure with the initial data
 * @param soundDevItem
 */
void soundPrintDevice(soundDevice_t * soundDevItem)
{
	printf("--------------------------\n");
	printf("Sound Dev name         :%s\n", soundDevItem->name);
	printf("Sound Dev description  :%s\n", soundDevItem->description);
}

/**
 * Initializes the alsaDevice_t structure with the initial data
 * @param pcmDev
 */
void alsaInitPCMDev(alsaDevice_t * pcmDev)
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
 * @param pcmDevItem
 */
void alsaPrintDevice(alsaDevice_t * pcmDevItem)
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

/* ************************************************ * /
void alsaPrintAllDevices(alsaDevice_t * pcmDev)
{
	alsaDevice_t * pcmDevItem;
	for(int i = 0; i < MAX_NUM_DEVICES; ++i)
	{
		printf("----------------------\n");
		printf("PCM Dev list nbr---:%d\n", i);
		pcmDevItem = &pcmDev[i];
		printDevice(pcmDevItem);
		if (i+1 < MAX_NUM_DEVICES)
			if (pcmDev[i+1].sndCardNbr == -1) break;
	}
}
/ * ************************************************ */

/**
 * Sets the pcmRecDevList from the values kept in the pcmDevList
 * No longer used, kept for historic purposes
 * @param pcmDevList
 * @param pcmRecDevList
 * @return list_t * pcmRecDevList
 */
list_t *  setPcmRecDevList(list_t * pcmDevList, list_t * pcmRecDevList)
{
	for(node_t * ptr = pcmDevList->head;ptr;ptr = ptr->next)
	{
		if (((alsaDevice_t*)(ptr->item))->capture)
		{
			pcmRecDevList = listAppend(pcmRecDevList, ptr->item, sizeof(alsaDevice_t));
		}
	}
	return pcmRecDevList;
}

/**
 * Splits the device information in separated fields: SCDid, devName, devSubName, playCapture
 * @param buffer
 * @param SCDid
 * @param devName
 * @param devSubName
 * @param playCapture
 */
void alsaSplit(char * buffer, char * SCDid, char * devName, char * devSubName, char * playCapture)
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
 * Gets the PCM devices stored in /proc/asound/pcm ALSA_PCM_FILE
 * Kept for historic purposes
 * @param pcmDevList
 * @return list_t * pcmDevList
 */
list_t * alsaGetPCMDevicesList(list_t * pcmDevList)
{
	FILE * file;
	alsaDevice_t pcmDev;

	char buffer[LONG_LEN];
	if (!(file = fopen(ALSA_PCM_FILE, "r"))) {
		sprintf(STATUS_MESSAGE, "cannot open the PCM file %s", ALSA_PCM_FILE);
		ERROR("%s", STATUS_MESSAGE);
		return pcmDevList;
	}

	while (fgets(buffer, LONG_LEN, file))
	{
		int buflen = strlen(buffer)-1; buflen = buflen >= 0 ? buflen : 0;
		buffer[buflen] = '\0';
		printf("buffer:%s:\n", buffer);
		alsaInitPCMDev(&pcmDev);
		alsaSplit(buffer, pcmDev.SCDid, pcmDev.devName, pcmDev.devSubName, pcmDev.playCapture);
		sscanf(pcmDev.SCDid, "%d-%d", &pcmDev.sndCardNbr, &pcmDev.deviceNbr);
		pcmDev.devId = pcmDev.sndCardNbr * 100 + pcmDev.deviceNbr;
		sprintf(pcmDev.hwDev, "hw:%d,%d", pcmDev.sndCardNbr, pcmDev.deviceNbr);
		pcmDev.capture = strstr(pcmDev.playCapture, "capture") ? true : false;
		pcmDevList = listAppend(pcmDevList, &pcmDev, sizeof(alsaDevice_t));
	}
	fclose(file);
	return pcmDevList;
}

/**
 * Gets the PCM RECORDING devices stored in /proc/asound/pcm
 * @param pcmDevList
 * @return list_t * pcmDevList
 */
list_t * alsaGetPCMRecDevicesList(list_t * pcmRecDevList)
{
	FILE * file;
	alsaDevice_t pcmDev;
	soundDevice_t soundDev;

	char buffer[LONG_LEN];
	if (!(file = fopen(ALSA_PCM_FILE, "r"))) {
		sprintf(STATUS_MESSAGE, "cannot open the PCM file %s", ALSA_PCM_FILE);
		ERROR("%s", STATUS_MESSAGE);
		return pcmRecDevList;
	}

	while (fgets(buffer, LONG_LEN, file))
	{
		int buflen = strlen(buffer)-1; buflen = buflen >= 0 ? buflen : 0;
		buffer[buflen] = '\0';
		alsaInitPCMDev(&pcmDev);
		alsaSplit(buffer, pcmDev.SCDid, pcmDev.devName, pcmDev.devSubName, pcmDev.playCapture);
		sscanf(pcmDev.SCDid, "%d-%d", &pcmDev.sndCardNbr, &pcmDev.deviceNbr);
		pcmDev.devId = pcmDev.sndCardNbr * 100 + pcmDev.deviceNbr;
		sprintf(pcmDev.hwDev, "hw:%d,%d", pcmDev.sndCardNbr, pcmDev.deviceNbr);
		pcmDev.capture = strstr(pcmDev.playCapture, "capture") ? true : false;
		//alsaPrintDevice(&pcmDev);
		if (pcmDev.capture) {
			//alsaPrintDevice(&pcmDev);
			//pcmRecDevList = listAppend(pcmRecDevList, &pcmDev, sizeof(alsaDevice_t));
			strncpy(soundDev.name, pcmDev.hwDev, LONG_LEN);
			//strncpy(soundDev.description, pcmDev.devName, LONG_LEN);
			strncpy(soundDev.description, pcmDev.devName, DEV_COMBO_TEXT_LINE_LEN);
			soundDev.description[DEV_COMBO_TEXT_LINE_LEN-1] = 0;
			pcmRecDevList = listAppend(pcmRecDevList, &soundDev, sizeof(soundDevice_t));
		}
	}
	fclose(file);
	return pcmRecDevList;
}

/**
 * Initializes the pulseDevice_t structure with the initial data
 * @param pulseDev
 */
void pulseInitDev(pulseDevice_t * pulseDev)
{
	pulseDev->description[0] = '\0';
	pulseDev->monitor_name[0] = '\0';
	pulseDev->name[0] = '\0';
	pulseDev->card = -1;
	pulseDev->index = -1;
	pulseDev->initialized = -1;
}

/**
 * Prints the content of pulseDevItem
 * Used for debug & trace purposes
 * @param pulseDevItem
 */
void pulsePrintDevice(pulseDevice_t * pulseDevItem)
{
	printf("--------------------------\n");
	printf("Pulse Dev name         :%s\n", pulseDevItem->name);
	printf("Pulse Dev description  :%s\n", pulseDevItem->description);
	printf("Pulse Dev monitor_name :%s\n", pulseDevItem->monitor_name);
	printf("Pulse Dev card         :%d\n", pulseDevItem->card);
	printf("Pulse Dev index        :%d\n", pulseDevItem->index);
	printf("Pulse Dev initialized  :%s\n", pulseDevItem->initialized ? "TRUE" : "FALSE");
}

// This callback gets called when our context changes state.  We really only
// care about when it's ready or if it has failed
void pa_state_cb(pa_context * c, void * userdata)
{
	pa_context_state_t state;
	int * pa_ready = userdata;

	state = pa_context_get_state(c);
	switch (state)
	{
		// There are just here for reference
		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
		default:
			*pa_ready = PA_READY_WAIT;
			break;
		case PA_CONTEXT_FAILED:
		case PA_CONTEXT_TERMINATED:
			*pa_ready = PA_READY_FAILED;
			break;
		case PA_CONTEXT_READY:
			*pa_ready = PA_READY_OK;
			break;
	}
}

// pa_mainloop will call this function when it's ready to tell us about a sink.
// Since we're not threading, there's no need for mutexes on the devicelist
// structure
void pa_sinklist_cb(pa_context * c, const pa_sink_info * l, int eol, void * userdata)
{
    pulseDevice_t * pa_device = userdata;
    int ctr = 0;

    // If eol is set to a positive number, you're at the end of the list
    if (eol > 0) {
    	return;
    }

    // We know we've allocated 16 slots to hold devices.  Loop through our
    // structure and find the first one that's "uninitialized."  Copy the
    // contents into it and we're done.  If we receive more than 16 devices,
    // they're going to get dropped.  You could make this dynamically allocate
    // space for the device list, but this is a simple example.
    for (ctr = 0; ctr < MAX_NUM_DEVICES; ctr++)
    {
		if (! pa_device[ctr].initialized)
		{
			strncpy(pa_device[ctr].name, l->name, LONG_LEN);
			strncpy(pa_device[ctr].description, l->description, SHORT_LEN);
			if (l->monitor_source == PA_INVALID_INDEX)
			{
				strncpy(pa_device[ctr].monitor_name, "No Source Monitor", LONG_LEN);
			}
			else
			{
				strncpy(pa_device[ctr].monitor_name, l->monitor_source_name, LONG_LEN);
			}
			pa_device[ctr].index = l->index;
			pa_device[ctr].card = l->card;
			pa_device[ctr].initialized = 1;
			break;
		}
    }
}

// See above. This callback is pretty much identical to the previous
void pa_sourcelist_cb(pa_context * c, const pa_source_info * l, int eol, void * userdata)
{
    pulseDevice_t * pa_device = userdata;
    int ctr = 0;

    if (eol > 0)
    {
    	return;
    }

    for (ctr = 0; ctr < MAX_NUM_DEVICES; ctr++)
    {
		if (! pa_device[ctr].initialized)
		{
			strncpy(pa_device[ctr].name, l->name, LONG_LEN);
			strncpy(pa_device[ctr].description, l->description, SHORT_LEN);
			if (l->monitor_of_sink == PA_INVALID_INDEX)
			{
				strncpy(pa_device[ctr].monitor_name, "No Sink Monitor", LONG_LEN);
			}
			else
			{
				strncpy(pa_device[ctr].monitor_name, l->monitor_of_sink_name, LONG_LEN);
			}
			pa_device[ctr].index = l->index;
			pa_device[ctr].card = l->card;
			pa_device[ctr].initialized = 1;
			break;
		}
    }
}

/**
 * Gets the Pulse Recording devices from the API
 * @param pulseDevice_t * input Array of INPUT MAX_NUM_DEVICES
 * @param pulseDevice_t * output Array of OUTPUT MAX_NUM_DEVICES
 * @return int state The final state of the process
 */
int pa_get_devicelist(pulseDevice_t * input, pulseDevice_t * output)
{
    // Define our pulse audio loop and connection variables
    pa_mainloop * pa_ml;
    pa_mainloop_api * pa_mlapi;
    pa_operation * pa_op;
    pa_context * pa_ctx;

    // We'll need these state variables to keep track of our requests
    int state = 0;
    int pa_ready = PA_READY_WAIT;

    // Initialize our device lists
    memset(input, 0, sizeof(pulseDevice_t) * MAX_NUM_DEVICES);
    memset(output, 0, sizeof(pulseDevice_t) * MAX_NUM_DEVICES);

    // Create a mainloop API and connection to the default server
    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    pa_ctx = pa_context_new(pa_mlapi, "test");

    // This function connects to the pulse server
    pa_context_connect(pa_ctx, NULL, 0, NULL);


    // This function defines a callback so the server will tell us it's state.
    // Our callback will wait for the state to be ready.  The callback will
    // modify the variable to 1 so we know when we have a connection and it's
    // ready.
    // If there's an error, the callback will set pa_ready to 2
    pa_context_set_state_callback(pa_ctx, pa_state_cb, &pa_ready);

    // Now we'll enter into an infinite loop until we get the data we receive
    // or if there's an error
    // for (;;) is the hacker way for an infinite loop
    while (true)
    {
		// We can't do anything until PA is ready, so just iterate the mainloop
		// and continue
		if (pa_ready == PA_READY_WAIT)
		{
			pa_mainloop_iterate(pa_ml, 1, NULL);
			continue;
		}
		// We couldn't get a connection to the server, so exit out
		if (pa_ready == PA_READY_FAILED)
		{
			pa_context_disconnect(pa_ctx);
			pa_context_unref(pa_ctx);
			pa_mainloop_free(pa_ml);
			return -1;
		}
		// At this point, we're connected to the server and ready to make
		// requests
		switch (state)
		{
			// State 0: we haven't done anything yet
			case init:
				// This sends an operation to the server.  pa_sinklist_info is
				// our callback function and a pointer to our devicelist will
				// be passed to the callback The operation ID is stored in the
				// pa_op variable
				pa_op = pa_context_get_sink_info_list(pa_ctx,
					pa_sinklist_cb,
					output
					);

				// Update state for next iteration through the loop
				++state;
				break;
			case wait:
				// Now we wait for our operation to complete.  When it's
				// complete our pa_output_devicelist is filled out, and we move
				// along to the next state
				if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
				{
					pa_operation_unref(pa_op);

					// Now we perform another operation to get the source
					// (input device) list just like before.  This time we pass
					// a pointer to our input structure
					pa_op = pa_context_get_source_info_list(pa_ctx,
						pa_sourcelist_cb,
						input
						);
					// Update the state so we know what to do next
					++state;
				}
				break;
			case done:
				if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
				{
					// Now we're done, clean up and disconnect and return
					pa_operation_unref(pa_op);
					pa_context_disconnect(pa_ctx);
					pa_context_unref(pa_ctx);
					pa_mainloop_free(pa_ml);
					return 0;
				}
				break;
			default:
				// We should never see this state
				TRACE("in state unknown %d", state);
				return -1;
		}
		// Iterate the main loop and go again.  The second argument is whether
		// or not the iteration should block until something is ready to be
		// done.  Set it to zero for non-blocking.
		pa_mainloop_iterate(pa_ml, 1, NULL);
    }
}

/**
 * Gets the Pulse Recording devices from the API
 * @param pulseRecDevList
 * @return list_t * pulseRecDevList
 */
list_t * pulseGetRecDevicesList(list_t * pulseRecDevList)
{
	soundDevice_t soundDev;

    // This is where we'll store the input device list
	pulseDevice_t pa_input_devicelist[MAX_NUM_DEVICES];

   // This is where we'll store the output device list
	pulseDevice_t pa_output_devicelist[MAX_NUM_DEVICES];

	pulseDevice_t pulseDevice;

   if (pa_get_devicelist(pa_input_devicelist, pa_output_devicelist) < 0)
   {
		TRACE("%s", "failed to get device list");
		return (list_t *)NULL;
   }

   for (int ctr = 0; ctr < MAX_NUM_DEVICES; ctr++)
   {
		if (!pa_input_devicelist[ctr].initialized) break;
		pulseInitDev(&pulseDevice);
		strncpy(soundDev.name, pa_input_devicelist[ctr].name, LONG_LEN);
		//strncpy(soundDev.description, pa_input_devicelist[ctr].description, LONG_LEN);
		strncpy(soundDev.description, pa_input_devicelist[ctr].description, DEV_COMBO_TEXT_LINE_LEN);
		soundDev.description[DEV_COMBO_TEXT_LINE_LEN-1]=0;
		pulseRecDevList = listAppend(pulseRecDevList, &soundDev, sizeof(soundDevice_t));
   }
   return pulseRecDevList;
}



