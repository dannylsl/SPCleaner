#ifndef __SPC_FETCH_H
#define __SPC_FETCH_H

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "SPCLog.h"
#include "SPCTrim.h"

#define MAX_LINE 100

//FUNCTIONS
int    SPC_FETCH(char *path, int flag);
int    get_file(const char *dirname, int flag);

#endif
