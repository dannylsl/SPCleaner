#include "SPCLog.h"

char *getTimeStamp() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("year=%d month=%d day=%d hour=%d min=%d sec=%d\n",
            timeinfo->tm_year+1900,
            timeinfo->tm_mon,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);

}

void SPC_MSG(int level, const char* msg) {

}

