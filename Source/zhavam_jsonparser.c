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

#include "zhavam_errtra.h"
#include "zhavam_jsonparser.h"

#define JSMN_PARENT_LINKS

/**
 * Macro substitutions for JSONParser token managing
 */
#define GET_TOKEN_LEN(token) token->end - token->start

int listToken(list_t * tokenList, char * tpath)
{
	token_t token;
	char * strIndex;
	char * tpathtok = strtok(tpath, ".");

	while (tpathtok)
	{
		token.index = 0;
		token.jtype = JSMN_OBJECT;
		strIndex = strchr(tpathtok, '[');
		if (strIndex)
		{
			*strIndex++ = '\0';
			token.jtype = JSMN_ARRAY;
			while (*strIndex != ']')
			{
				token.index *= 10;
				token.index += *strIndex++ -'0';
			}
		}

		token.name = malloc(strlen(tpathtok));
		strcpy(token.name, tpathtok);
		tokenList = listAppend(tokenList, &token, sizeof(token));
		tpathtok = strtok(NULL, ".");
	}
	return (int)listItemsCount(tokenList);
}

void printTok(void * param)
{
	token_t * token  = (token_t *)param;
	printf("token->name:%s\n", token->name);
	printf("token->jtype:%d\n", token->jtype);
	printf("token->index:%d\n", token->index);
}

void freeTokenList(list_t * tokenList)
{
	token_t * token;
	while (tokenList->tail->prev)
	{
		token = tokenList->tail->item;
		free(token->name);
		listDrop(tokenList);
	}
	free(tokenList);
}

int parseJSON(char * jsonMsg, jsmn_parser * parser, jsmntok_t * jsonTokens)
{
//	jsmntok_t jsonTokens[MAX_NUM_OF_TOKENS];
	memset(jsonTokens, 0, sizeof(jsmntok_t) * MAX_NUM_OF_TOKENS);
	jsmn_init(parser);
	int tokenCount = jsmn_parse(parser, jsonMsg, strlen(jsonMsg), jsonTokens, MAX_NUM_OF_TOKENS);
	if (tokenCount < 0) {
		  printf("unable to read tokens from json message");
		  return tokenCount;
	}
	return tokenCount;
}

void nextToken(jsmntok_t ** jsonToken, int * jsonTokNbr)
{
	++(*jsonToken);
	++(*jsonTokNbr);
}

void prevToken(jsmntok_t ** jsonToken, int * jsonTokNbr)
{
	--(*jsonToken);
	--(*jsonTokNbr);
}

jsmntok_t *  findJsonToken(char * jsonMsg, list_t * tokenList, jsmntok_t * jsonTokens, int * jsonTokNbr, int tokenCount)
{
	jsmntok_t * jsonToken = jsonTokens;
	node_t * tokenListNode = tokenList->head;
	token_t * token = (token_t*)((node_t*)(tokenListNode->item));
	char * jsonTokenItem;
	int indexCount = 0;
	int tokenIndex = 0;

	if (GET_TOKEN_LEN(jsonToken) < 0) return (jsmntok_t *) NULL;
	if (!memcmp("", &jsonMsg[jsonToken->start], (size_t)GET_TOKEN_LEN(jsonToken))) return (jsmntok_t *) NULL;
	nextToken(&jsonToken, jsonTokNbr);
	while (*jsonTokNbr < tokenCount)
	{
		jsonTokenItem = malloc((size_t)GET_TOKEN_LEN(jsonToken)+1);
		jsonTokenItem[(size_t)GET_TOKEN_LEN(jsonToken)] = '\0';
		strncpy(jsonTokenItem, &jsonMsg[jsonToken->start], (size_t)GET_TOKEN_LEN(jsonToken));
		//TRACE("Token to find token->name:%s", token->name);
		//TRACE("jsonTokenItem:%s", jsonTokenItem);
		if (!strcmp(token->name, jsonTokenItem))
		{
			if (token->jtype == JSMN_ARRAY) tokenIndex = token->index;
			if (tokenIndex == indexCount)
			{
				indexCount = 0;
				if (tokenListNode->next)
				{
					tokenListNode = tokenListNode->next;
					token = (token_t*)((node_t*)(tokenListNode->item));
					//TRACE("**** %s Token to find token->name:%s", (tokenListNode->next) ? "Next" : "Last", token->name);
				}
				else // item found
				{
					free(jsonTokenItem);
					return jsonToken;
				}
			}
			else ++indexCount;
		}
		free(jsonTokenItem);
		nextToken(&jsonToken, jsonTokNbr);
	}
	return (jsmntok_t *)NULL;
}

void printJsonTokens(char * jsonMsg, jsmntok_t * jsonTokens, int tokenCount)
{
	jsmntok_t * jsonToken = jsonTokens;
	int  jsonTokNbr;
	char tokenfmt[50];

	nextToken(&jsonToken, &jsonTokNbr);
	while (jsonTokNbr < tokenCount)
	{
		sprintf(tokenfmt, "%s%d%s", "Token:%.", (size_t)GET_TOKEN_LEN(jsonToken), "s start:%d end:%d size:%d type:%d parent:%d\n");
		printf(tokenfmt, &jsonMsg[jsonToken->start], jsonToken->start, jsonToken->end, jsonToken->size, jsonToken->type, jsonToken->parent);
		nextToken(&jsonToken, &jsonTokNbr);
	}
}

void printJsonToken(char * jsonMsg, jsmntok_t * jsonToken)
{
	char tokenfmt[50];

	sprintf(tokenfmt, "%s%d%s", "Token:%.", (size_t)GET_TOKEN_LEN(jsonToken), "s start:%d end:%d size:%d type:%d parent:%d\n");
	printf(tokenfmt, &jsonMsg[jsonToken->start], jsonToken->start, jsonToken->end, jsonToken->size, jsonToken->type, jsonToken->parent);
}

void printJsonTokenValue(char * jsonMsg, jsmntok_t * jsonToken)
{
	printJsonToken(jsonMsg, ++jsonToken);
}

char * getTokenValue(char * tpath, char * jsonMsg, jsmntok_t * jsonTokens, int tokenCount)
{
	list_t * tokenList;
	jsmntok_t * jsonTokenFound;
	int jsonTokNbr = 0;
	char * tokenValue = (char*)NULL;
	char * tokenPath = malloc(strlen(tpath));
	char tokenfmt[50];

	sprintf(tokenPath, "%s", tpath);
	tokenList = listNew(tokenList);
	//TRACE("tokenPath:%s:", tokenPath);
	if (!listToken(tokenList, tokenPath)) TRACE("%s", "Unable to generate token path list.");
	else
	{
		//listPrint(tokenList, (printTok));
		jsonTokenFound = findJsonToken(jsonMsg, tokenList, jsonTokens, &jsonTokNbr, tokenCount);
		if (jsonTokenFound)
		{
			++jsonTokenFound;
			//printJsonToken(jsonMsg, --jsonTokenFound);
			//printJsonToken(jsonMsg, ++jsonTokenFound);
			tokenValue = malloc((size_t)GET_TOKEN_LEN(jsonTokenFound));
			sprintf(tokenfmt, "%s%d%s", "%.", (size_t)GET_TOKEN_LEN(jsonTokenFound), "s");
			sprintf(tokenValue, tokenfmt, &jsonMsg[jsonTokenFound->start]);
			TRACE("%s:%s", tpath, tokenValue);
		}
		else
		{
			tokenValue = malloc(1);
			*tokenValue = '\0';
			TRACE("%s:Token not found", tpath);
		}
	}
	free(tokenPath);
	freeTokenList(tokenList);
	return tokenValue;
}

char * setUpAcrResponseField(char * tpath, char * acrResponseField, char * jsonMsg, jsmntok_t * jsonTokens, int tokenCount)
{
	char * tokenValue;
	tokenValue = getTokenValue(tpath, jsonMsg, jsonTokens, tokenCount);
	sprintf(acrResponseField, "%s", tokenValue);
	free(tokenValue);
	return acrResponseField;
}

int getAcrData(char * jsonMsg, acr_data_t * acrResponse)
{
	jsmn_parser parser;
	jsmntok_t jsonTokens[MAX_NUM_OF_TOKENS];

	int tokenCount = parseJSON(jsonMsg, &parser, jsonTokens);
	if (tokenCount < 0) return EXIT_FAILURE;

	jsmntok_t * jsonToken = jsonTokens;
	if (GET_TOKEN_LEN(jsonToken) < 0) return JSMN_ERROR_PART;
	if (!memcmp("", &jsonMsg[jsonToken->start], (size_t)GET_TOKEN_LEN(jsonToken))) return JSMN_ERROR_PART;

	setUpAcrResponseField("status.code", acrResponse->status.code, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("status.msg", acrResponse->status.msg, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("status.version", acrResponse->status.version, jsonMsg, jsonTokens, tokenCount);

	if (acrResponse->status.code[0] != '0') return EXIT_FAILURE;

	setUpAcrResponseField("metadata.music.album.name", acrResponse->metadata.music.album, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.artists.name[0]", acrResponse->metadata.music.artists[0], jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.genres.name[0]", acrResponse->metadata.music.genres[0], jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.label", acrResponse->metadata.music.label, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.release_date", acrResponse->metadata.music.release_date, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.title", acrResponse->metadata.music.title, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.external_metadata.spotify.album.id", acrResponse->metadata.music.external_metadata.spotify.album_id, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.external_metadata.spotify.track.id", acrResponse->metadata.music.external_metadata.spotify.track_id, jsonMsg, jsonTokens, tokenCount);
	setUpAcrResponseField("metadata.music.external_metadata.youtube.vid", acrResponse->metadata.music.external_metadata.youtube_vid, jsonMsg, jsonTokens, tokenCount);

	return EXIT_SUCCESS;
}
