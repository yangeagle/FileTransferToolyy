/*
 * File Transfer Tool
 * log
 *
 * Author:ZYY
 * date:2015-11-028
 *
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

int log_flag = 1;
FILE *log_fp = NULL;


int log_init(char *logname)
{
    FILE *fp = NULL;

    fp = fopen(logname, "w");
    if (!fp)
    {
        return -1;
    }

    log_fp = fp;
    return 0;
}

void log_print(char *fname, int lineno, int priority, char *fmt, ...)
{
    if (!log_flag)
    {
        return;
    }

    if (!log_fp)
    {
        log_fp = stdout;
    }

    struct timeval tv;
    struct tm time_tm;

    gettimeofday(&tv, NULL);

    localtime_r(&(tv.tv_sec), &time_tm); //get local time
    fprintf (log_fp, "%d-%02d-%02d ", (1900 + time_tm.tm_year), (1 + time_tm.tm_mon), time_tm.tm_mday);
    fprintf(log_fp, "%02d:%02d:%02d %lu: [%s:%d] ", time_tm.tm_hour, time_tm.tm_min, time_tm.tm_sec, tv.tv_usec, fname, lineno);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(log_fp, fmt, ap);
    va_end(ap);

    fflush(log_fp);
}


void log_close()
{
    if (!log_fp)
    {
        fflush(log_fp);
        fclose(log_fp);
        log_fp = NULL;
    }
}



