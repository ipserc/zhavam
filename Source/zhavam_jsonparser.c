/*
 * zhavam_jsonparser.c
 *
 *  Created on: 20 ene. 2018
 *      Author: ipserc
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strrep.h"
#include "acrcloud_recognizer.h"
#include "zhavam_jsonparser.h"
#include "zhavam_errtra.h"

/**
 * Initializes the fields of the acr_data_t to an empty strings
 * @param acrData: pointer to acr_data_t variable
 */
void initAcrDataT(acr_data_t * acrData)
{
	acrData->status.code[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->status.msg[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->status.version[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.acrid[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.album[0] = '\0'; // @suppress("Field cannot be resolved")
	for (int i = 0; i < MAX_ITEMS; ++i)
		acrData->metadata.music.artists[0][i] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.duration_ms[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_ids[0] = '\0'; // @suppress("Field cannot be resolved")
	for (int i = 0; i < MAX_ITEMS; ++i)
		acrData->metadata.music.genres[0][i] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.label[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.play_offset_ms[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.release_date[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.result_from[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.score[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.title[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_metadata.youtube_vid[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_metadata.spotify.album_id[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_metadata.spotify.artist_id[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_metadata.spotify.track_id[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_metadata.deezer.album_id[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_metadata.deezer.artist_id[0] = '\0'; // @suppress("Field cannot be resolved")
	acrData->metadata.music.external_metadata.deezer.track_id[0] = '\0'; // @suppress("Field cannot be resolved")

}

/**
* Replaces the first occurrence of f(rom)car to t(o)car
* It's a trick way to change the metadata.music[0] index.
* @param string: String to make the substitution
* @param fcar: character to find
* @param tcar: Character to put in place
* @return A pointer to the string
*/
char * substMusicIndex(char * string, char fcar, char tcar)
{
	char * ptr = string;
	while (*ptr)
	{
		if (*ptr++ == fcar)
		{
			*ptr = tcar;
			break;
		}
	}
	return string;
}

/**
 * Gets the value from the jsmn structure and copies it into the destination. Returns an empty string if not found.
 * @param tpath: Path to the wanted value
 * @param musicIndex: New index for metadata.music[0]
 * @param acrResponseField: Destination variable in which the found value is copied.
 * @param jsonMsg: JSON message in which the value is sought
 * @param jsmnTokenArray: JSMN Tokens array
 * @return A pointer to the destination variable in which the found value is copied.
 */
char * setUpAcrResponseField(char * tpath, int musicIndex, char * acrResponseField, char * jsonMsg, jsmntok_t * jsmnTokenArray)
{
	char * btpath;
	char * tokenValue;

	btpath = malloc(sizeof(char)*(strlen(tpath)+1));
	sprintf(btpath, "%s", tpath);
	if (musicIndex != 0) btpath = substMusicIndex(btpath, '0', '0' + musicIndex);

	tokenValue = getTokenValue(btpath, jsonMsg, jsmnTokenArray);
	sprintf(acrResponseField, "%s", tokenValue ? tokenValue : "");
	free(tokenValue);
	free(btpath);
	return acrResponseField;
}

/**
 * Runs over the JSON Message to find the best scored music item
 * @param jsonMsg: JSON message in which the value is sought
 * @param jsmnTokenArray: JSMN Tokens array
 * @return The index of the best music score
 */
int getIndexBestMusicScore(char * jsonMsg, jsmntok_t * jsmnTokenArray)
{
	int bestMusicScore = 0;
	int musicScore = 0;
	int bestIndex = 0;
	int index = 0;
	char * tokenValue;
	char tpath[25];

	sprintf(tpath, "%s%d%s", "metadata.music[", index, "].score");
	while ((tokenValue = getTokenValue(tpath, jsonMsg, jsmnTokenArray)) != NULL)
	{
		musicScore = atoi(tokenValue);
		free(tokenValue);
		if (bestMusicScore < musicScore)
		{
			bestMusicScore = musicScore;
			bestIndex = index;
		}
		if (++index > MAX_ITEMS) break;
		sprintf(tpath, "%s%d%s", "metadata.music[", index, "].score");
	}
	return bestIndex;
}

/**
 * Gets the specific fields from the JSON Message and loads them into the acr_data_t variable
 * @param jsonMsg: JSON message returned from the music content recognition service
 * @param acrResponse: The structure in which the information is stored
 * @return 0 if success or any other value if fails.
 */
int getAcrData(char * jsonMsg, acr_data_t * acrResponse)
{
	jsmn_parser parser;
	jsmntok_t * jsmnTokenArray;

	/* First remove some escaped chars */
	jsonMsg = strrep(jsonMsg, "\/", "/");
	jsonMsg = strrep(jsonMsg, "\\/", "/");
	jsonMsg = strrep(jsonMsg, "&amp;", "&");

	//jsonMsg = strrep(jsonMsg, "\\", "");

	int tokenCount = parseJSON(jsonMsg, &parser, &jsmnTokenArray);
	if (tokenCount < 0) return EXIT_FAILURE;

	jsmntok_t * jsonToken = jsmnTokenArray;
	if (getJsmnTokenLen(jsonToken) < 0) return JSMN_ERROR_PART;
	if (!memcmp("", &jsonMsg[jsonToken->start], (size_t)getJsmnTokenLen(jsonToken))) return JSMN_ERROR_PART;

	// ain't necessary
	//initAcrDataT(acrResponse);

	setUpAcrResponseField("status.code", 0, acrResponse->status.code, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("status.msg", 0, acrResponse->status.msg, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("status.version", 0, acrResponse->status.version, jsonMsg, jsmnTokenArray);

	if (acrResponse->status.code[0] != '0') return EXIT_FAILURE;

	int musicIndex = getIndexBestMusicScore(jsonMsg, jsmnTokenArray);

	setUpAcrResponseField("metadata.music[0].album.name", musicIndex, acrResponse->metadata.music.album, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].artists[0].name", musicIndex, acrResponse->metadata.music.artists[0], jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].artists[1].name", musicIndex, acrResponse->metadata.music.artists[1], jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].artists[2].name", musicIndex, acrResponse->metadata.music.artists[2], jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].genres[0].name", musicIndex, acrResponse->metadata.music.genres[0], jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].genres[1].name", musicIndex, acrResponse->metadata.music.genres[1], jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].genres[2].name", musicIndex, acrResponse->metadata.music.genres[2], jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].label", musicIndex, acrResponse->metadata.music.label, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].release_date", musicIndex, acrResponse->metadata.music.release_date, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].title", musicIndex, acrResponse->metadata.music.title, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].external_metadata.spotify.album.id", musicIndex, acrResponse->metadata.music.external_metadata.spotify.album_id, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].external_metadata.spotify.track.id", musicIndex, acrResponse->metadata.music.external_metadata.spotify.track_id, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].external_metadata.youtube.vid", musicIndex, acrResponse->metadata.music.external_metadata.youtube_vid, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].external_metadata.deezer.album.id", musicIndex, acrResponse->metadata.music.external_metadata.deezer.album_id, jsonMsg, jsmnTokenArray);
	setUpAcrResponseField("metadata.music[0].external_metadata.deezer.track.id", musicIndex, acrResponse->metadata.music.external_metadata.deezer.track_id, jsonMsg, jsmnTokenArray);

	return EXIT_SUCCESS;
}
