#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <unistd.h> //close, read, write
#include <errno.h> //errno
#include <poll.h>
#include <time.h>
#include <arpa/inet.h>

int serverInit (int);

int main() {
    int serverSockfd, client_len;
    struct sockaddr_in clients[10];

    char test;

    memset(clients, 0, sizeof(clients));

    int port = 2244;
    if ((serverSockfd = serverInit(port)) < 0) {
        printf("ERROR: Fail to bind on port %d\nExiting...\n", port);
        exit(1);
    }
    
    printf("waiting...\n");
    int i = 0;
    while (1) {
        client_len = sizeof(clients[i]);
        recvfrom(serverSockfd, &test, 1, 0, (struct sockaddr *) &clients[i],(socklen_t *)  &client_len);
        printf("now address %s, port %d\n", inet_ntoa(clients[i].sin_addr), clients[i].sin_port);
        printf("Test %c\n", test);
        test++;

        //確認 IP 以及 Port 有無重複
        for (int j = 0; j < i; j++) {
            if (!memcmp(&clients[i].sin_addr, &clients[j].sin_addr, sizeof(clients[i].sin_addr)) &&
                    clients[j].sin_port == clients[i].sin_port) {

                printf("isTheSame\n");
                i--;
                break;
            }
        }

        // broadcast 給其他使用者。
        i++;
        for (int j = 0; j < i; j++) {
            printf("address %s, port %d\n", inet_ntoa(clients[j].sin_addr), clients[j].sin_port);
            sendto(serverSockfd, &test, 1, 0, (struct sockaddr *) &clients[j],(socklen_t) client_len);
        }
    }

    close(serverSockfd);
    return 0;
}

int serverInit (int port) {
    int fd, len;
    int setup = 1;
    struct sockaddr_in server;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    //setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &setup, sizeof(int));
    len = sizeof(server);

    bzero(&server, len);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if ((bind(fd, (struct sockaddr *) &server, len)) < 0) {
        return -1;
    }

    return fd;
}

