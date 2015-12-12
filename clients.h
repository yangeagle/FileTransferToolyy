#ifndef CLIENTS_H
#define CLIENTS_H

#include <time.h>
#include "service.h"


typedef struct client_capsule
{
    int socket;
    TRequestParam *req;
    TResponseParam *resp;

    time_t timeout;
}TClientCaps;





#endif // CLIENTS_H
