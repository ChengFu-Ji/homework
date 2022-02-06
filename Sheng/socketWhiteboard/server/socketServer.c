#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> //close, read, write
#include <errno.h> //errno
#include <poll.h>

#include "linkedlist/page.h"
#include "linkedlist/stroke.h"
#include "linkedlist/pos.h"

#define MAX_P 10

int createServer (int port);
void writeHW(SockCond sender, Pos *pos);
void readHW(SockCond *recv, Pos ***pos);
int HWlen();

int main() {
    int serverSockfd, clientSockfd, clientlen;
    struct sockaddr_in client;
    struct pollfd clients[MAX_P+1];

    int port = 2244;
    if ((serverSockfd = createServer(port)) < 0) {
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

                    int n = HWlen();
                    if (n) {
                        Pos **tmp;
                        SockCond user[n];

                        readHW(user, &tmp);

                        for (int j = 0; j < n; j++) {
                            write(clients[i].fd, &user[j], sizeof(SockCond));
                            if (user[j].poslen) {
                                write(clients[i].fd, tmp[j], sizeof(Pos)*user[j].poslen);
                                free(tmp[j]);
                            }
                        }
                    }
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
                int n;
                SockCond recv;

                if ((n = read(clientSockfd, &recv, sizeof(SockCond))) > 0) {
                    Pos tmp[recv.poslen];
                    if (recv.drawpid != -1 && recv.poslen != 0) {
                        read(clientSockfd, &tmp, sizeof(Pos)*recv.poslen);
                        writeHW(recv, tmp);
                    } else {
                        writeHW(recv, NULL);
                    }
                    
                    /*
                    printf("draw %d ", recv.drawpid);
                    printf("add %d del ", recv.addpid);
                    printf("%d eraser %d thk %d len %d\n", recv.deletepid, recv.eraser, recv.thickness, recv.poslen);
                    */
                    for (int j = 1; j <= maxi; j++) {
                        if (clients[j].fd != clientSockfd && clients[j].fd != -1) {
                            write(clients[j].fd, &recv, sizeof(SockCond));
                            if (recv.poslen) {
                                write(clients[j].fd, &tmp, sizeof(Pos)*recv.poslen);
                            }
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

int createServer (int port) {
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

void writeHW(SockCond sender, Pos *pos) {
    FILE *fp;
    int userlen, filelen;

    if (pos != NULL) {
        fp = fopen("serverHW.log", "a");
        fwrite(pos, sizeof(Pos), sender.poslen, fp);
        fclose(fp);
    }

    fp = fopen("serverHW.index", "a");
    //fwrite(&userlen, sizeof(int), 1, fp);
    //fwrite(&uc.dotslen, sizeof(int), 1, fp);
    fwrite(&sender, sizeof(SockCond), 1, fp);
    fclose(fp);
}


void readHW(SockCond *recv, Pos ***pos) {
    FILE *fp, *index;
    int userlen, filelen, len;

    len = HWlen();
    index = fopen("serverHW.index", "r");
    fp = fopen("serverHW.log", "r");
    *pos = (Pos **)malloc(sizeof(Pos *)*len);
    for (int i = 0; i < len; i++) {
        fread(&recv[i], sizeof(SockCond), 1, index);
        if (recv[i].poslen != 0) {
            *(*(pos)+i) = (Pos *)malloc(sizeof(Pos)*recv[i].poslen);
            fread(*(*(pos)+i), sizeof(Pos), recv[i].poslen, fp);
        }
    /*
        printf("%d %d %d %d %d %d\n", recv[i].drawpid, recv[i].addpid, recv[i].deletepid, recv[i].eraser, recv[i].thickness, recv[i].poslen);
        printf("len %d\n", uc[i].dotslen);
        for (int j = 0; j < uc[i].dotslen; j++) {
            printf("%d %d\n", pos[i][j].x, pos[i][j].y);
        }
    */
    }
    fclose(fp);

}

int HWlen() {
    FILE *fp;

    if (!(fp = fopen("serverHW.index", "r"))) {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
    if (!len) {
        return 0;
    }

    return len/(sizeof(int)*6);
}
