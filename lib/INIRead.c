#include "INIRead.h"

char g_szConfigPath[MAX_PATH];
//获取当前程序目录
int GetCurrentPath(char buf[],char *pFileName)
{

    char pidfile[64];
    int fd;
    sprintf(pidfile, "/proc/%d/cmdline", getpid());

    fd = open(pidfile, O_RDONLY, 0);
    read(fd, buf, 256);
    close(fd);
    buf[MAX_PATH] = '\0';
    char *p = &buf[strlen(buf)];
    do
    {
        *p = '\0';
        p--;
    } while( '/' != *p );
    p++;
    //配置文件目录
    memcpy(p,pFileName,strlen(pFileName));
    return 0;
}

//从INI文件读取字符串类型数据
char *GetIniKeyString(char *title,char *key,char *filename)
{
    FILE *fp;
    char szLine[LINE_MAX_LEN];
    char szLineTmp[LINE_MAX_LEN];
    static char tmpstr[LINE_MAX_LEN]={0};

    char rtnval;
    int i = 0;
    int j = 0;
    int index = 0;
    int flag = 0;
    char *tmp;

    if(NULL == (fp = fopen(filename, "r")))
    {
        printf("have   no   such   file \n");
        return 0;
    }
    while(!feof(fp))
    {
        memset(tmpstr, 0, 1024);
        rtnval = fgetc(fp);
        if(EOF == rtnval)
        {
            break;
        }
        else
        {
            szLine[i] = rtnval;
            szLineTmp[i] = rtnval;
            i++;
            if(('#' == szLine[0])||(';' == szLine[0]))
            {
            }
            else if('/' == szLine[0]&&'/' == szLine[1])
            {
            }
        }
        if(rtnval == '\n')
        {
            i--;
            szLine[i] = '\0';
            szLineTmp[i] = '\0';
            i = 0;
            tmp = strchr(szLine, '=');
            for(j = 0;j<strlen(szLine);j++)
            {
                if(szLineTmp[j] == '=')
                {
                    szLineTmp[j] = '\0';
                    break;
                }
            }
            if(( tmp != NULL )&&(flag == 1))
            {
                if(strcmp(szLineTmp,key) == 0)
                {
                    //注释行
                    if( ('#' == szLine[0])||(';'==szLine[0]))
                    {
                    }
                    else if ( '/' == szLine[0] && '/' == szLine[1] )
                    {
                    }
                    else
                    {
                        //找打key对应变量
                        //strcpy(tmpstr,tmp+1);
                        tmp++;
                        while(*tmp != '\n' && *tmp != '\0') {
                            tmpstr[index] = *tmp;
                            tmp++;
                            index++;
                        }
                        fclose(fp);
                        return tmpstr;
                    }
                }
            }
            else
            {
                strcpy(tmpstr,"[");
                strcat(tmpstr,title);
                strcat(tmpstr,"]");
                if(strncmp(tmpstr,szLine,strlen(tmpstr)) == 0 )
                {
                    //找到title
                    flag = 1;
                }
            }
        }
    }
    fclose(fp);
    return NULL;
}

//从INI文件读取整类型数据
int GetIniKeyInt(char *title,char *key,char *filename)
{
    return atoi(GetIniKeyString(title,key,filename));
}



int isSection(char* line, char *section) {
    int len;

    SPC_Trim(line);
    SPC_Strip(line);
    len = strlen(line);

    if(len < 2) {
        return FALSE;
    }
    if(line[0] == '[' && line[len - 1] == ']') {
        strncpy(section, line + 1, len - 2);
        section[len - 2] = '\0';
        return TRUE;
    }else{
        return FALSE;
    }
}

struct Node*GetSections(char *filename) {
    FILE *fp;
    char line[LINE_MAX_LEN];
    char section[NAME_LEN];
    struct Node *node, *ptr,*sectList;
    ptr = sectList = NULL;

    if (NULL == (fp = fopen(filename,"rb"))) {
        return NULL;
    }

    memset(line, 0x00, LINE_MAX_LEN);
    while( fgets(line, LINE_MAX_LEN, fp) != NULL ) {
        if(TRUE == isSection(line, section)) {
            node = (struct Node*)malloc(sizeof(struct Node));
            if(NULL == node) {
                printf("[ERR] Failed to malloc Node\n");
                exit(EXIT_FAILURE);
            }
            memset(node, 0x00, sizeof(struct Node));
            strcpy(node->value, section);
            node->next = NULL;

            if(sectList == NULL) {
                sectList = node;
            } else {
                ptr->next = node;
            }
            ptr = node;
        }
        memset(line, 0x00, LINE_MAX_LEN);
    }

    fclose(fp);

    return sectList;
}


int GetItems(char *filename, char *section, struct Node *itemList) {

    return SPC_OK;
}
