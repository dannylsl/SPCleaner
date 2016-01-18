#ifndef __SPC_TRIM_H
#define __SPC_TRIM_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../lib/SPCLog.h"
#include "../lib/SPCString.h"

#define LINE_MAX_LEN 1024

//FUNCTIONS
long int  getFileSize(char *filename);
int       SPC_TRIM(char *filename);
int       create_dir(char *sPathName);

#endif
