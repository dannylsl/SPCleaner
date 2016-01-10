#include "SPCFetch.h"

int SPC_FETCH(char *path, int flag) {

    /*定义私有变量*/
    int unResult = 0;
    struct stat statbuf;
    char *pFilepath = NULL;

    memset(&statbuf, 0x00, sizeof(statbuf));

    SPC_INIT();

    if(SPC_ERR == stat(path, &statbuf)) {
        SPC_MSG(LOGERR, "Get path stat fail!");
        return SPC_ERR;
    }

    if(S_ISREG(statbuf.st_mode)) {
        pFilepath = path;
        printf("%s\n", pFilepath);
        unResult = SPC_TRIM(pFilepath);
        if(unResult != 0) {
            SPC_MSG(LOGERR, "SPCTrim error in SPC_FETCH!");
            return SPC_ERR;
        }
    }
    else if(S_ISDIR(statbuf.st_mode)) {
        unResult = get_file(path, flag);
        if(unResult != SPC_OK) {
            SPC_MSG(LOGERR, "get_file is error!");
            return SPC_ERR;
        }
    }

    return SPC_OK;
}

int get_file(const char *dirname, int flag) {
    int unResult = 0;
    DIR *pDir = NULL;
    char buffer[MAX_LINE];
    struct dirent *pDirent = NULL;
    struct stat statbuf;

    memset(&statbuf, 0x00, sizeof(statbuf));

    pDir = opendir(dirname);
    if(NULL == pDir) {
        SPC_MSG(LOGERR, "Open directory fail!");
        return SPC_ERR;
    }

   while((pDirent = readdir(pDir)) != NULL) {
        if((strcmp(pDirent->d_name, ".") != 0)
            && (strcmp(pDirent->d_name, "..") != 0)) {
            if(flag != 0) {
                if('.' == pDirent->d_name[0]) {
                    continue;
                }
            }
            memset(buffer, 0x00, MAX_LINE);
            strcat(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, pDirent->d_name);
            unResult = stat(buffer, &statbuf);
            if(SPC_ERR == unResult) {
                SPC_MSG(LOGERR, "Get file stat fail!");
                return SPC_ERR;
            }
            if(S_ISREG(statbuf.st_mode)) {
                printf("%s\n", buffer);
                unResult = SPC_TRIM(buffer);
                if(unResult != 0) {
                    SPC_MSG(LOGERR, "SPCTrim error in get_files!");
                    return SPC_ERR;
                }
                continue;
            }
            else if(S_ISDIR(statbuf.st_mode)) {
                unResult = get_file(buffer, flag);
                if(unResult != 0) {
                    SPC_MSG(LOGERR, "get_file is error!");
                    return SPC_ERR;
                }
                continue;
            }
       }
    }

    return SPC_OK;
}
