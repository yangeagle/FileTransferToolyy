#ifndef CLIENTS_H
#define CLIENTS_H

#include <time.h>
#include "service.h"
#include "common_utils.h"


typedef enum request_priority
{
    PRI_INVAL,
    PRI_LOW,
    PRI_NORMAL,
    PRI_HIGH,
    PRI_CRITICAL,

    PRI_MAX,
}PriorityLevel;

typedef struct client_capsule
{
    int socket;
    TRequestParam req;
    TResponseParam resp;

    PriorityLevel plevel;
    time_t timeout;
    LINK_LIST_ENTRY(client_capsule) entries;
}TClient;



int AddNewClient(TClient **first, int fd);

int CheckActivity(TClient **first, int interval);

int DeleteClients(TClient **first);

#endif // CLIENTS_H
