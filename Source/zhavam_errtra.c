/*
 * zhavam_errtra.c
 *
 *  Created on: 6 ene. 2018
 *      Author: ipserc
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "zhavam_errtra.h"

/**
 * Full Error message with information of errno, fileName, functionName, lineNbr and ErrorMsg printed in stderr
 * @param fileName
 * @param functionName
 * @param lineNbr
 * @param errorNbr
 * @param fmtstr: Format string as in printf
 * @param ...: Rest of parameters
 */
void funcError(const char * fileName,
				const char * functionName,
				const int lineNbr,
				const int errorNbr,
				const char * fmtstr,...)
{
	char fullErrorMsg[ERROR_MSGLEN];
	char ErrorMsg[ERROR_MSGLEN];
	va_list arglist;

	//strerror(errno) ain't necessary since perror shows the strerror
	errno = errorNbr;
	va_start(arglist, fmtstr);
	vsprintf(ErrorMsg, fmtstr, arglist);
	va_end(arglist);
	sprintf(fullErrorMsg, "ERROR:%d - %s %s [%d] - %s", errno, fileName, functionName, lineNbr, ErrorMsg);
	perror(fullErrorMsg);
	//gtkSetCursor(NORMAL_CURSOR);
	exit(EXIT_FAILURE);
}

/**
 * Short Error message with information of errno and ErrorMsg printed in stderr
 * @param fmtstr: Format string as in printf
 * @param ...: Rest of parameters
 */
void eError(const char * fmtstr,...)
{
	char fullErrorMsg[ERROR_MSGLEN];
	char ErrorMsg[ERROR_MSGLEN];
	va_list arglist;

	//strerror(errno) ain't necessary since perror shows the strerror
	va_start(arglist, fmtstr);
	vsprintf(ErrorMsg, fmtstr, arglist);
	va_end(arglist);
	sprintf(fullErrorMsg, "ERROR:%d - %s", errno, ErrorMsg);
	perror(fullErrorMsg);
	//gtkSetCursor(NORMAL_CURSOR);
	exit(EXIT_FAILURE);
}

/**
 * Full trace message with information of timestamp, fileName, functionName, lineNbr and TraceMsg printed in stdout
 * @param const char * fileName
 * @param const char * functionName
 * @param fmtstr: Format string as in printf
 * @param ...: Rest of parameters
 */
void funcTrace(const char * fileName,
				const char * functionName,
				const int lineNbr,
				const char * fmtstr,...)
{
	struct timespec currtime;
	struct tm * tmtime;
	char strtime[STR_TIMELEN];
	va_list arglist;

	clock_gettime(CLOCK_REALTIME, &currtime);
	tmtime = localtime(&currtime.tv_sec);
	sprintf(strtime, "%d/%02d/%02d %02d:%02d:%02d.%09lu", tmtime->tm_year+1900, tmtime->tm_mon+1, tmtime->tm_mday, tmtime->tm_hour, tmtime->tm_min, tmtime->tm_sec, currtime.tv_nsec);
	printf("%s:%s %s [%d] :", strtime, fileName, functionName, lineNbr);
	va_start(arglist, fmtstr);
	vprintf(fmtstr, arglist);
	va_end(arglist);
	puts("");
}

/**
 * Full warning message with information of errno, fileName, functionName, lineNbr and WarningMsg printed in stdout
 * @param const char * fileName
 * @param const char * functionName
 * @param const int lineNbr
 * @param const int errorNbr
 * @param fmtstr: Format string as in printf
 * @param ...: Rest of parameters
 */
int funcWarning(const char * fileName,
				const char * functionName,
				const int lineNbr,
				const int errorNbr,
				const char * fmtstr,...)
{
	char fullWarningMsg[WARNING_MSGLEN];
	char WarningMsg[WARNING_MSGLEN];
	va_list arglist;

	//strerror(errno) ain't necessary since perror shows the strerror
	errno = errorNbr;
	va_start(arglist, fmtstr);
	vsprintf(WarningMsg, fmtstr, arglist);
	va_end(arglist);
	sprintf(fullWarningMsg, "WARNING:%d - %s %s [%d] - %s", errno, fileName, functionName, lineNbr, WarningMsg);
	perror(fullWarningMsg);
	//gtkSetCursor(NORMAL_CURSOR);
	return EXIT_FAILURE;
}

/**
 * Short warning message with information of errno and WarningMsg printed in stdout
 * @param fmtstr: Format string as in printf
 * @param ...: Rest of parameters
 */
void wWarning(const char * fmtstr,...)
{
	char fullWarningMsg[WARNING_MSGLEN];
	char WarningMsg[WARNING_MSGLEN];
	va_list arglist;

	va_start(arglist, fmtstr);
	vsprintf(WarningMsg, fmtstr, arglist);
	va_end(arglist);
	sprintf(fullWarningMsg, "WARNING:%d - %s", errno, WarningMsg);
	perror(fullWarningMsg);
}

/**
 * Stat Lib Errro Manager. Writes specific error messages for EACCES, ELOOP, ENAMETOOLONG, ENOENT and EOVERFLOW
 * @param  statError
 * @return statError
 */
int statErrorMngr(int statError)
{
	switch (statError) {
		case EACCES:
			eError("%s", ERROR06); break;
		case EIO:
			eError("%s", ERROR07); break;
		case ELOOP:
			eError("%s", ERROR08); break;
		case ENAMETOOLONG:
			eError("%s", ERROR09); break;
		case ENOENT:
			wWarning("%s", WARNING07); break;
		case ENOTDIR:
			eError("%s", ERROR10); break;
		case EOVERFLOW:
			eError("%s", ERROR11); break;
	}
	return statError;
}

/**
 * mkdir Errro Manager. Writes specific error messages for EACCES, EEXIST, EMLINK, ENOSPC and EROFS
 * @param mkdirError
 * @return mkdirError
 */
int mkdirErrorMngr(int mkdirError)
{
	switch (mkdirError) {
		case EACCES:
			wWarning("%s", WARNING08); break;
		case EEXIST:
			wWarning("%s", WARNING09); break;
		case EMLINK:
			eError("%s", ERROR12); break;
		case ENOSPC:
			eError("%s", ERROR13); break;
		case EROFS:
			wWarning("%s", WARNING10); break;
	}
	return mkdirError;
}
