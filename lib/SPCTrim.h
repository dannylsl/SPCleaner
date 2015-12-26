#ifndef __SPC_TRIM_H
#define __SPC_TRIM_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "SPCLog.h"

#define MAX_LINE 100

//FUNCTIONS
long int  getFileSize(char *filename);
int       SPC_TRIM(char *filename);
char     *SPC_RTRIM(char *fline);

#endif
