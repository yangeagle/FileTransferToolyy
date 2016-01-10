/*
 *response thread
 *
 *
*/

#include <pthread.h>

#include "common_utils.h"


#include "log.h"


#define THREAD_STACK_SIZE   (128*1024)

static void *response_thread_main_loop(void *arg);

int response_thread_init()
{
    pthread_t pthread_id;
    pthread_attr_t attr;

    if (pthread_attr_init(&attr))
    {
        LOG_MESG(EERROR, "pthread attribute init failed.\n");
        return -1;
    }

    if (pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE))
    {
        LOG_MESG(EERROR, "pthread setstack size failed.\n");
        return -1;
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
    {
        LOG_MESG(EERROR, "pthread setdetach state failed.\n");
        return -1;
    }

    if (pthread_create(&pthread_id, &attr, response_thread_main_loop, NULL))
    {
        LOG_MESG(EERROR, "pthread create failed.\n");
    }

    if (pthread_attr_destroy(&attr))
    {
        LOG_MESG(EERROR, "pthread attribute destroy failed.\n");
        return -1;
    }

    return 0;
}

static void *response_thread_main_loop(void *arg)
{

    while(quit_flag)
    {

    }
}
