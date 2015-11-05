/*
*File Transfer tools
* Author:ZYY
* date:2015-11-03
*
*
*/

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_LISTEN 5
#define SERVER_PORT (2618)


int confg_socket_create()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("create socket failed");
        return -1;
    }

    struct sockaddr_in addr_serv;
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(SERVER_PORT);
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(struct sockaddr_in)) < 0) {
        perror("bind failed");
        return -1;
    }

    if (listen(sock_fd, MAX_LISTEN) < 0) {
        perror("listen failed");
        return -1;
    }

    return sock_fd;
}
