#ifndef LOG_H
#define LOG_H

typedef enum LOG_LEVEL_TAG
{
    EGENERAL,
    EWARN,
    EERROR,
    EFATAl,
}LOG_LEVEL;


extern int log_flag;


void log_print(char *fname, int lineno, int priority, char *fmt, ...);

#define LOG_MESG(priority, fmt, args...)  log_print(__FILE__, __LINE__, priority, fmt, ##args)



#endif // LOG_H
