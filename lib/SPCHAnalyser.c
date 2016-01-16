#include "SPCHAnalyser.h"


int SPC_HPrint(FILE *fp) {

    long pos = 0;

    pos = ftell(fp);
    if(pos < 0) {
        SPC_MSG(LOGERR,"ftell exception.");
    }
    return SPC_OK;
}
