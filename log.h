#ifndef LOG_H
#define LOG_H
#include <stdio.h>

typedef enum LOG_LEVEL_TAG
{
    EGENERAL,
    EWARN,
    EERROR,
    EFATAl,
}LOG_LEVEL;


extern int log_flag;
extern FILE *log_fp;


void log_print(char *fname, int lineno, int priority, char *fmt, ...);

#define LOG_MESG(priority, fmt, args...)  log_print(__FILE__, __LINE__, priority, fmt, ##args)


void log_close();

#endif // LOG_H
