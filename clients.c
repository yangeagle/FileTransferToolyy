
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

    TClient *tmp = *first;

    do {
        if (new_client->plevel > tmp->plevel)
        {
            new_client->entries.next = tmp;
            new_client->entries.pre = tmp->entries.pre;

            tmp->entries.pre = new_client;

            if (new_client->plevel > (*first)->plevel)
            {
                *first = new_client;
            }
            break;
        }

        tmp = tmp->entries.next;
    }while(tmp != *first);

    /*the last one*/
    if (tmp == first)
    {
        new_client->entries.next = tmp;
        new_client->entries.pre = tmp->entries.pre;

        tmp->entries.pre = new_client;
    }



    return 0;
}


/*
 *Remove client
 *
*/

int RemoveClient(TClient **first, TClient *remove_client)
{

    if (first == NULL || *first == NULL)
    {
        LOG_MESG(EFATAl, "Invalid client link head.\n");
        return -1;
    }

    TClient *pre = remove_client->entries.pre;
    TClient *next = remove_client->entries.next;

    /*removed node is the only node*/
    if (pre == next)
    {
        *first = NULL;
        DeleteClient(remove_client);
        return 0;
    }

    /*removed node is the first node*/
    if (*first == remove_client)
    {
        *first = next;
    }


    pre->entries.next = next;
    next->entries.pre = pre;

    DeleteClient(remove_client);

    return 0;
}



int DeleteClient(TClient *client)
{
    LOG_MESG(EGENERAL, "DeleteCient() Enter.\n");

    if (client->req)
    {
        free(client->req);
        client->req = NULL;
    }

    if (client->resp)
    {
        free(client->resp);
        client->resp = NULL;
    }

    free(client);

    return 0;
}

