/*
 * zhavam_jsonparser.h
 *
 *  Created on: 21 ene. 2018
 *      Author: ipserc
 */

#ifndef SOURCE_ZHAVAM_JSONPARSER_H_
#define SOURCE_ZHAVAM_JSONPARSER_H_

#define JSMN_PARENT_LINKS

#include <stddef.h>
#include "jsmnRipper.h"

#include "zhavam_acrcloud.h"

//#define MAX_NUM_OF_TOKENS 1000 // a number >= total number of tokens
#define VALUE_MAX_LEN 200 // Maximum length of the value field
#define MAX_ITEMS 3	// Maximum number of item in a list

/*
typedef struct
{
	char * name;
	jsmntype_t jtype;
	int index; //If array the index of the item
} item_t;
*/

/**
 * status definition section
 */
typedef struct
{
	char msg[VALUE_MAX_LEN];
	char code[VALUE_MAX_LEN];
	char version[VALUE_MAX_LEN];
} status_t;

//"metadata.music.external_metadata.spotify.album.id"
typedef struct
{
	char album_id[VALUE_MAX_LEN];
	char artist_id[VALUE_MAX_LEN];
	char track_id[VALUE_MAX_LEN];
} spotify_t;

typedef struct
{
	spotify_t spotify;
	char youtube_vid[VALUE_MAX_LEN];
} external_metadata_t;

/**
 * music structure
 */
typedef struct
{
	char external_ids[VALUE_MAX_LEN];
	char play_offset_ms[VALUE_MAX_LEN];
	external_metadata_t external_metadata;
	char artists[MAX_ITEMS][VALUE_MAX_LEN];
	char genres[MAX_ITEMS][VALUE_MAX_LEN];
	char title[VALUE_MAX_LEN];
	char release_date[VALUE_MAX_LEN];
	char label[VALUE_MAX_LEN];
	char duration_ms[VALUE_MAX_LEN];
	char album[VALUE_MAX_LEN];
	char acrid[VALUE_MAX_LEN];
	char result_from[VALUE_MAX_LEN];
	char score[VALUE_MAX_LEN];
} music_t;

/**
 * metadata structure
 */
typedef struct
{
	music_t music;
	char timestamp_utc[VALUE_MAX_LEN];
} metadata_t;

/**
 * data structure
 */
typedef struct
{
	status_t status;
	metadata_t metadata;
	char cost_time[VALUE_MAX_LEN];
	char result_type[VALUE_MAX_LEN];
} acr_data_t;

/*
 * Prototypes
 */
/* zhavam_jsonparser.c */
void initAcrDataT(acr_data_t * acrResponse);
char *substMusicIndex(char *string, char fcar, char tcar);
char *setUpAcrResponseField(char *tpath, int musicIndex, char *acrResponseField, char *jsonMsg, jsmntok_t *jsmnTokenArray);
int getIndexBestMusicScore(char *jsonMsg, jsmntok_t *jsonTokenArray);
int getAcrData(char *jsonMsg, acr_data_t *acrResponse);

#endif /* SOURCE_ZHAVAM_JSONPARSER_H_ */
