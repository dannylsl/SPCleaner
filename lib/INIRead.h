#ifndef __INIREAD_H
#define __INIREAD_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include "SPCLog.h"
#include "SPCString.h"
#include "SPCList.h"


#define MAX_FILE PATH "Config.ini"
#define MAX_PATH    256
#define NAME_LEN    128

//FUNCTIONS
int GetCurrentPath(char buf[],char *pFileName);
char *GetIniKeyString(char *title,char *key,char *filename);
int GetIniKeyInt(char *title,char *key,char *filename);

int isSection(char* line, char *section);
struct Node *GetSections(char *filenamet);
int GetItems(char *filename, char *section, struct Node *itemList);

#endif
