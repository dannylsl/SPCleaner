#include <stdio.h>
#include "SPCLog.h"

int main() {
    char ts[TS_LEN];

    getTimeStamp(ts);
    printf("TS: %s\n", ts);

    SPC_INIT();

    SPC_MSG(LOGINF,"LOGINF MESSAGE");
    SPC_MSG(LOGWAN,"LOGWAN MESSAGE");
    SPC_MSG(LOGERR,"LOGERR MESSAGE");
    SPC_MSG(LOGDBG,"LOGDBG MESSAGE");
    SPC_MSG(10,"TYPE ERROR MESSAGE");

    SPC_FREE();
    return 0;
}
