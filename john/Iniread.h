#ifndef __INIREAD_H
#define __INIREAD_H

#include <stdio.h>

#define MAX_PATH 260
#define MAX_LEN 1024

//FUNCTIONS
char g_szConfigPath[MAX_PATH]
char *GetIniKeyString(char *title,char *key,char *filename)
int GetIniKeyInt(char *title,char *key,char *filename)
