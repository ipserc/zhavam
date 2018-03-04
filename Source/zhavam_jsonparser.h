/*
 * zhavam_jsonparser.h
 *
 *  Created on: 21 ene. 2018
 *      Author: ipserc
 */

#ifndef SOURCE_ZHAVAM_JSONPARSER_H_
#define SOURCE_ZHAVAM_JSONPARSER_H_

#include <stddef.h>

#include "jsmn.h"
#include "zhavam_acrcloud.h"

#define MAX_NUM_OF_TOKENS 1000 // a number >= total number of tokens

/**
 * JSON Reply ACR Data tokens
 */
enum
{
	_START_SECTION_ = -99,
	//main_section definition section
	_STATUS_ = 0,
	_METADATA_,
	_COST_TIME_,
	_RESULT_TYPE_,
	//status definition section
	_MSG_,
	_CODE_,
	_VERSION_,
	//metadata definition section
	_MUSIC_,
	_TIMESTAMP_UTC_,
	//music definition section
	_EXTERNAL_IDS_,
	_PLAY_OFFSET_MS_,
	_EXTERNAL_METADATA_,
	_ARTISTS_,
	_GENRES_,
	_TITLE_,
	_REALEASE_DATE_,
	_LABEL_,
	_DURATION_MS_,
	_ALBUM_,
	_ACRID_,
	_RESULT_FROM_,
	_SCORE_
};

/*
 * Prototypes
 */
void initACRdataStruct(acr_data_t *acrResponse);
int indentifyMainSection(int section, char *jsonToken, size_t tokenLen);
int indentifyStatusToken(char *jsonToken, size_t tokenLen);
int indentifyMusicToken(char *jsonToken, size_t tokenLen);
int indentifyMetadataToken(char *jsonToken, size_t tokenLen);
void printToken(char *jsonTokenPtr, size_t tokenLen);
char *copyToken(char *token, char *jsonTokenPtr, size_t tokenLen);
int countField(const char *string2find, const char *myString);
jsmntok_t *getStatusTokens(acr_data_t *acrResponse, char *jsonMsg, jsmntok_t *token, int *tokNbr, int section);
jsmntok_t *getMusicToken(acr_data_t *acrResponse, char *jsonMsg, jsmntok_t *token, int *tokNbr, int section, int m);
jsmntok_t *walkMusicToken(acr_data_t *acrResponse, char *jsonMsg, jsmntok_t *token, int *tokNbr, int section);
jsmntok_t *getMetadataTokens(acr_data_t *acrResponse, char *jsonMsg, jsmntok_t *token, int *tokNbr, int section);
int walkTokens(acr_data_t *acrResponse, char *jsonMsg, int tokenCount, jsmntok_t *tokens);
int parseJSON(acr_data_t *acrResponse, char *jsonMsg);
void printAcrData(acr_data_t *acrResponse);

#endif /* SOURCE_ZHAVAM_JSONPARSER_H_ */
