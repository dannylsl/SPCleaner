#include <stdio.h>
#include "Iniread.h"

#define CONF_FILE_PATH "Config.ini"

char g_szConfigPath[MAX_PATH];

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
    printf("%d\n",iCatAge);
    printf("%s\n",a);

    return 0;
}
