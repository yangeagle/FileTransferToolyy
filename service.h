#ifndef SERVICE_H
#define SERVICE_H

#define  MAX_MESG_LEN   (512)



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


typedef struct Capsule
{
    int len;
    void *content;

}TCapsule;

typedef struct RequestParameter
{
    TMESG_ID msg_id;
    int len;
    void *body;
}TRequestParam;

typedef struct ResponseParameter
{
    TRESULT_CODE rc;
    int len;
    void *body;
}TResponseParam;


typedef TRESULT_CODE (*ServiceHandler)(TRequestParam *req, TResponseParam *resp);

typedef struct
{
    TMESG_ID mesg_id;
    ServiceHandler handler;
}TMesgHandlers;



#endif // SERVICE_H
