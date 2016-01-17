#ifndef __INIREAD_H
#define __INIREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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
int isItem(char* line, struct Node *item);
struct List *GetSections(char *filenamet);
struct List *GetItems(char *filename, char *section);

#endif
