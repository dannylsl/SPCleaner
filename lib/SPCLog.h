#ifndef __SPC_LOG_H
#define __SPC_LOG_H

#include <stdio.h>
#include "ColorPrint.h"
#include "Macros.h"

#define TS_LEN          30
#define MSG_MAX_LEN     1024
#define MSG_TYPE_LEN    20


// LOG TIME TYPE
#define TS_USEC         1
#define TS_DATE         2
#define TS_DATE_USEC    3

// LOG LEVEL
#define LOGINF 0
#define LOGWAN 1
#define LOGERR 2
#define LOGDBG 3

// CONFIG TOGGLE
#define DEBUG 1
#define TS_TYPE TS_USEC
#define LOGNAME "SPCleaner.log"

// Global variant
FILE *fp_log;

// FUNCTIONS
int getTimeStamp(char *ts);
int SPC_INIT();
void SPC_MSG(int level, const char *format, ...);
int SPC_FREE();

#endif
