
#include "clients.h"
#include "common_utils.h"
#include "log.h"

/*
 *Add new client
 *
*/

int AddNewClient(TClient *first, TClient *new_client)
{
    if (new_client == NULL)
    {
        LOG_MESG(EERROR, "TClient pointer is NULL.\n");
        return -1;
    }

    TClient *tmp = first->entries.next;

    while(tmp != first)
    {
        if (new_client->plevel > tmp->plevel)
        {

            return 0;
        }
        tmp = tmp->entries.next;
    }



    return -1;
}





/*
 *Remove client
 *
*/

int RemoveClient(TClient *first)
{

    return 0;
}
