/*
*File Transfer tools
* Author:ZYY
* date:2015-11-03
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.h"

#define MAX_LISTEN 5
#define SERVER_PORT (2618)

#define LOGFLAG "LOGFLAG"
#define LOGDIR  "LOGDIR"
#define ROOTDIR "ROOTDIR"

int config_server_socket()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        LOG_MESG(EERROR, "create socket failed.\n");
        return -1;
    }

    struct sockaddr_in addr_serv;
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(SERVER_PORT);
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(struct sockaddr_in)) < 0) {
        LOG_MESG(EERROR, "bind failed.\n");
        return -1;
    }

    if (listen(sock_fd, MAX_LISTEN) < 0) {
         LOG_MESG(EERROR, "listen failed\n");
        return -1;
    }

    return sock_fd;
}


void read_config_file()
{
    const char *optionfile = "/etc/fft.conf";
    const char *logfile = "fft.log";

    char buf[512] = {0};
    char *strptr = NULL;

    FILE *fp = NULL;
    fp = fopen(optionfile, "r");
    if (!fp)
    {
        LOG_MESG(EERROR, "Open option file failed.\n");
    }

    while(fgets(buf, sizeof(buf), fp))
    {

        if (!strstr(buf, LOGFLAG))
        {

        }
        else if (!strstr(buf, LOGDIR))
        {}
        else if (!strstr(buf, ROOTDIR))
        {

        }
        else
        {
            LOG_MESG(EWARN, "unkown config option.\n");
        }
    }



}



