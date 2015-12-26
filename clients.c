
#include "clients.h"
#include "common_utils.h"
#include "log.h"


/*
 *
 * Add new client
 *
*/
int AddNewClient(TClient **first, int fd)
{
    LOG_MESG(EGENERAL, "AddNewClient(%d)\n", fd);
    if (fd < 0)
    {
        LOG_MESG(EWARN, "Invalid file descriptor.\n");
        return -1;
    }

    TClient *new_client = (TClient *)malloc(sizeof(TClient));
    if (!new_client)
    {
        LOG_MESG(EERROR, "Mem malloc failed:%s.\n", strerror(errno));
        return -1;
    }

    new_client->socket = fd;
    new_client->timeout = time();
    new_client->plevel = PRI_NORMAL;

    /*the first one*/
    if (!(*first))
    {

        LOG_MESG(EGENERAL, "The first request in.\n");
        new_client->entries.pre = new_client;
        new_client->entries.next = new_client;
        *first = new_client;
        return 0;
    }

    /*insert into link list according to the priority*/
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

    /*the minimum priority level*/
    /*insert at the end of the link list*/
    if (tmp == first)
    {
        new_client->entries.next = tmp;
        new_client->entries.pre = tmp->entries.pre;

        tmp->entries.pre = new_client;
    }

    return 0;
}


/*
 *
 * Remove client
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



static int DeleteClient(TClient *client)
{
    LOG_MESG(EGENERAL, "DeleteClient() Enter.\n");
    if (!client)
    {
        LOG_MESG(EWARN, "Invalid client.\n");
        return -1;
    }

    if (client->socket >= 0)
    {
        close(client->socket);
        client->socket = -1;
    }

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

/*
 * Delete all clients.
 *
*/
int DeleteClients(TClient **first)
{
    TClient *tmp = *first;
    do {
        DeleteClient(tmp);
        tmp = tmp->entries.next;
    }while(tmp != *first);

    /*Delete the first node in the end*/
    DeleteClient(*first);
    *first = NULL;
    return 0;
}

/*
 * Check client active or inactive during the interval time
 *
 * parameter:
 * interval in seconds
*/
int CheckActivity(TClient **first, int interval)
{
    TClient *tmp = *first;
    time_t current = time();
    do {

        if (current > tmp->timeout + interval)
        {
            RemoveClient(first, tmp);
        }

        tmp = tmp->entries.next;
    }while(tmp != *first && *first);

    return 0;
}
