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

#include "acrcloud_recognizer.h"
#include "jsmn.h"
#include "list.h"

#include "zhavam.h"
#include "zhavam_acrcloud.h"
#include "zhavam_alsa.h"
#include "zhavam_config.h"
#include "zhavam_devices.h"
#include "zhavam_errtra.h"
#include "zhavam_jsonparser.h"

/**
 * Macro substitutions for JSONParser token managing
 */
#define GET_TOKEN_LEN(token) token->end - token->start
#define NEXT_TOKEN {++token; ++tokNbr;}
#define PREV_TOKEN {--token; --tokNbr;}

/**
 * ACRCloud JSON reply message tokens
 */
char * tokenMetaWords[] = {\
		"status", "metadata", "cost_time", "result_type", \
		"msg", "code", "version", \
		"music", "timestamp_utc", \
		"external_ids", "play_offset_ms", "external_metadata", "artists", "genres", "title", "release_date", \
				"label", "duration_ms", "album", "acrid", "result_from", "score" \
};

/**
 * ACR Data Response Structure Initialization function
 * @param acr_data_t * acrResponse
 */
void initACRdataStruct(acr_data_t * acrResponse)
{
	for(int m = 0; m < MAX_ITEMS; ++m)
	{
		memset(acrResponse->metadata.music[m].acrid, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].album, 0, VALUE_MAX_LEN);
		for(int i = 0; i < MAX_ITEMS; ++i) {
			memset(acrResponse->metadata.music[m].artists[i], 0, VALUE_MAX_LEN);
			memset(acrResponse->metadata.music[m].genres[i], 0, VALUE_MAX_LEN);
		}
		memset(acrResponse->metadata.music[m].duration_ms, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].external_ids, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].external_metadata, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].label, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].play_offset_ms, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].release_date, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].result_from, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].score, 0, VALUE_MAX_LEN);
		memset(acrResponse->metadata.music[m].title, 0, VALUE_MAX_LEN);
	}
	memset(acrResponse->metadata.timestamp_utc, 0, VALUE_MAX_LEN);
	memset(acrResponse->status.code, 0, VALUE_MAX_LEN);
	memset(acrResponse->status.msg, 0, VALUE_MAX_LEN);
	memset(acrResponse->status.version, 0, VALUE_MAX_LEN);
	memset(acrResponse->cost_time, 0, VALUE_MAX_LEN);
	memset(acrResponse->result_type, 0, VALUE_MAX_LEN);
}

/**
 * To be used instead of GET_TOKEN_LEN(token)
 * Substitute GET_TOKEN_LEN(token) with getTokenLen(token) along this file
int getTokenLen(jsmntok_t * token)
{
	return token->end - token->start;
}
*/

/**
 * Identify Main Section of
 * @param int section
 * @param char * jsonToken
 * @param size_t tokenLen
 */
int identifyMainSection(int section, char * jsonToken, size_t tokenLen)
{
	for (int tokId = _STATUS_; tokId <= _RESULT_TYPE_; ++tokId)
		if (!memcmp(tokenMetaWords[tokId], jsonToken, (size_t)tokenLen)) return tokId;
	return section;
}

/**
 * Identify Status Tokens Section
 * @param char * jsonToken
 * @param size_t tokenLen
 * @return Status Token identified or -1 if fails
 */
int identifyStatusToken(char * jsonToken, size_t tokenLen)
{
	for (int tokId = _MSG_; tokId <= _VERSION_; ++tokId)
		if (!memcmp(tokenMetaWords[tokId], jsonToken, (size_t)tokenLen)) return tokId;
	return -1;
}

/**
 * Identify Music Tokens Section
 * @param char * jsonToken
 * @param size_t tokenLen
 * @return Music Token identified or -1 if fails
 */
int identifyMusicToken(char * jsonToken, size_t tokenLen)
{
	for (int tokId = _EXTERNAL_IDS_; tokId <= _SCORE_; ++tokId)
		if (!memcmp(tokenMetaWords[tokId], jsonToken, (size_t)tokenLen)) return tokId;
	return -1;
}

/**
 * Identify Metadata Tokens Section
 * @param char * jsonToken
 * @param size_t tokenLen
 * @return Metadata Token identified or -1 if fails
 */
int identifyMetadataToken(char * jsonToken, size_t tokenLen)
{
	for (int tokId = _MUSIC_; tokId <= _TIMESTAMP_UTC_; ++tokId)
		if (!memcmp(tokenMetaWords[tokId], jsonToken, (size_t)tokenLen)) return tokId;
	return -1;
}

/**
 * Prints Token content. For debugging purposes
 * @param char * jsonTokenPtr
 * @param size_t tokenLen
 */
void printToken(char * jsonTokenPtr, size_t tokenLen)
{
	char token[1000];
	strncpy(token, jsonTokenPtr, tokenLen);
	token[tokenLen] = '\0';
	printf("------------TOKEN:%s\n", token);
}

/**
 * Copies the token pointed by jsonTokenPtr to token
 * @param char * token
 * @param char * jsonTokenPtr
 * @param size_t tokenLen
 * @return char * token
 */
char * copyToken(char * token, char * jsonTokenPtr, size_t tokenLen)
{
	strncpy(token, jsonTokenPtr, tokenLen);
	token[tokenLen] = '\0';
	return token;
}

/**
 * Counts the number of times that a specific Field appears in the string
 * @param const char * string2find
 * @param const char * myString
 * @return int the number of fields found
 */
int countField(const char * string2find, const char * myString)
{
	int count = 0;
	const char *tmp = myString;
	while((tmp = strstr(tmp, string2find)))
	{
	   ++count;
	   ++tmp;
	}
	return count;
}

/**
 * Extracts a Status Token field: status.msg, status.code and status.version
 * @param acr_data_t * acrResponse
 * @param char * jsonMsg
 * @param jsmntok_t * token
 * @param int * tokNbr
 * @param int section
 * @return jsmntok_t * token
 */
jsmntok_t * getStatusTokens(acr_data_t * acrResponse, char * jsonMsg, jsmntok_t * token, int * tokNbr, int section)
{
	switch (identifyStatusToken(&jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token)))
	{
	case _MSG_:
		NEXT_TOKEN;
		copyToken(acrResponse->status.msg, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _CODE_:
		NEXT_TOKEN;
		copyToken(acrResponse->status.code, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _VERSION_:
		NEXT_TOKEN;
		copyToken(acrResponse->status.version, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	}
	return token;
}

/**
 * Extracts a Status Token field: music[m].external_ids, music[m].play_offset_ms, music[m].external_metadata,
 * music[m].artists[item], music[m].genres[item], music[m].title, music[m].release_date, music[m].label,
 * music[m].duration_ms, music[m].album, music[m].acrid, music[m].result_from and music[m].score
 * @param acr_data_t * acrResponse
 * @param char * jsonMsg
 * @param jsmntok_t * token
 * @param int * tokNbr
 * @param int section
 * @return jsmntok_t * token
 */
jsmntok_t * getMusicToken(acr_data_t * acrResponse, char * jsonMsg, jsmntok_t * token, int * tokNbr, int section, int m)
{
	int item = 0;

	switch (identifyMusicToken(&jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token)))
	{
	case _EXTERNAL_IDS_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].external_ids, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _PLAY_OFFSET_MS_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].play_offset_ms, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _EXTERNAL_METADATA_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].external_metadata, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _ARTISTS_:
		NEXT_TOKEN;
		NEXT_TOKEN;
		NEXT_TOKEN;
		item = 0;
		while (!memcmp("name", &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token))) {
			NEXT_TOKEN;
			copyToken(acrResponse->metadata.music[m].artists[item], &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
			++item;
			NEXT_TOKEN;
			NEXT_TOKEN;
		}
		PREV_TOKEN;
		PREV_TOKEN;
		break;
	case _GENRES_:
		NEXT_TOKEN;
		NEXT_TOKEN;
		NEXT_TOKEN;
		item = 0;
		while (!memcmp("name", &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token))) {
			NEXT_TOKEN;
			copyToken(acrResponse->metadata.music[m].genres[item], &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
			++item;
			NEXT_TOKEN;
			NEXT_TOKEN;
		}
		PREV_TOKEN;
		PREV_TOKEN;
		break;
	case _TITLE_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].title, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _REALEASE_DATE_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].release_date, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _LABEL_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].label, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _DURATION_MS_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].duration_ms, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _ALBUM_:
		NEXT_TOKEN;
		NEXT_TOKEN;
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].album, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _ACRID_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].acrid, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _RESULT_FROM_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].result_from, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _SCORE_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.music[m].score, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	}
	NEXT_TOKEN;
	return token;
}

/**
 * Walks through the Music Token JSON Structure
 * @param acr_data_t * acrResponse
 * @param char * jsonMsg
 * @param jsmntok_t * token
 * @param int * tokNbr
 * @param int section
 * @return jsmntok_t * token
 */
jsmntok_t * walkMusicToken(acr_data_t * acrResponse, char * jsonMsg, jsmntok_t * token, int * tokNbr, int section)
{
	int numOfItems = countField(tokenMetaWords[_ACRID_], &jsonMsg[token->start]);
	numOfItems = numOfItems > MAX_ITEMS ? MAX_ITEMS : numOfItems;

	NEXT_TOKEN;
	NEXT_TOKEN;
	for(int m = 0; m < numOfItems; ++m)
	{
		while (identifyMusicToken(&jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token)) != -1)
		{
			token = getMusicToken(acrResponse, jsonMsg, token, tokNbr, section, m);
		}
		NEXT_TOKEN;
	}
	PREV_TOKEN;
	PREV_TOKEN;
	return token;
}

/**
 * Extracts Metadata Token field metadata.timestamp_utc and detects music section
 * @param acr_data_t * acrResponse
 * @param char * jsonMsg
 * @param jsmntok_t * token
 * @param int * tokNbr
 * @param int section
 * @return jsmntok_t * token
 */
jsmntok_t *  getMetadataTokens(acr_data_t * acrResponse, char * jsonMsg, jsmntok_t * token, int * tokNbr, int section)
{
	switch (identifyMetadataToken(&jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token)))
	{
	case _TIMESTAMP_UTC_:
		NEXT_TOKEN;
		copyToken(acrResponse->metadata.timestamp_utc, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		break;
	case _MUSIC_:
		NEXT_TOKEN;
		token = walkMusicToken(acrResponse, jsonMsg, token, tokNbr, section);
		break;
	}
	return token;
}

/**
 * Walks through the JSON Token Structure: STATUS, METADATA, COST_TIME and RESULT_TYPE
 * @param acr_data_t * acrResponse
 * @param char * jsonMsg
 * @param jsmntok_t * token
 * @return EXIT_SUCCESS
 */
int walkTokens(acr_data_t * acrResponse, char * jsonMsg, int tokenCount, jsmntok_t * tokens)
{
	int section = _START_SECTION_;
	int tokNbr = 0;

	jsmntok_t * token = tokens;
	initACRdataStruct(acrResponse);
	while (tokNbr < tokenCount)
    {
		if (GET_TOKEN_LEN(token) < 0) return JSMN_ERROR_PART;
		if (!memcmp("", &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token))) return JSMN_ERROR_PART;
		section = identifyMainSection(section, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
		switch (section)
		{
		case _STATUS_:
			//printToken(&jsonMsg[token->start], (size_t)tokenLen);
			token = getStatusTokens(acrResponse, jsonMsg, token, &tokNbr, section);
			break;
		case _METADATA_:
			token = getMetadataTokens(acrResponse, jsonMsg, token, &tokNbr, section);
			break;
		case _COST_TIME_:
			NEXT_TOKEN;
			strncpy(acrResponse->cost_time, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
			break;
		case _RESULT_TYPE_:
			NEXT_TOKEN;
			strncpy(acrResponse->result_type, &jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token));
			break;
		case _START_SECTION_:
			break;
		default:
			sprintf(STATUS_MESSAGE, "*** BAD SECTION:%s - token->type:%d - Value:%s", tokenMetaWords[section], token->type, strndup(&jsonMsg[token->start], (size_t)GET_TOKEN_LEN(token)));
		}
		NEXT_TOKEN;
    }
	return EXIT_SUCCESS;
}

/**
 * JSMN (Jasmine) Parser call
 * @param acr_data_t * acrResponse
 * @param char * jsonMsg
 * return Number of tokens found if fails, EXIT_SUCCESS on success
 */
int parseJSON(acr_data_t * acrResponse, char * jsonMsg)
{
	jsmn_parser parser;
	jsmntok_t tokens[MAX_NUM_OF_TOKENS];

	memset(&tokens, 0, sizeof(tokens));
	jsmn_init(&parser);
	int tokenCount = jsmn_parse(&parser, jsonMsg, strlen(jsonMsg), tokens, MAX_NUM_OF_TOKENS);
	if (tokenCount < 0) {
		  sprintf(STATUS_MESSAGE, "unable to read tokens from json message");
		  ERROR("%s", STATUS_MESSAGE);
		  gtkWarning("%s", STATUS_MESSAGE);
		  return tokenCount;
	}
	return walkTokens(acrResponse, jsonMsg, tokenCount, tokens);;
}

/**
 * Prints the ACRcloud data. For debugging puposes
 * @param acr_data_t * acrResponse
 */
void printAcrData(acr_data_t * acrResponse)
{
	printf("status.msg:%s\n", acrResponse->status.msg);
	printf("status.code:%s\n", acrResponse->status.code);
	printf("status.version:%s\n", acrResponse->status.version);

	if (acrResponse->status.code[0] != '0') return;

	for (int m; m < MAX_ITEMS; ++m)
	{
		if (!acrResponse->metadata.music[m].title[0]) return;
		printf("metadata.music[%d].external_ids:%s\n", m, acrResponse->metadata.music[m].external_ids);
		printf("metadata.music[%d].play_offset_ms:%s\n", m, acrResponse->metadata.music[m].play_offset_ms);
		for(int i = 0; i < MAX_ITEMS; ++i) {
			if (!acrResponse->metadata.music[m].genres[i][0]) break;
			printf("metadata.music[%d].genres[%d]:%s\n", m, i, acrResponse->metadata.music[m].genres[i]);
		}
		printf("metadata.music[%d].external_metadata:%s\n", m, acrResponse->metadata.music[m].external_metadata);
		for(int i = 0; i < MAX_ITEMS; ++i) {
			if (!acrResponse->metadata.music[m].artists[i][0]) break;
			printf("metadata.music[%d].artists[%d]:%s\n", m, i, acrResponse->metadata.music[m].artists[i]);
		}
		printf("metadata.music[%d].title:%s\n", m, acrResponse->metadata.music[m].title);
		printf("metadata.music[%d].release_date:%s\n", m, acrResponse->metadata.music[m].release_date);
		printf("metadata.music[%d].label:%s\n", m, acrResponse->metadata.music[m].label);
		printf("metadata.music[%d].duration_ms:%s\n", m, acrResponse->metadata.music[m].duration_ms);
		printf("metadata.music[%d].album:%s\n", m, acrResponse->metadata.music[m].album);
		printf("metadata.music[%d].acrid:%s\n", m, acrResponse->metadata.music[m].acrid);
		printf("metadata.music[%d].result_from:%s\n", m, acrResponse->metadata.music[m].result_from);
		printf("metadata.music[%d].score:%s\n", m, acrResponse->metadata.music[m].score);
	}
	printf("metadata.timestamp_utc:%s\n", acrResponse->metadata.timestamp_utc);
	printf("result_type:%s\n", acrResponse->result_type);
}
