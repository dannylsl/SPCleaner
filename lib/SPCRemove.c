#include "SPCRemove.h"

int SPC_REMOVE(char *filename) {

    /*定义私有变量*/
    FILE      *pRead = NULL;
    FILE      *pCopy = NULL;
    int       lenth = 0;
    int       temp = 0;
    int       unResult = 0;
    long int  size1 = 0;
    long int  size2 = 0;
    float     effic = 0.00;
    char      outputfile[MAX_LINE] = "./copy/";
    char      fline[MAX_LINE];

    /*变量初始化*/
    memset(fline, 0x00, MAX_LINE);

    strcat(outputfile, filename);

    pRead = fopen(filename, "r");
    if(NULL == pRead) {
        SPC_MSG(LOGERR, "File open fail!");
        return 1;
    }
    size1 = getFileSize(filename);

    pCopy = fopen(outputfile, "w");
    if(NULL == pCopy) {
        SPC_MSG(LOGERR, "File open fail!");
        return 1;
    }

    while(fgets(fline, MAX_LINE, pRead) != NULL) {
        lenth = strlen(fline);
        if(1 == lenth) {
            fline[0] = '\n';
            unResult = fputs(fline, pCopy);
            if(EOF == unResult) {
                SPC_MSG(LOGERR, "File write fail!");
            }
        }
        else if(lenth == (MAX_LINE - 1)) {
            temp = MAX_LINE - 1;
            if(fline[temp] != '\n') {
                unResult = fputs(fline, pCopy);
                if(EOF == unResult) {
                    SPC_MSG(LOGERR, "File write fail!");
                }
                continue;
            }
            else {
                unResult = fputs(SPC_TRIM(fline), pCopy);
                if(EOF == unResult) {
                    SPC_MSG(LOGERR, "File write fail!");
                }
            }
        }
        else {
            unResult = fputs(SPC_TRIM(fline), pCopy);
            if(EOF == unResult) {
                SPC_MSG(LOGERR, "File write fail!");
            }
        }
    }
    unResult = fclose(pRead);
    if(unResult != 0) {
        SPC_MSG(LOGERR, "File [pRead] close fail !");
    }
    unResult = fclose(pCopy);
    if(unResult != 0) {
        SPC_MSG(LOGERR, "File [pCopy] close fail!");
    }

    size2 = getFileSize(outputfile);
    effic = (float)size2 / size1 * 100;
    printf("size1 = %ld ,size2 = %ld \n", size1, size2);
    printf("Code efficiency:%.02f%% \n", effic);

    return 0;
}

char *SPC_TRIM(char *fline) {
    int     unLen = 0;

    unLen = strlen(fline);
    if(1 == unLen) {
        fline[0] = '\n';
        unLen++;
        fline[unLen] = '\0';
    }
    else {
        for(unLen -= 2; unLen >= 0; unLen--) {
            if((' ' == fline[unLen]) || ('\t' == fline[unLen])) {
                if(0 == unLen) {
                    fline[unLen] = '\n';
                    unLen++;
                    fline[unLen] = '\0';
                    break;
                }
                continue;
            }
            else {
                unLen++;
                fline[unLen] = '\n';
                unLen++;
                fline[unLen] = '\0';
                break;
            }
        }
    }
    return fline;
}

long int getFileSize(char *filename) {
    struct stat statbuf;
    stat(filename, &statbuf);
    long int size = statbuf.st_size;

    return size;
}
