#ifndef SERVICE_H
#define SERVICE_H

#define  MAX_MESG_LEN   (1024)



typedef enum MESG_ID
{
    MESG_INVAL = -1,
    MESG_BROWSER = 0,
    MESG_DOWNLOAD,
    MESG_UPLOAD,


    MESG_MAX
}TMESG_ID;


typedef enum RESULT_CODE
{
    RCODE_INVALID = -1,
    RCODE_NORMAL,
    RCODE_ERROR,


    RCODE_MAX
}TRESULT_CODE;

struct req_header
{
    TMESG_ID msg_id;
    int body_len;
};

typedef struct RequestParameter
{
    struct req_header header;
    void *body;
}TRequestParam;


struct resp_header
{
    TMESG_ID msg_id;
    TRESULT_CODE rc;
    int body_len;
};

typedef struct ResponseParameter
{
    struct resp_header header;
    void *body;
}TResponseParam;


typedef TRESULT_CODE (*ServiceHandler)(TRequestParam *req, TResponseParam *resp);

typedef struct
{
    TMESG_ID mesg_id;
    ServiceHandler handler;
}TMesgHandlers;



int compact_request_v2(TClient *client);


#endif // SERVICE_H
