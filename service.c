/*
*File Transfer tools
* Author:ZYY
*
*
*/

#include "service.h"

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



TRESULT_CODE dispatch_command(TRequestParam *req, TResponseParam *resp)
{
    int i = 0;
    TRESULT_CODE rc = RCODE_ERROR;
    for(i = 0; i < sizeof(methods)/sizeof(methods[0]); ++i)
    {
        if (req->msg_id == methods[i].mesg_id)
        {
            rc = (methods[i].handler)(req, resp);
            return rc;
        }
    }

    return RCODE_ERROR;
}

