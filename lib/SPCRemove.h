#ifndef __SPC_REMOVE_H
#define __SPC_REMOVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "SPCLog.h"

#define MAX_LINE 100

//FUNCTIONS
long int  getFileSize(char *filename);
int       SPC_REMOVE(char *filename);
char     *SPC_TRIM(char *fline);

#endif
