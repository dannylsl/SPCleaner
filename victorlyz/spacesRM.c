#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define     MAX_LINE    100

char *spcTrim(char *str);
long int   file_size(char *filename);

int main(int argc, char *argv[]) {
    FILE    *pRead = NULL;
    FILE    *pCopy = NULL;
    int     lenth = 0;
    int     temp = 0;
    int     unResult = 0;
    long int size1 = 0;
    long int size2 = 0;
    float   effic = 0.00;
    char    filename[MAX_LINE];
    char    outputfile[MAX_LINE] = "./copy/";
    char    str[MAX_LINE];

    memset(str, 0x00, MAX_LINE);
    memset(filename, 0x00, MAX_LINE);

    if(argc != 2) {
        printf("Input parameter error! ./main [filename]\n");
        return -1;
    }
    strcpy(filename, argv[1]);
    strcat(outputfile, filename);

    pRead = fopen(filename, "r");
    if(NULL == pRead) {
        printf("File open fail ! 1\n");
        return 1;
    }
    size1 = file_size(filename);
    pCopy = fopen(outputfile, "w");
    if(NULL == pCopy) {
        printf("File open fail ! 2\n");
        return 1;
    }
    while(fgets(str, MAX_LINE, pRead) != NULL) {
        lenth = strlen(str);
        if(1 == lenth) {
            str[0] = '\n';
            unResult = fputs(str, pCopy);
            if(EOF == unResult) {
                printf("File write fial! \n");
            }
        }
        else if(lenth == (MAX_LINE - 1)) {
            temp = MAX_LINE - 1;
            if(str[temp] != '\n') {
                unResult = fputs(str, pCopy);
                if(EOF == unResult) {
                    printf("File write fial! \n");
                }
                continue;
            }
            else {
                unResult = fputs(spcTrim(str), pCopy);
                if(EOF == unResult) {
                    printf("File write fail! \n");
                }
            }
        }
        else {
            unResult = fputs(spcTrim(str), pCopy);
            if(EOF == unResult) {
                printf("File write fail! \n");
            }
        }
    }
    unResult = fclose(pRead);
    if(unResult != 0) {
        printf("File [pRead] close fail! unResult=[%d]\n", unResult);
    }
    unResult = fclose(pCopy);
    if(unResult != 0) {
        printf("File [pCopy] close fail! unResult=[%d]\n", unResult);
    }

    size2 = file_size(outputfile);
    effic = (float)size2 / size1 * 100;
    printf("size1 = %ld ,size2 = %ld \n", size1, size2);
    printf("Code efficiency:%.02f%% \n", effic);

    return 0;
}

char *spcTrim(char *str) {
    int     unLen = 0;
    int     unPosition =0;

    unLen = strlen(str);
    if(1 == unLen) {
        str[0] = '\n';
        unLen++;
        str[unLen] = '\0';
    }
    else {
        unLen -= 2;
    }
    for(unLen; unLen >= 0; unLen--) {
        if((' ' == str[unLen]) || ('\t' == str[unLen])) {
            if((0 == unLen)) {
                str[unLen] = '\n';
                unLen++;
                str[unLen] = '\0';
                break;
            }
            continue;
        }
        else {
            unLen++;
            str[unLen] = '\n';
            unLen++;
            str[unLen] = '\0';
            break;
        }
    }
    return str;
}

long int file_size(char *filename) {
    struct stat statbuf;
    stat(filename, &statbuf);
    long int size = statbuf.st_size;

    return size;
}
