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

#define MAX_P 12 

void *writefile(void*);
void showTime(char *, int);
void writeCSV(Data_s data);
void CSVinit();

int times = 0;
int main() {
    int serverSockfd, clientSockfd, serverlen, clientlen;
    struct sockaddr_in  server, client;
    struct pollfd clients[MAX_P];

    serverSockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverlen = sizeof(server);

    bzero(&server, serverlen);
    server.sin_family = AF_INET;
    server.sin_port = htons(2244);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(serverSockfd, (struct sockaddr *) &server, sizeof(server));
    listen(serverSockfd, MAX_P);

    clients[0].fd = serverSockfd;
    clients[0].events = POLLRDNORM;
    for (int i = 1; i < MAX_P; i++) {
        clients[i].fd = -1;
    }
    CSVinit();

    int nready = 0;
    int maxi = 1;
    int Scount = 0;
    printf("waiting...\n");

    while (1) {
        nready = poll(clients, maxi, -1);

        if (clients[0].revents & POLLRDNORM) {
            clientlen = sizeof(client);
            clientSockfd = accept(serverSockfd, (struct sockaddr *) &client, (socklen_t *) &clientlen);
            int i = 0;
            for (i = 1; i <= MAX_P; i++) {
                if (clients[i].fd < 0) {
                    clients[i].fd = clientSockfd;
                    clients[i].events = POLLRDNORM;
                    //showTime("Add", i);
                    //write(clientSockfd, &i, sizeof(int));
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

        for (int i = 1; i < maxi; i++) {
            if (clients[i].fd < 0) {
                continue;
            }
            clientSockfd = clients[i].fd;
            if (clients[i].revents & (POLLRDNORM | POLLERR)) {
                char test[100];
                Data_s tmp;
                int n;

                if ((n = read(clientSockfd, &tmp, sizeof(tmp))) > 0) {
                    
                    //strcpy(test, "12345678")
                    //模擬轉發給其他使用者
                    for (int j = 0; j < 9; j++) {
                        write(clientSockfd, &tmp, n);
                    }
                    if (times < 90*101) {
                        writeCSV(tmp);
                    }
                    printf("times %d\n", ++times);

                    /*
                    for (int j = 1; j < maxi; j++) {
                        if ( clients[j].fd != clientSockfd && clients[j].fd != -1) {
                            //int n = sprintf(sendArr, "x: %d, y: %d\n", tmp.x, tmp.y);
                            //write(clients[j].fd, sendArr, n);
                            //write(clients[j].fd, &tmp, n);
                            //send(clients[j].fd, sendArr, n, MSG_DONTROUTE | MSG_DONTWAIT); 
                            write(clients[j].fd, test, k);
                        }
                    }
                    */
                } else {
                    if (n == 0 || errno == ECONNRESET) {
                        //showTime("Exit", i);
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

void showTime(char *status, int id) {
    struct timespec ts;
    size_t t;

    clock_gettime(CLOCK_REALTIME, &ts);

    t = ts.tv_sec;
    for (int i = 0; i < 9; i++) {
        t *= 10;
    }

    //writeCSV(ts.tv_nsec+t, status, id);
    printf("%s, time [%lu ns]\n", status, ts.tv_nsec + t);
}

void writeCSV(Data_s data) {
    FILE *fp;
    static int id = 1;

    fp = fopen("dataLog.csv", "a");
    fprintf(fp, "%d, %d, %d\n", id++, data.x, data.y);
    fclose(fp);
}

void CSVinit() {
    FILE *fp;

    fp = fopen("dataLog.csv", "w");
    fprintf(fp, "id, x, y\n");
    fclose(fp);
}


