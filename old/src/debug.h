#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

extern int debug_print;

void debug(char *s);

#define debug(s) if(debug_print) printf("%s: %s\n", __func__, s)

#endif
