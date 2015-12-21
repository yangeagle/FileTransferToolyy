
#include "clients.h"
#include "common_utils.h"
#include "log.h"

/*
 *Add new client
 *
*/

int AddNewClient(TClient **first, TClient *new_client)
{
    if (new_client == NULL)
    {
        LOG_MESG(EERROR, "TClient pointer is NULL.\n");
        return -1;
    }

    if (!(*first))
    {

        LOG_MESG(EGENERAL, "The first request in.\n");
        new_client->entries.pre = new_client;
        new_client->entries.next = new_client;
        *first = new_client;
        return 0;
    }

    TClient *tmp = (*first)->entries.next;

    if (*first == tmp)
    {

        new_client->entries.next = tmp;
        new_client->entries.pre = tmp;

        tmp->entries.next = new_client;
        tmp->entries.pre = new_client;

        if (new_client->plevel > tmp->plevel)
        {

            *first = new_client;
        }

        return 0;
    }


    while(*first != tmp)
    {
        if (new_client->plevel > tmp->plevel)
        {

            new_client->entries.next = tmp;
            new_client->entries.pre = tmp->entries.pre;

            tmp->entries.pre = new_client;

            return 0;
        }
        tmp = tmp->entries.next;
    }

    new_client->entries.next = tmp;
    new_client->entries.pre = tmp->entries.pre;

    tmp->entries.pre = new_client;

    return 0;
}





/*
 *Remove client
 *
*/

int RemoveClient(TClient *first)
{

    return 0;
}
