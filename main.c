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

int add_new_fd(int fd)
{
    if (fd < 0)
    {
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

    return -1;
}


void set_fds(fd_set *preadset, int *pmax_fd)
{
    int i = 0;
    *pmax_fd = -1;
    FD_ZERO(preadset);

    for (i = 0; i < MAX_CLIENTS_NUM; i++) {
        if (clients[i] >= 0) {
            FD_SET(clients[i], preadset);
            if (*pmax_fd < clients[i])
            {
                *pmax_fd = clients[i];
            }
        }
    }
}



int main(int argc, char *arg[])
{
    int i, nready, max_fd;

    for (i = 0; i < MAX_CLIENTS_NUM; i++) {
        clients[i] = -1;
    }

    config_init(argc, arg);

    fd_set readset;
    int sock_fd = confg_socket_create();
    max_fd = sock_fd;

    LOG_MESG(EGENERAL,"max fd num %d\n", FD_SETSIZE);
    LOG_MESG(EGENERAL,"File transfer tool starting...");

    while (!quit_flag) {

        set_fds(&readset, &max_fd);

        nready = select(max_fd+1, &readset, NULL, NULL, NULL);
        if (nready < 0)
        {
            LOG_MESG(EGENERAL, "select error\n");
        }


    }

    exit(EXIT_SUCCESS);
    return 0;
}

void process_request(fd_set *fdset)
{
    char recv_buf[1024];
    char send_buf[1024];
    int send_num, recv_num;

    if (FD_ISSET(listen_fd, fdset)) {
        conn_fd = accept(listen_fd, (struct sockaddr *)&addr_client, &client_size);
        if (conn_fd < 0) {
            perror("accept failed");
            exit(1);
        }

        FD_SET(conn_fd, fdset);
        FD_CLR(sock_fd, fdset);

        if (conn_fd > max_fd) {
            max_fd = conn_fd;
        }
        client[conn_fd] = 1;
    }

    //检查所有的描述符，查看可读的是哪个，针对它进行IO读写
    for (i = 0; i < FD_SETSIZE; i ++) {
        if (FD_ISSET(i, &readset)) {

            recv_num = recv(i, recv_buf, sizeof(recv_buf), 0);
            if (recv_num <= 0) {
                FD_CLR(i, &readset);
                client[i] = -1;
            }
            recv_buf[recv_num] = '\0';
            memset(send_buf,0,sizeof(send_buf));
            sprintf(send_buf, "server proc got %d bytes\n", recv_num);
            send_num = send(i, send_buf, strlen(send_buf), 0);
            if (send_num <= 0) {
                FD_CLR(i, &readset);
                client[i] = -1;
            }
        }
    }

}

