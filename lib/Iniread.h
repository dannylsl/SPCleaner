#ifndef __INIREAD_H
#define __INIREAD_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_FILE PATH "Config.ini"
#define MAX_PATH 260
#define MAX_LEN 1024

//FUNCTIONS
int GetCurrentPath(char buf[],char *pFileName);
char *GetIniKeyString(char *title,char *key,char *filename);
int GetIniKeyInt(char *title,char *key,char *filename);

#endif
