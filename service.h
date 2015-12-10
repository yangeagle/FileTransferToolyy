#ifndef SERVICE_H
#define SERVICE_H

#define  MAX_MESG_LEN   (512)



typedef enum MESG_ID
{
    MESG_INVAL = -1,
    MESG_UPLOAD = 0,
    MESG_DOWNLOAD,
    MESG_BROWSER,

    MESG_MAX
}TMESG_ID;


typedef enum RESULT_CODE
{
    RCODE_INVALID = -1,
    RCODE_NORMAL,
    RCODE_ERROR,



    RCODE_MAX
}TRESULT_CODE;


typedef struct RequestParameter
{
    TMESG_ID msg_id;
    int mesg_len;
    char mesg_content[MAX_MESG_LEN];

}TRequestParam;

typedef struct ResponseParameter
{


}TResponseParam;

#endif // SERVICE_H
