#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <opencv2/opencv.hpp>

#include "bezier.h"
extern "C" {
#include "linkedlist.h"
}

#define TIMES 1000
#define DOTS 6

using namespace cv;

int clientInit (int, char *);
Node_s **linkedlistInit ();
void linkedlistFree (Node_s **);
void plotHandwriting (Point2d *, Point2d *, int);
void *recvData (void *);
void MouseWork (int, int, int, int, void *);
void button(int, void*);
void trackBar(int, void*);
Pos toPos(int, int);

char windowName[] = "whiteboard";
Mat image;
Node_s **recvList, **sendList;
int pass = 0;

int main () {
    pthread_t t;
    int fd;

    image = Mat(900, 1600, CV_8UC3, Scalar(255, 255, 255));

    int port = 2244;
    char address[] = "localhost";

    sendList = linkedlistInit();
    recvList = linkedlistInit();

    namedWindow(windowName, WINDOW_NORMAL | WINDOW_FREERATIO | WINDOW_GUI_NORMAL);
    imshow(windowName, image);

    setMouseCallback(windowName, MouseWork, (void *) &fd);

    if ((fd = clientInit(port, address)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", address, port);
    } else {
        pthread_create(&t, NULL, recvData, (void *) &fd);
    }

    int id = 0;
    while (waitKey(100) != 27) {
        //printf("id %d, %lf, fd %d, alive %d\n", id++, getWindowProperty(windowName, WND_PROP_VISIBLE), fd, pthread_kill(t, 0));
        if (getWindowProperty(windowName, WND_PROP_VISIBLE) <= 0) {
            break;
        }
        if (fd > 0) {
            if (pthread_kill(t, 0) == ESRCH) {
                printf("in\n");
                pthread_create(&t, NULL, recvData, (void *) &fd);
                imshow(windowName, image);
            }
        }
    }

    close(fd);
    linkedlistFree(recvList);
    linkedlistFree(sendList);
    printf("Exiting...\n");
    if (fd > 0) {
        if (pthread_kill(t, 0) != ESRCH) {
            pthread_cancel(t);
        }
    }
}

void button(int status, void *data) {
    printf("clicked !! \n");
    pass = 1;
}

void trackBar(int pos, void *data) {
    printf("pos %d\n", pos);
}

Node_s **linkedlistInit () {
    Node_s **list;
    list = (Node_s **)malloc(sizeof(Node_s *));
    *list = (Node_s *)malloc(sizeof(Node_s));
    (*list)->next = NULL;
    return list;
}

void linkedlistFree (Node_s **list) {
    free(*list);
    free(list);
}

int clientInit (int port, char *address) {
    int fd;
    struct sockaddr_in mAddr;
    struct hostent *host;

    if ((host = gethostbyname(address)) == NULL) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&mAddr, 0, sizeof(mAddr));
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(port);
    memcpy(&mAddr.sin_addr, host->h_addr_list[0], host->h_length);

    if (connect(fd, (struct sockaddr *) &mAddr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }

    return fd;
}

void *recvData(void *fd) {
    Point2d p[DOTS], plot[TIMES];
    Node_s *cur, *next;
    int sockfd = *(int *) fd, i = 0, j = 0;
    int id, size, n;
    double distance = 0.0;

    if (read(sockfd, &size, sizeof(int)) < 0) {
        pthread_exit(NULL);
    }

    if ((n = socket_read(recvList, sockfd, size)) > 0) {
        printf("size %d, n %d\n", size, n);
        for (i = 0; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
        cur = (*recvList)->next;
        while (cur != NULL) {
            //printf("cur %d %d, next %p\n", cur->point.x, cur->point.y, cur->next);
            if (cur->point.x != -1 && cur->point.y != 0) {
                for(i = 0; i < DOTS-1; i++) {
                    if (p[i].x == -1 && p[i].y == -1) {
                        p[i] = Point(cur->point.x, cur->point.y);
                        cur = cur->next;
                        break;
                    }
                }
                if (i != DOTS-1) {
                    continue;
                }
                p[DOTS-1] = Point(cur->point.x, cur->point.y);
                bezierCurve(plot, TIMES, p, DOTS);
                plotHandwriting(p, plot, 0);
                //imshow(windowName, image);
            } else {
                break;
            }
            cur = cur->next;
        }

        plotHandwriting(p, plot, 1);
        //IDdelete(recvList, id);
        showList(recvList);
        cleanList(recvList);
    }
    printf("exit\n");
    pthread_exit(0);
}

void MouseWork (int event, int x, int y, int flags, void *userdata) {
    static Pos tmp;
    static Point2d p[DOTS];
    static int dots = 0;
    Point2d plot[TIMES] ;
    int fd = *(int *) userdata, i = 0, j = 0;
    double distance = 0.0;

    if (event == EVENT_LBUTTONDOWN) {
        p[0] = Point(x, y);

        if (fd > 0) {
            dots++;
            add(sendList, toPos(x, y));
        }
        for (i = 1; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) && p[0].x != -1 && p[0].y != -1) {
        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = Point(x, y);
                if (fd > 0) {
                    dots++;
                    add(sendList, toPos(x, y));
                }
                return;
            }
        }

        p[DOTS-1] = Point(x, y);
        if (fd > 0) {
            dots++;
            add(sendList, toPos(x, y));
        }

        bezierCurve(plot, TIMES, p, DOTS);
        plotHandwriting(p, plot, 0);
        /*
        for(int x = 0; x < DOTS;  x++) {
            printf("drawin p %lf, %lf\n", p[i].x, p[i].y);
        }
        */
        imshow(windowName, image);
    } else if (event == EVENT_LBUTTONUP) {

        if (fd > 0) {
            dots++;
            add(sendList, toPos(-1, 0));
            socket_write(sendList, fd, dots);
            showList(sendList);
            cleanList(sendList);
            dots = 0;
        }
        plotHandwriting(p, plot, 1);
        imshow(windowName, image);
    }
}

void plotHandwriting (Point2d *p, Point2d *plot, int repair) {
    int i, j;
    double distance = 0.0;
    Scalar handwritColor = Scalar(17, 10, 120);
    int thk = 6;

    if (!repair) {
        for (i = 0; i < DOTS-1; i++) {
            distance += dist(p[i], p[i+1]);
        }
        //printf("distance %lf\n", distance);

        if (distance <= 70) {
            for (i = 0; i < DOTS-1; i++) {
                line(image, p[i], p[i+1], handwritColor, thk);
            }
            p[0] = p[DOTS-1];
            i = 1;
        } else {
            /*
            printf("dist(plot[i], p[DOTS-1]) %lf\n", dist(plot[i], p[DOTS-1]));
            printf("plot[i] %lf %lf\n", plot[i].x, plot[i].y);
            printf("p[DOTS-1] %lf %lf\n", p[DOTS-1].x, plot[i].y);
            */
            //30 67.082 36
            for (i = 0; dist(plot[i], p[DOTS-1]) > 30 && i < TIMES-3; i++) {
                line(image, plot[i], plot[i+1], handwritColor, thk);
                //printf("dot %lf, %lf\n", plot[i].x, plot[i].y);
            }
            //21.2 47.434 25.46
            for (j = i; dist(plot[j], p[DOTS-1]) > 21.2 && j < TIMES-2; j++);
            //printf("i and j %d, %d\n", i, j);
            p[0] = plot[i];
            p[1] = plot[j];
            p[2] = p[DOTS-1];
            i = 3;
            //printf("%lf %lf\n", p[0].x, p[0].y);
            
            /*
            for (int i = 0; i < DOTS-1; i++) {
                printf("\n");
                printf("dones points [%lf, %lf]\n", p[i].x, p[i].y);
            }
            */
        }
    } else {
        for (i = DOTS; i >= 3; i--) {
            //printf("p[i-1] %lf %lf\n",p[i-1].x, p[i-1].y);
            if (p[i-1].x != -1 && p[i-1].y != -1) {
                bezierCurve(plot, TIMES, p, i);
                for (j = 0; j < TIMES-1; j++) {
                    line(image, plot[j], plot[j+1], handwritColor, thk);
                }
                line(image, plot[TIMES-1], p[i-1], handwritColor, thk);
                break;
            }
        }

        if (p[1].x != -1 && p[1].y != -1) {
            line(image, p[0], p[1], handwritColor, thk); 
        }
        i = 0;
        //printf("in\n");
    }

    for (; i < DOTS; i++) {
        p[i] = Point(-1, -1);
    }
}

Pos toPos(int x, int y) {
    Pos tmp;
    tmp.x = x;
    tmp.y = y;
    return tmp;
}

