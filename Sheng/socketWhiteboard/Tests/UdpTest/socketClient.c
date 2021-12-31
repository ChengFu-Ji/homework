#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netdb.h>
#include <unistd.h> //close, read, write
#include <errno.h> //errno
#include <poll.h>
#include <time.h>

#define MAX_P 10 

int clientTest (int);
void *recvData (void *sockfd);


int main() {
    int clientSockfd, len;
    int port = 2244, i = 1;
    char test;
    struct hostent *host;
    struct sockaddr_in server;

    clientSockfd = clientTest(port);
    len = sizeof(server);

    host = gethostbyname("localhost");
    bzero(&server, len);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy(&server.sin_addr, host->h_addr_list[0], host->h_length);

    test = getchar();
    sendto(clientSockfd, &test, 1, 0, (struct sockaddr *) &server,(socklen_t) len);

    while (i) {
        recvfrom(clientSockfd, &test, 1, 0, (struct sockaddr *) &server,(socklen_t *) &len);
        printf("test %c\n", test);
    }
    close(clientSockfd);
    return 0;
}

int clientTest (int port) {
    int fd, len;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    return fd;
}
