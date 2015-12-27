/*
*File Transfer tools
* Author:ZYY
*
*
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include "service.h"
#include "clients.h"
#include "log.h"

TRESULT_CODE browser_handler(TRequestParam *req, TResponseParam *resp);
TRESULT_CODE download_handler(TRequestParam *req, TResponseParam *resp);
TRESULT_CODE upload_handler(TRequestParam *req, TResponseParam *resp);

TMesgHandlers methods[] =
{
    {MESG_BROWSER, browser_handler},
    {MESG_DOWNLOAD, download_handler},
    {MESG_UPLOAD, upload_handler},
};



/*
 *browser
 *
*/
TRESULT_CODE browser_handler(TRequestParam *req, TResponseParam *resp)
{


}


/*
 *download
 *
*/
TRESULT_CODE download_handler(TRequestParam *req, TResponseParam *resp)
{


}

/*
 *upload
 *
*/
TRESULT_CODE upload_handler(TRequestParam *req, TResponseParam *resp)
{


}

int compact_request(TClient *client)
{
    char buffer[512] = {0};
    size_t buf_len = sizeof(buffer);
    int total = 0, len = 0;

    /*header*/
    do {
        len = recv(client->socket, buffer + total, buf_len - total, 0);
        if (len > 0)
        {
            total += len;
            if (total == sizeof(client->req.header))
            {
                memcpy(&(client->req.header), buffer, len);
            }
            else
            {
                LOG_MESG(EERROR, "Invalid request header size, len:%d, header:%d\n", len, sizeof(client->req.header));
                return -1;
            }
        }
        else if (len <= 0 && errno != EINTR)
        {
            LOG_MESG(EERROR, "Receive header failed:%s.\n", strerror(errno));
            return -1;
        }
    }
    while(len < 0 && errno == EINTR);

    /*body*/
    int body_len = client->req.header.body_len;
    if (body_len > 0)
    {
        total = 0;

        do {
            memset(buffer, 0, buf_len);
            len = recv(client->socket, buffer, buf_len, 0);
            if (len > 0)
            {}
            else if (len == 0)
            {
                LOG_MESG(EERROR, "Connection break unexpectedly:%s.\n", strerror(errno));
                /*process clean up free */
                return -1;
            }
            else
            {
                if (errno == EINTR)
                {
                    continue;
                }
            }

        }
        while(total < body_len);
    }

    return 0;
}

int response_back(const TClient *client)
{
    /*send back to client*/

    return 0;
}

TRESULT_CODE dispatch_command(TRequestParam *req, TResponseParam *resp)
{
    int i = 0;
    TRESULT_CODE rc = RCODE_ERROR;
    for(i = 0; i < sizeof(methods)/sizeof(methods[0]); ++i)
    {
        if (req->header.msg_id == methods[i].mesg_id)
        {
            rc = (methods[i].handler)(req, resp);

            /*response to client*/
            return rc;
        }
    }

    return rc;
}



