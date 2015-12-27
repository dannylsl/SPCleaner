#include "SPCTrim.h"

int SPC_TRIM(char *path) {

    /*定义私有变量*/
    FILE      *pRead = NULL;
    FILE      *pCopy = NULL;
    int       lenth = 0;
    int       temp = 0;
    int       unResult = 0;
    long      size1 = 0;
    long      size2 = 0;
    float     effic = 0.00;
    char      outputfile[MAX_LINE] = "./copy/";
    char      fline[MAX_LINE];
    char      *filename = NULL;

    /*变量初始化*/
    memset(fline, 0x00, MAX_LINE);

    SPC_INIT();

#ifdef WIN32
    if(access("./copy", 0) != 0) {
        /*创建输出目录*/
        unResult = mkdir("./copy");
        if(unResult != 0) {
            SPC_MSG(LOGERR, "Make directory fail!");
            return SPC_ERR;
        }
    }
#else
    if(access("./copy", 0) != 0) {
        /*创建输出目录*/
        unResult = mkdir("./copy", 0755);
        if(unResult != 0) {
            SPC_MSG(LOGERR, "Make directory fail!");
            return SPC_ERR;
        }
    }
#endif

    /*获取输出文件路径*/
    filename = strrchr(path, 47) + 1;
    strcat(outputfile, filename);

    /*打开处理文件-读取*/
    pRead = fopen(path, "r");
    if(NULL == pRead) {
        SPC_MSG(LOGERR, "File open fail!");
        return SPC_ERR;
    }
    size1 = getFileSize(path);

    /*打开输出文件-新建写入*/
    pCopy = fopen(outputfile, "w");
    if(NULL == pCopy) {
        SPC_MSG(LOGERR, "File open fail!");
        return SPC_ERR;
    }

    /*去空格处理*/
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
                unResult = fputs(SPC_RTRIM(fline), pCopy);
                if(EOF == unResult) {
                    SPC_MSG(LOGERR, "File write fail!");
                }
            }
        }
        else {
            unResult = fputs(SPC_RTRIM(fline), pCopy);
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

    /*输出文件前后差异*/
    size2 = getFileSize(outputfile);
    effic = (float)size2 / size1 * 100;
    printf("size1 = %ld ,size2 = %ld \n", size1, size2);
    printf("Code efficiency:%.02f%% \n", effic);

    return SPC_OK;
}

char *SPC_RTRIM(char *fline) {
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
