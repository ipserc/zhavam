/*
 * zhavam_errtra.h
 *
 *  Created on: 6 ene. 2018
 *      Author: ipserc
 */

#ifndef SOURCE_ZHAVAM_ERRTRA_H_
#define SOURCE_ZHAVAM_ERRTRA_H_

#include <errno.h>

#define ERROR_MSGLEN 600
#define WARNING_MSGLEN 600
#define STR_TIMELEN 50
#define MAXLEN_STATUS_MESSSAGE 120

char STATUS_MESSAGE[MAXLEN_STATUS_MESSSAGE];
int errno;

/**
 * WARNING icon, message length aand messages
 */
#define WARNINGICON "50697_121100_warning.png"
#define WARNINGMSGLEN 600
#define WARNING01 "No capture devices found. Unable to record sound."
#define WARNING02 "Incorrect zhavam config file. Unable to parse it."
#define WARNING03 "No access key found. Please introduce one into zhavam config file."
#define WARNING04 "zhavam cofig file does not exist. creating a new one from scratch with default values. Please set the acrcloud access credentials."
#define WARNING05 "Unable to open %s"
#define WARNING06 "ACRClod account is not set. Please go to https://www.acrcloud.com and create an account to use Zhavam."
#define WARNING07 "A component of path does not name an existing file or path is an empty string."
#define WARNING08 "Write permission is denied for the parent directory in which the new directory is to be added."
#define WARNING09 "A directory named with the filename already exists."
#define WARNING10 "The parent directory of the directory being created is on a read-only file system and cannot be modified."
#define WARNING11 "WARNING11"
#define WARNING12 "WARNING12"
#define WARNING13 "Unable to open file:%s"
#define WARNING14 "Unable to create file:%s"
#define WARNING15 "Unable to get zhavam home:%s"
#define WARNING16 "Could not parse file %s"
#define WARNING17 "WARNING17"
#define WARNING18 "WARNING18"
#define WARNING19 "WARNING19"

/**
 * ERROR icon, message length aand messages
 */
#define ERRORMSGLEN 600
#define ERROR01 ".zhavam exist but it is not a directory %s"
#define ERROR02 "zhavam.conf exists but it is not a file %s"
#define ERROR03 "ERROR03"
#define ERROR04 "ERROR04"
#define ERROR05 "Invalid directory:%s %s"
#define ERROR06 "Search permission is denied for a component of the path prefix."
#define ERROR07 "An error occurred while reading from the file system."
#define ERROR08 "A loop exists or more than {SYMLOOP_MAX} in symbolic links encountered during resolution of the path argument."
#define ERROR09 "The length of the path argument exceeds {PATH_MAX} or a pathname component is longer than {NAME_MAX}."
#define ERROR10 "A component of the path prefix is not a directory."
#define ERROR11 "The file size in bytes or the number of blocks allocated to the file or the file serial number cannot be represented correctly in the structure pointed to by buf."
#define ERROR12 "The parent directory has too many links (entries). Check if it could result from network access to a file system on another machine."
#define ERROR13 "The file system doesn’t have enough room to create the new directory."
#define ERROR14 "The path %s is not a directory."
#define ERROR15 "Unable to get statvfs info"
#define ERROR16 "Tree Selection is NULL"

/**
 * Macro substitutions for debuggin purposes
 */
#define ERROR(fmtstr,...) funcError(__FILE__, __FUNCTION__, __LINE__ - 1, errno, fmtstr, __VA_ARGS__)
#define WARNING(fmtstr,...) funcWarning(__FILE__, __FUNCTION__, __LINE__ - 1, errno, fmtstr, __VA_ARGS__)
#define TRACE(fmtstr,...) funcTrace(__FILE__, __FUNCTION__, __LINE__, fmtstr, __VA_ARGS__)

/*
 * Prototypes
 */
void funcError(const char *fileName, const char *functionName, const int lineNbr, const int errorNbr, const char *fmtstr, ...);
void eError(const char *fmtstr, ...);
void funcTrace(const char *fileName, const char *functionName, const int lineNbr, const char *fmtstr, ...);
int funcWarning(const char *fileName, const char *functionName, const int lineNbr, const int errorNbr, const char *fmtstr, ...);
void wWarning(const char *fmtstr, ...);
int statErrorMngr(int statError);
int mkdirErrorMngr(int mkdirError);

#endif /* SOURCE_ZHAVAM_ERRTRA_H_ */
