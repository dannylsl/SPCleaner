#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define CONF_FILE_PATH  "Config.ini"
#define  MAX_PATH 260

char g_szConfigPath[MAX_PATH];
//获取当前程序目录
int GetCurrentPath(char buf[],char *pFileName)
{

    char pidfile[64];
    int bytes;
    int fd;
    sprintf(pidfile, "/proc/%d/cmdline", getpid());

    fd = open(pidfile, O_RDONLY, 0);
    bytes = read(fd, buf, 256);
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
char *GetIniKeyString(char *title,char *value,char *filename)
{
    FILE *fp;
    char szLine[1024];
    static char tmpstr[1024];
    char keystr[1024];
    char rtnval;
    int i = 0;
    int flag = 0;
    char *tmp;
    char section[1024];
    char key[1024];

    if(NULL==(fp = fopen(filename, "r")))
    {
        printf("have   no   such   file \n");
        return 0;
    }

    printf("The section name is:\n");
    scanf("%s",section);
    printf("The key is:\n");
    scanf("%s",key);

    while(!feof(fp))
    {
        rtnval = fgetc(fp);
        if((EOF==rtnval)&&(sizeof(szLine)!=0))
        {
            break;
        }
        else
        {
            szLine[i++] = rtnval;
        }
        if(rtnval == '\n')
        {
            szLine[--i] = '\0';
            if(('#'==szLine[0])||(';'==szLine[0]))
            {
            }
            else if('/'==szLine[0]&&'/'==szLine[1])
            {
            }
            else
                i = 0;
            tmp = strchr(szLine, '=');

            if(( tmp != NULL )&&(flag == 1))
            {
                if(strstr(szLine,value)!=NULL)
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
                        strncpy(keystr,szLine,(int)(tmp-szLine));
                        if(strcmp(keystr,key)==0)
                        {
                        }
                    }
                }
                else
                {
                    //找打value对应变量
                    strcpy(tmpstr,tmp+1);
                    fclose(fp);
                    return tmpstr;
                }
            }
            else
            {
                strcpy(tmpstr,"[");
                strcat(tmpstr,title);
                strcat(tmpstr,"]");
                if((strncmp(tmpstr,szLine,strlen(tmpstr)) == 0 )&&
                        (strncmp(tmpstr,section,strlen(tmpstr)==0)))
                {
                    //找到title
                    flag = 1;
                }
            }
        }
    }
    fclose(fp);
    return "";
}

//从INI文件读取整类型数据
int GetIniKeyInt(char *title,char *value,char *filename)
{
    return atoi(GetIniKeyString(title,value,filename));
}
int main(int argc, char* argv[])
{
    char buf[MAX_PATH];
    memset(buf,0,sizeof(buf));
    GetCurrentPath(buf,CONF_FILE_PATH);
    strcpy(g_szConfigPath,buf);

    int iCatAge;
    char szCatName[32];

    iCatAge = GetIniKeyInt("CAT","age",g_szConfigPath);
    char *a = GetIniKeyString("CAT","name",g_szConfigPath);
    printf("%d\r\n",iCatAge);
    printf("%s\r\n",a);

    return 0;
}
