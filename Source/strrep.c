/*
 * strrep.c
 *
 *  Created on: 12 ene. 2020
 *      Author: ipserc
 */

#include "strrep.h"

/** appendstr (Append String). Appends the string 'append' to the string 'string' in a new allocated string.
 * You have to freed the new string when you don't need it anymore.
 * @param string The string in which append the new text.
 * @param append The new string to append.
 * @return returns the new string with the appended text.
 */
char * appendstr(const char * string, const char * append) {
	char * newString = malloc(strlen(string) + strlen(append) + 1);

	sprintf(newString, "%s%s", string, append);
	return newString;
}

/**
 * strtokk (String Tokenizer). Breaks string 'string' into a series of tokens using 'strf' as a WHOLE DELIMITER string.
 * It works in the same way as strtok, so the 'string' passed as parameter is split in the different parts, breaking the 'string'.
 * @param string The contents of this string are modified and broken into smaller strings (tokens).
 * @param strf This is the C string containing the DELIMITER.
 * @return Returns a pointer to the first token found in the string. A null pointer is returned if there are no tokens left to retrieve.
 */
char * strtokk(char * string, const char * strf) {
	static char * ptr;
	static char * ptr2;

	if (!*strf) return string;
	if (string) ptr = string;
	else {
		if (!ptr2) return ptr2;
		ptr = ptr2 + strlen(strf);
	}

	if (ptr) {
		ptr2 = strstr(ptr, strf);
		if (ptr2) memset(ptr2, 0, strlen(strf));
	}
	return ptr;
}

/**
 * String Replace. Replaces 'strf' with 'strr' in 'cadena' and returns the new string
 * You need to free the returned string in your code after using strrep
 * @param cadena the string with the text
 * @param strf the text to find
 * @param strr the replacement text
 * @return the text updated wit the replacement
 */
char * strrep(const char * cadena, const char * strf, const char * strr) {
	char * string;
	char * ptr;
	char * strrep;

	string = (char *)malloc(strlen(cadena) + 1);
	sprintf(string, "%s", cadena);
	ptr = strtokk(string, strf);
	strrep = malloc(strlen(ptr) + 1);
	memset(strrep, 0, strlen(ptr));
	while (ptr) {
		strrep = appendstr(strrep, ptr);
		ptr = strtokk(NULL, strf);
		if (ptr) strrep = appendstr(strrep, strr);
	}
	free(string);
	return strrep;
}

