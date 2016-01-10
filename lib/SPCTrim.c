#include "SPCTrim.h"

int SPC_TRIM(char *path) {

    /*定义私有变量*/
    FILE      *pRead = NULL;
    FILE      *pCopy = NULL;
    int       lenth = 0;
    int       temp = 0;
    int       i = 0;
    int       j = 0;
    int       unResult = 0;
    long      size1 = 0;
    long      size2 = 0;
    float     effic = 0.00;
    char      outputfile[MAX_LINE] = "./copy/";
    char      fline[MAX_LINE];
    char      filename[MAX_LINE];

    /*变量初始化*/
    memset(fline, 0x00, MAX_LINE);
    memset(filename, 0x00, MAX_LINE);

    /*获取输出文件路径*/
    lenth = strlen(path);
    for(i = 0; i < lenth; i++) {
        if('.' == path[i]) {
            if('/' == path[i+1]) {
                i++;
                continue;
            }
            if('.' == path[i+1]) {
                i += 2;
                continue;
            }
        }
        filename[j] = path[i];
        j++;
    }

    strcat(outputfile, filename);

    create_dir(outputfile);

    /*打开处理文件-读取*/
    pRead = fopen(path, "rb");
    if(NULL == pRead) {
        SPC_MSG(LOGERR, "File open fail!");
        return SPC_ERR;
    }
    size1 = getFileSize(path);

    /*打开输出文件-新建写入*/
    pCopy = fopen(outputfile, "wb");
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
    printf("\tsize1 = %ld ,size2 = %ld\n", size1, size2);
    printf("\tCode efficiency:%.02f%%\n", effic);

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

int create_dir(char *sPathName) {
    char DirName[MAX_LINE];
    char *sFileName = NULL;
    int  i = 0;
    int  len = 0;
    int  posit = 0;

    memset(DirName, 0X00, MAX_LINE);

    strcpy(DirName, sPathName);
    len = strlen(DirName);
    sFileName = strrchr(sPathName, 47) + 1;
    posit = strlen(sFileName);
    len -= posit;
    for(i = 1;i < len; i++) {
        if(DirName[i] == '/') {
            DirName[i] = 0;
            if(access(DirName, 0) != 0) {
#ifdef WIN32
                if(mkdir(DirName) == -1) {
                    SPC_MSG(LOGERR, "Make directory fail!");
                    return SPC_ERR;
                }
#else
                if(mkdir(DirName, 0755) == -1) {
                    SPC_MSG(LOGERR, "Make directory fail!");
                    return SPC_ERR;
                }
#endif
            }
            DirName[i] = '/';
        }
    }

    return 0;
}
