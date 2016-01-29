#include "SPCStatis.h"

int SPC_STATIS(char *filepath) {
    int     commentline = 0;
    int     next = 0;
    int     found_flag = 0;
    int     lenth = 0;
    int     loop = 0;
    int     space_flag = 0;
    int     spaceline = 0;
    int     totalline = 0;
    char    fileline[LINE_MAX_LEN];
    char    *Hfound = NULL;
    char    *Tfound = NULL;
    FILE    *pRead = NULL;

    memset(fileline, 0x00, LINE_MAX_LEN);

    pRead = fopen(filepath, "rb");
    if(NULL == pRead) {
        SPC_MSG(LOGERR, "File open fail!");
        return SPC_ERR;
    }

    while(fgets(fileline, LINE_MAX_LEN, pRead) != NULL) {

        totalline++;

        space_flag = 0;
        if((0 == next) && (0 == found_flag)) {
            lenth = strlen(fileline) - 1;
            if((0 == lenth) && ('\n' == fileline[0])) {
            }
            else {
                for(loop = 0; loop < lenth; loop++) {
                    if(fileline[loop] != ' ') {
                        space_flag = 1;
                        break;
                    }
                }
            }
            if(space_flag != 1) {
                spaceline++;
                continue;
            }
        }

        if(next != 1) {
           if((Hfound = strchr(fileline, '/')) != NULL) {
                if('*' == Hfound[1]) {
                    found_flag = 1;
                    commentline++;
                }
                else if('/' == Hfound[1]) {
                    commentline++;
                    continue;
                }
                else {
                    continue;
                }
           }
        }
        if(1 == found_flag) {
            if((Tfound = strrchr(fileline, '*')) != NULL) {
                if('/' == Tfound[1]) {
                    found_flag = 0;
                    next = 0;
                }
                else {
                    commentline++;
                    next = 1;
                }
            }
            else {
                commentline++;
            }
        }
    }

    printf("Total totalline lines=[%d]\n", totalline);
    printf("Total comment lines=[%d]\n", commentline);
    printf("Total space lines=[%d]\n", spaceline);

    return SPC_OK;
}
