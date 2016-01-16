#include "SPCString.h"

char *SPC_LTrim(char *line) {

    char *ptr = line;

    while(*ptr == ' ' || *ptr == '\t') {
        ptr++;
    }
    strcpy(line, ptr);
    return line;
}

char *SPC_RTrim(char *line) {

    char CRLF[CRLF_LEN] = {0};
    int len = strlen(line);
    char *end;

    if(len <= 1) {
        return line;
    }

    end = line + len - 1;
    if(*end == '\r') {      /* MAC CRLF */
        strcpy(CRLF,"\r\0\0");

    }else if(*end == '\n') {
        end--;
        if(*end == '\r') {  /* DOS CRLF */
            if( len == 2) {
                return line;
            }
            strcpy(CRLF,"\r\n\0");
        }else {             /* UNIX CRLF */
            strcpy(CRLF,"\n\0\0");
        }
    }else{                  /* NO NEW LINE */
        memset(CRLF, 0x00, CRLF_LEN);
    }

    end--;
    while(end != line) {
        if(*end == ' ' || *end == '\t') {
            *end = '\0';
            end--;
        }else{
            break;
        }
    }

    strcat(line, CRLF);
    return line;
}


char *SPC_Trim(char *line) {

    return SPC_RTrim(SPC_LTrim(line));
}



char *SPC_Strip(char *line) {
    int len;
    char *end = NULL;

    if(line == NULL)
        return line;

    len = strlen(line);
    end = line + len - 1;

    while(*end == '\n' || *end == '\r') {
        *end = '\0';
        end = end - 1;
    }
    return line;
}
