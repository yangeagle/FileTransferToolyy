#ifndef CLIENTS_H
#define CLIENTS_H

#include <time.h>
#include "service.h"
#include "common_utils.h"

typedef struct client_capsule
{
    int socket;
    TRequestParam *req;
    TResponseParam *resp;

    time_t timeout;
    LINK_LIST_ENTRY(client_capsule) entries;
}TClient;





#endif // CLIENTS_H
