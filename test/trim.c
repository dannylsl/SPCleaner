#include "../lib/SPCString.h"
#include "../lib/SPCLog.h"

int main() {

    SPC_INIT();

    char str[]="     \tHello World\n";

    char str1[]="     \tHello World   \t\n";
    char str2[]="     \tHello World  \t  \r\n";
    char str3[]="     \tHello World  \t1  \r";
    char str4[]="\n";
    char str5[]="\r\n";
    char str6[]="\r";
    char str7[]="";
    char str8[]="a";

    SPC_MSG(LOGINF,str);
    SPC_MSG(LOGINF,SPC_LTrim(str));


    printf("STR[%s] LEN[%d]\n",str1, strlen(str1));
    SPC_RTrim(str1);
    printf("STR[%s] LEN[%d]\n",str1, strlen(str1));

    printf("STR[%s] LEN[%d]\n",str2, strlen(str2));
    SPC_RTrim(str2);
    printf("STR[%s] LEN[%d]\n",str2, strlen(str2));

    printf("STR[%s] LEN[%d]\n",str3, strlen(str3));
    SPC_RTrim(str3);
    printf("STR[%s] LEN[%d]\n",str3, strlen(str3));

    printf("STR[%s] LEN[%d]\n",str4, strlen(str4));
    SPC_RTrim(str4);
    printf("STR[%s] LEN[%d]\n",str4, strlen(str4));

    printf("STR[%s] LEN[%d]\n",str5, strlen(str5));
    SPC_RTrim(str5);
    printf("STR[%s] LEN[%d]\n",str5, strlen(str5));

    printf("STR[%s] LEN[%d]\n",str6, strlen(str6));
    SPC_RTrim(str6);
    printf("STR[%s] LEN[%d]\n",str6, strlen(str6));

    printf("STR[%s] LEN[%d]\n",str7, strlen(str7));
    SPC_RTrim(str7);
    printf("STR[%s] LEN[%d]\n",str7, strlen(str7));

    printf("STR[%s] LEN[%d]\n",str8, strlen(str8));
    SPC_RTrim(str8);
    printf("STR[%s] LEN[%d]\n",str8, strlen(str8));



    SPC_FREE();
    return 0;
}
