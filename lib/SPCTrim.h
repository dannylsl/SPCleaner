#ifndef __SPC_TRIM_H
#define __SPC_TRIM_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "SPCLog.h"
#include "SPC_RTRIM.h"

//FUNCTIONS
long int  getFileSize(char *filename);
int       SPC_TRIM(char *filename);
int       create_dir(char *sPathName);

#endif
