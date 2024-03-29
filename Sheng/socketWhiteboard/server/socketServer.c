#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> //close, read, write
#include <errno.h> //errno
#include <poll.h>

#include "linkedlist.h"

#define MAX_P 10

int serverInit (int port);

int main() {
    int serverSockfd, clientSockfd, clientlen;
    struct sockaddr_in client;
    struct pollfd clients[MAX_P+1];

    int port = 2244;
    if ((serverSockfd = serverInit(port)) < 0) {
        printf("ERROR: Fail to listen on port %d\nExiting...\n", port);
        exit(1);
    }
    
    clients[0].fd = serverSockfd;
    clients[0].events = POLLRDNORM;
    for (int i = 1; i <= MAX_P; i++) {
        clients[i].fd = -1;
    }

    int nready = 0;
    int maxi = 1;
    int refuse = 0;

    while (1) {
        nready = poll(clients, maxi, -1);

        if (clients[0].revents & POLLRDNORM) {
            clientlen = sizeof(client);
            clientSockfd = accept(serverSockfd, (struct sockaddr *) &client, (socklen_t *) &clientlen);
            int i = 0;
            for (i = 1; i < MAX_P; i++) {
                if (refuse) {
                    close(clientSockfd);
                    break;
                } else if (clients[i].fd < 0) {
                    clients[i].fd = clientSockfd;
                    clients[i].events = POLLRDNORM;
                    printf("added %d\n", i);
                    break;
                }
            }

            if (i > MAX_P) {
                printf("There are too many people here, refused...\n");
                refuse = 1;
            } else if (i == maxi) {
                maxi++;
                printf("i = %d, maxi = %d\n", i, maxi);
            }

            if (--nready <= 0) {
                continue;
            }
        }

        for (int i = 1; i <= maxi; i++) {
            if (clients[i].fd < 0) {
                continue;
            }

            clientSockfd = clients[i].fd;
            if (clients[i].revents & (POLLRDNORM | POLLERR)) {
                //Pos tmp;
                int n, len;

                if ((n = read(clientSockfd, &len, sizeof(int))) > 0) {
                    Pos tmp[len];
                    int n2 = read(clientSockfd, &tmp, sizeof(Pos)*len);

                    printf("n = %lu, n2 = %d, len %d\n", len*sizeof(Pos), n2, len);
                    for (int j = 1; j <= maxi; j++) {
                        if (clients[j].fd != clientSockfd && clients[j].fd != -1) {
                            write(clients[j].fd, &len, sizeof(int));
                            write(clients[j].fd, &tmp, len*sizeof(Pos));
                        }
                    }
                } else {
                    if (n == 0 || errno == ECONNRESET) {
                        printf("Exit: User[%d]\n", i);
                        close(clientSockfd);
                        clients[i].fd = -1;
                        refuse = 0;
                    }
                } 
            }
            if (--nready <= 0) {
                continue;
            }
        }
    }
    return 0;
}

int serverInit (int port) {
    int fd, len;
    struct sockaddr_in server;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    len = sizeof(server);

    bzero(&server, len);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if ((bind(fd, (struct sockaddr *) &server, len)) < 0 || (listen(fd, 5)) < 0) {
        return -1;
    }

    return fd;
}

