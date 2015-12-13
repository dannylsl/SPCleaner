#ifndef __SPC_LOG_H
#define __SPC_LOG_H

#include <time.h>

// LOG LEVEL
#define LOGINF 0
#define LOGWAN 1
#define LOGERR 2
#define LOGDBG 3

// TOGGLE
#define DEBUG 1

// FUNCTIONS
char *getTimeStamp();
void SPC_MSG(int level, const char* msg);

#endif
