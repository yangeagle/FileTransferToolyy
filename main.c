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
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "config_socket.h"
#include "log.h"

#define MAX_CLIENTS_NUM (1000)

int clients[MAX_CLIENTS_NUM];

int quit_flag = 0;

/* Handler for the SIGTERM signal (kill)
 * SIGINT is also handled */
static void
sigterm(int sig)
{
    signal(sig, SIG_IGN);	/* Ignore this signal while we are quitting */

    LOG_MESG(EGENERAL, "received signal %d, good-bye\n", sig);

    quit_flag = 1;
}


void config_init(int argc, char *arg[])
{
    /*Set log flag*/
    log_flag = 1;


    struct sigaction sa;

    /*set signal handler*/
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sigterm;
    if (sigaction(SIGTERM, &sa, NULL))
    {
        LOG_MESG(EGENERAL, "Failed to set %s handler. EXITING.\n", "SIGTERM");
    }

    if (sigaction(SIGINT, &sa, NULL))
    {
        LOG_MESG(EGENERAL, "Failed to set %s handler. EXITING.\n", "SIGINT");
    }

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        LOG_MESG(EGENERAL, "Failed to set %s handler. EXITING.\n", "SIGPIPE");
    }

//    if (signal(SIGHUP, &sighup) == SIG_ERR)
//    {
//        LOG_MESG(EGENERAL, "Failed to set %s handler. EXITING.\n", "SIGHUP");
//    }

}


/*
 *add new fd info clients sets
 *
 *
*/
int add_new_fd(int fd)
{
    if (fd < 0)
    {
        LOG_MESG(EWARN, "Invalid file descriptor.\n");
        return -1;
    }

    int i = 0;

    for(i = 0; i < MAX_CLIENTS_NUM; ++i)
    {
        if (-1 == clients[i])
        {
            clients[i] = fd;
            return 0;
        }
    }

    LOG_MESG(EWARN, "Too many clients.\n");

    return -1;
}


void reset_fds(int listen_fd, fd_set *preadset, int *pmax_fd)
{
    int i = 0;
    FD_ZERO(preadset);

    FD_SET(listen_fd, preadset);
    *pmax_fd = listen_fd;

    for (i = 0; i < MAX_CLIENTS_NUM; ++i) {
        if (clients[i] >= 0) {
            FD_SET(clients[i], preadset);
            if (*pmax_fd < clients[i])
            {
                *pmax_fd = clients[i];
            }
        }
    }
}

/*
 *delete unuse fd from clients sets
 *
 *
*/
int delete_fds(int fd)
{
    int i = 0;
    for(i = 0; i < MAX_CLIENTS_NUM; ++i)
    {
        if (fd == clients[i])
        {
            clients[i] = -1;
            break;
        }
    }

    if (i == MAX_CLIENTS_NUM)
    {

        LOG_MESG(EWARN, "fd not found\n");
        return -1;
    }

    return 0;
}

/*===============main===============
 *
*/
int main(int argc, char *arg[])
{
    int i, nready, max_fd;
    int listen_fd;
    fd_set readset;

    for (i = 0; i < MAX_CLIENTS_NUM; i++) {
        clients[i] = -1;
    }

    config_init(argc, arg);


    listen_fd = config_server_socket();
    if (listen_fd < 0)
    {
        LOG_MESG(EFATAl, "Create server socket error.\n");
        return -1;
    }


    LOG_MESG(EGENERAL,"max fd num %d\n", FD_SETSIZE);
    LOG_MESG(EGENERAL,"File transfer tool starting...");

    while (!quit_flag) {

        reset_fds(listen_fd, &readset, &max_fd);

        nready = select(max_fd+1, &readset, NULL, NULL, NULL);
        if (nready < 0)
        {
            LOG_MESG(EGENERAL, "select error\n");
        }

        process_request(listen_fd, &readset);
    }

    exit(EXIT_SUCCESS);
    return 0;
}

int  process_request(int listen_fd, fd_set *fdset)
{
    char recv_buf[1024];
    char send_buf[1024];
    int i, len;
    socklen_t client_size;
    struct sockaddr_in addr_client;

    if (FD_ISSET(listen_fd, fdset)) {
        int conn_fd = accept(listen_fd, (struct sockaddr *)&addr_client, &client_size);
        if (conn_fd < 0) {
            LOG_MESG(EERROR, "accept failed");
            return -1;
        }

        LOG_MESG(EGENERAL, "connection from client:%s port:%d.\n", inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port));

        add_new_fd(conn_fd);
    }

    for (i = 0; i < MAX_CLIENTS_NUM; ++i) {
        if (FD_ISSET(clients[i], fdset)) {
            len = recv(clients[i], recv_buf, sizeof(recv_buf) - 1, 0);
            if (len <= 0) {
                clients[i] = -1;
            }

            recv_buf[len] = '\0';
            memset(send_buf,0,sizeof(send_buf));
            sprintf(send_buf, "server proc got %d bytes\n", len);
            len = send(clients[i], send_buf, strlen(send_buf), 0);
            if (len <= 0) {
                clients[i] = -1;
            }
        }
    }

}

