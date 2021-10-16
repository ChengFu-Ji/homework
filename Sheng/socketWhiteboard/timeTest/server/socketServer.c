#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> //close, read, write
#include <errno.h> //errno
#include <poll.h>
#include <time.h>

#include "linkedlist.h"

#define MAX_P 10
void showTime(char *status, struct timespec);

int main() {
    int serverSockfd, clientSockfd, serverlen, clientlen;
    struct sockaddr_in  server, client;
    struct pollfd clients[MAX_P];
    Node_s **head;

    head = (Node_s **)malloc(sizeof(Node_s *));
    *head = (Node_s *)malloc(sizeof(Node_s));

    serverSockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverlen = sizeof(server);

    bzero(&server, serverlen);
    server.sin_family = AF_INET;
    server.sin_port = htons(2244);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(serverSockfd, (struct sockaddr *) &server, sizeof(server));
    listen(serverSockfd, 5);

    clients[0].fd = serverSockfd;
    clients[0].events = POLLRDNORM;
    for (int i = 1; i < MAX_P; i++) {
        clients[i].fd = -1;
    }
    int nready = 0;
    int maxi = 1;
    while (1) {
        printf("waiting...\n");
        nready = poll(clients, maxi, -1);
        printf("status: polled\n");

        if (clients[0].revents & POLLRDNORM) {
            clientlen = sizeof(client);
            clientSockfd = accept(serverSockfd, (struct sockaddr *) &client, (socklen_t *) &clientlen);
            int i = 0;
            for (i = 1; i < MAX_P; i++) {
                if (clients[i].fd < 0) {
                    clients[i].fd = clientSockfd;
                    clients[i].events = POLLRDNORM;
                    printf("Add client in %d\n", i);
                    break;
                }
            }
            if (i == MAX_P) {
                printf("There are too many people here, Exiting...\n");
                exit(1);
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
                Data_s tmp;
                int n;

                if ((n = read(clientSockfd, &tmp, sizeof(tmp))) > 0) {
                    if (tmp.x == -1) {
                        showTime("After Read ");
                    }
                    if (tmp.x == -1 && tmp.y == -1) {
                        printf("Exit: User[%d]\n", i);
                        close(clientSockfd);
                        clients[i].fd = -1;
                        continue;
                    }
                    for (int j = 1; j <= maxi; j++) {
                        if (clients[j].fd != clientSockfd && clients[j].fd != -1) {
                            //int n = sprintf(sendArr, "x: %d, y: %d\n", tmp.x, tmp.y);
                            //write(clients[j].fd, sendArr, n);
                            write(clients[j].fd, &tmp, n);
                        }
                    }
                } else {
                    if (n == 0 || errno == ECONNRESET) {
                        printf("Exit: User[%d]\n", i);
                        close(clientSockfd);
                        clients[i].fd = -1;
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

void showTime(char *status) {
    static struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    
    printf("%s, time [%ld]\n", status, ts.tv_nsec);
}
