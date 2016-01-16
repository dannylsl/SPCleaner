#include "SPC_RTRIM.h"

char *SPC_RTRIM(char *fline) {
    int     unLen = 0;
    char    CRLR[3];

    memset(CRLR, 0X00, sizeof(CRLR));

    unLen = strlen(fline);
    if((fline[unLen-1] != '\n') && (fline[unLen-1] != '\r')) { /*NULL*/
        strcat(CRLR, "\0\0\0");
    }
    if(unLen >= 2) {
        if(('\n' == fline[unLen-1]) && ('\r' == fline[unLen-2])) { /*DOS*/
            strcat(CRLR, "\r\n\0");
            fline[unLen-2] = '\0';
        }
        else if(('\n' == fline[unLen-1]) && (fline[unLen-2] != '\r')) { /*UNIX -S*/
            strcat(CRLR, "\n\0\0");
            fline[unLen-1] = '\0';
        }
    }
    else {
        if('\n' == fline[unLen-1]) { /*UNIX -C*/
            strcat(CRLR, "\n\0\0");
            fline[unLen-1] = '\0';
        }
        else if('\r' == fline[unLen-1]) { /*MAC*/
            strcat(CRLR, "\r\0\0");
            fline[unLen-1] = '\0';
        }
    }

    unLen = strlen(fline);
    if(unLen != 0) {
        for(unLen -= 1; unLen >= 0; unLen--) {
            if((' ' == fline[unLen]) || ('\t' == fline[unLen])) {
                fline[unLen] = '\0';
                continue;
            }
            else {
                unLen++;
                fline[unLen] = '\0';
                break;
            }
        }
    }
    strcat(fline, CRLR);

    return fline;
}
