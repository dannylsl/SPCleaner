#ifndef __SPC_TRIM_H
#define __SPC_TRIM_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "SPCLog.h"

#define LINE_MAX_LEN 200

#ifdef WIN32
#define N "\r\n\0"
#else
#define N "\n\0"
#endif
//FUNCTIONS
long int  getFileSize(char *filename);
int       SPC_TRIM(char *filename);
char     *SPC_RTRIM(char *fline);
int       create_dir(char *sPathName);

#endif
