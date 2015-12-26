/*
 * File Transfer tools project
 *
 *
 * File Transfer tools
 * Copyright Reserved  Yangeagle
 *
 * This file is part of File Transfer tools.
 *
 * File Transfer tools is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "log.h"
#include "config.h"
#include "common_utils.h"
#include "clients.h"

#define CHECK_INTERVAL  (30)


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

    /*read config from file*/
    read_config_file();
}



void reset_fds(int listen_fd, const TClient *first, fd_set *preadset, int *pmax_fd)
{
    int i = 0;
    FD_ZERO(preadset);

    FD_SET(listen_fd, preadset);
    *pmax_fd = listen_fd;

    TClient *tmp = first;

    do {
        if (tmp->socket >= 0)
        {
            FD_SET(tmp->socket, preadset);

            if (*pmax_fd < tmp->socket)
            {
                *pmax_fd = tmp->socket;
            }
        }

        tmp = tmp->entries.next;
    }while(tmp != first);
}

/*
 *delete unuse fd from clients sets
 *
 *
*/
int delete_fds(int fd)
{
    int i = 0;

    close(fd);

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

/*
 * ===============main===============
 *
*/
int main(int argc, char *arg[])
{
    int i, nready, max_fd;
    int listen_fd;
    fd_set readset;
    struct timeval timeout;

    LINK_LIST_HEAD(client_capsule) clients_head;
    LIST_INIT(&clients_head);

    config_init(argc, arg);

    listen_fd = config_server_socket();
    if (listen_fd < 0)
    {
        LOG_MESG(EFATAl, "Create server socket error.\n");
        return -1;
    }

    LOG_MESG(EGENERAL,"max fd num %d\n", FD_SETSIZE);
    LOG_MESG(EGENERAL,"File transfer tool starting...\n");

    while (!quit_flag) {

        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        reset_fds(listen_fd, clients_head.l_first, &readset, &max_fd);

        nready = select(max_fd + 1, &readset, NULL, NULL, &timeout);
        if (nready < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            LOG_MESG(EGENERAL, "select error: %s\n", strerror(errno));
        }
        else if (nready == 0)
        {
            LOG_MESG(EGENERAL, "select timeout.\n");
        }
        else if (nready > 0)
        {
            process_request(listen_fd, clients_head.l_first, &readset);
        }

        /*Check time out*/
        CheckActivity(&(clients_head.l_first), CHECK_INTERVAL);
    }

    DeleteClients(&(clients_head.l_first));
    cleanup();
    exit(EXIT_SUCCESS);
    return 0;
}

int process_request(int listen_fd, TClient *first, fd_set *fdset)
{
    char recv_buf[1024];
    char send_buf[1024];
    int i, len;
    socklen_t client_size;
    struct sockaddr_in addr_client;

    /*listen fd*/
    if (FD_ISSET(listen_fd, fdset)) {
        int conn_fd = accept(listen_fd, (struct sockaddr *)&addr_client, &client_size);
        if (conn_fd < 0) {
            LOG_MESG(EERROR, "Accept failed");
        }
        else
        {
            LOG_MESG(EGENERAL, "Connection from client:%s port:%d.\n", inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port));

            AddNewClient(&first, conn_fd);
        }
    }

    if (first)
    {
        TClient *tmp = first;
        do {

            if (FD_ISSET(tmp->socket, fdset))
            {

            }

            tmp = tmp->entries.next;
        }while(tmp != first);

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

void cleanup()
{

    log_close();
}

