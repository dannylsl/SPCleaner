#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>
#include "SPCLog.h"

// External Global Variant
extern FILE *fp_log;

int getTimeStamp(char *ts) {

    memset(ts, 0x00, TS_LEN);

# if TS_TYPE == TS_USEC
    struct timeval secs;
    gettimeofday(&secs, NULL);
    sprintf(ts,"%ld.%ld",secs.tv_sec, secs.tv_usec);

# elif TS_TYPE == TS_DATE
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(ts, "%2d-%2d-%2d %2d:%2d:%2d",
            timeinfo->tm_year+1900,
            timeinfo->tm_mon+1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);

# elif TS_TYPE == TS_DATE_USEC
    time_t rawtime;
    struct tm *timeinfo;
    struct timeval secs;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    gettimeofday(&secs, NULL);

    sprintf(ts, "%2d-%2d-%2d %2d:%2d:%2d.%ld",
            timeinfo->tm_year+1900,
            timeinfo->tm_mon+1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec,
            secs.tv_usec);

#endif

    return SPC_OK;
}


int SPC_INIT() {

    if( fp_log != NULL ) {
        return SPC_OK;
    }else {
        if(NULL == (fp_log = fopen(LOGNAME, "wb"))) {
            printf("LOGFILE[%s] open failed!\n", LOGNAME);
            exit(EXIT_FAILURE);
        }
    }

    SPC_MSG(LOGDBG, "SPC_INIT called");
    return SPC_OK;
}


int SPC_FREE() {
    SPC_MSG(LOGDBG, "SPC_FREE called");
    if(NULL != fp_log) {
        fclose(fp_log);
    }
    return SPC_OK;
}


char *colorMsg(const char *msg, const char *color, char *msgc) {
    sprintf(msgc, "%s%s%s%c", color, msg, NC, '\0');
    return msgc;
}


void SPC_MSG(int level, const char *format, ...) {

    char ts[TS_LEN];
    char msg[MSG_MAX_LEN + 1];
    char log[MSG_MAX_LEN + 1];
    char logc[MSG_MAX_LEN + 1];
    char msgc[MSG_MAX_LEN + 1];
    char type[MSG_TYPE_LEN + 1];
    char typec[MSG_TYPE_LEN + 1];

    memset(msg, 0x00, MSG_MAX_LEN);
    memset(msgc, 0x00, MSG_MAX_LEN);
    memset(log, 0x00, MSG_MAX_LEN);
    memset(logc, 0x00, MSG_MAX_LEN);
    memset(type, 0x00, MSG_TYPE_LEN);
    memset(typec, 0x00, MSG_TYPE_LEN);

    getTimeStamp(ts);

    if(NULL == fp_log) {
        colorMsg("Call SPC_INIT before using SPC_MSG.", RED, logc);
        sprintf(msgc,"[%s] %s", ts, logc);
        printf("%s\n", msgc);
        exit(EXIT_FAILURE);
    }

    va_list argptr;
    va_start(argptr, format);
    vsprintf(log, format, argptr);
    va_end(argptr);

    switch(level) {
        case LOGINF:
            strcpy(type, "INF");
            sprintf(msg,"[%s] [%s] %s\n", ts, type, log);
            sprintf(msgc,"[%s] [%s] %s", ts, type, log);
            break;
        case LOGWAN:
            strcpy(type, "WAN");
            colorMsg(type, YELLOW, typec);
            sprintf(msg,"[%s] [%s] %s\n", ts, type, log);
            sprintf(msgc,"[%s] [%s] %s", ts, typec, log);
            break;
        case LOGERR:
            strcpy(type, "ERR");
            colorMsg(type, RED, typec);
            colorMsg(log, RED, logc);
            sprintf(msg,"[%s] [%s] %s\n", ts, type, log);
            sprintf(msgc,"[%s] [%s] %s", ts, typec, logc);
            break;
        case LOGDBG:
            strcpy(type, "DBG");
            colorMsg(type, BLUE, typec);
            sprintf(msg,"[%s] [%s] %s\n", ts, type, log);
            sprintf(msgc,"[%s] [%s] %s", ts, typec, log);
            break;
        default:
            colorMsg("ERROR MSG TYPE IN FUNCTION", RED, logc);
            sprintf(msg,"[%s] %s\n", ts, "ERROR MSG TYPE IN FUNCTION");
            sprintf(msgc,"[%s] %s", ts, logc);
            break;
    }

    //printf("%s\n",msg);
    printf("%s\n",msgc);
    fwrite(msg, strlen(msg), 1, fp_log);
    return;
}

