#ifndef __SPC_STRING_H
#define __SPC_STRING_H

#include <string.h>
#include "Macros.h"

#define CRLF_LEN 3

char *SPC_LTrim(char *line);
char *SPC_RTrim(char *line);
char *SPC_Trim(char *line);

char *SPC_Strip(char *line);
#endif
