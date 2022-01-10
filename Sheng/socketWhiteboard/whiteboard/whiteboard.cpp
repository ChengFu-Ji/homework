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

extern "C" {
#include "linkedlist.h"
}

#define TIMES 1000
#define DOTS 6

using namespace cv;

int clientInit (int, char *);
Node_p **linkedlistInit ();
void linkedlistFree (Node_p **);
int factorial(int);
double dist (Point2d, Point2d);
double bezierPoly (double, int, int); 
void triangleDist (Point2d *, double *);
void bezierCurve (Point2d *, int, Point2d *, int);
void plotHandwriting (Point2d *, Point2d *, int);
void *recvData (void *);
void MouseWork (int, int, int, int, void *);

char windowName[] = "whiteboard";
char imageName[] = "background.png";
Mat image;
Node_p **recvList, **sendList;
int lastIds[MAX_P] = {0};
Point2d lastPos[MAX_P][DOTS];

int main () {
    pthread_t t;
    int fd;

    image = Mat(900, 1600, CV_8UC3, Scalar(255, 255, 255));

    int port = 2244;
    char address[] = "localhost";

    sendList = linkedlistInit();
    recvList = linkedlistInit();

    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);

    setMouseCallback(windowName, MouseWork, (void *) &fd);

    if ((fd = clientInit(port, address)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", address, port);
    } else {
        pthread_create(&t, NULL, recvData, (void *) &fd);
    }

    while (1) {
        if (waitKey(100) == 27) {
            close(fd);
            linkedlistFree(recvList);
            linkedlistFree(sendList);
            printf("Exiting...\n");
            break;
        }
        if (fd > 0) {
            if (pthread_kill(t, 0) == ESRCH) {
                pthread_create(&t, NULL, recvData, (void *) &fd);
                imshow(windowName, image);
            }
        }
    }
    if (fd > 0) {
        if (pthread_kill(t, 0) != ESRCH) {
            pthread_cancel(t);
        }
    }
}

Node_p **linkedlistInit () {
    Node_p **list;
    list = (Node_p **)malloc(sizeof(Node_p *));
    *list = (Node_p *)malloc(sizeof(Node_p));
    (*list)->next = NULL;
    return list;
}

void linkedlistFree (Node_p **list) {
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
    Pos tmp;
    Point2d p[DOTS], plot[TIMES];
    Node_p *cur, *next;
    int sockfd = *(int *) fd, i = 0, j = 0;
    int id, size;
    double distance = 0.0;

    if (read(sockfd, &tmp, sizeof(Pos)) < 0) {
        pthread_exit(NULL);
    }
    size = tmp.x;
    id = tmp.y;

    int n;
    if ((n = socket_read(sockfd, id, recvList, size))) {
        cur = (*recvList);
        while (cur->next != NULL) {
            cur = cur->next;
        }

        for (i = 0; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }

        if (lastIds[cur->id-1]) {
            for (i = 0; i < DOTS; i++) {
                p[i] = lastPos[cur->id-1][i];
            }
            lastIds[cur->id-1]--;
        }


        for (i = 0; i < cur->len; i++) {
            if (cur->p[i].x == -1 && cur->p[i].y == 0) {
                break;
            }
            for (j = 0; j < DOTS-1; j++) {
                if (p[j].x == -1 && p[j].y == -1) {
                    p[j] = Point(cur->p[i].x, cur->p[i].y);
                    break;
                }
            }
            if (j != DOTS-1) {
                continue;
            }
            if (cur->p[i].x == -1 && cur->p[i].y == 0) {
                break;
            }
            p[DOTS-1] = Point(cur->p[i].x, cur->p[i].y);

            bezierCurve(plot, TIMES, p, DOTS);
            plotHandwriting(p, plot, 0);
        }
        if (cur->p[cur->len-1].x != -1 && cur->p[cur->len-1].y != 0) {
            lastIds[cur->id-1]++;
            for (i = 0; i < DOTS; i++) {
                lastPos[cur->id-1][i] = p[i]; 
            }
        } else {
            plotHandwriting(p, plot, 1);
        }
        //IDdelete(recvList, id);
        //cleanList(recvList);
    }
    pthread_exit(0);
}

void MouseWork (int event, int x, int y, int flags, void *userdata) {
    static Pos tmp[100];
    static Point2d p[DOTS];
    static int dots = 0;
    Point2d plot[TIMES] ;
    int fd = *(int *) userdata, i = 0, j = 0;
    double distance = 0.0;

    if (event == EVENT_LBUTTONDOWN) {
        p[0] = Point(x, y);

        if (fd > 0) {
            tmp[dots].x = x;
            tmp[dots++].y = y;
        }
        for (i = 1; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) && p[0].x != -1 && p[0].y != -1) {
        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = Point(x, y);
                if (fd > 0) {
                    tmp[dots].x = x;
                    tmp[dots++].y = y;
                    if (dots == 100) {
                        add(sendList, 0, tmp, sizeof(Pos)*dots);
                        socket_write(fd, 0, sendList);
                        cleanList(sendList);
                        dots = 0;
                    }
                }
                return;
            }
        }

        p[DOTS-1] = Point(x, y);
        if (fd > 0) {
            tmp[dots].x = x;
            tmp[dots++].y = y;
            if (dots == 100) {
                add(sendList, 0, tmp, sizeof(Pos)*dots);
                socket_write(fd, 0, sendList);
                cleanList(sendList);
                dots = 0;
            }
        }

        bezierCurve(plot, TIMES, p, DOTS);
        plotHandwriting(p, plot, 0);
        imshow(windowName, image);
    } else if (event == EVENT_LBUTTONUP) {
        if (fd > 0) {
            tmp[dots].x = -1;
            tmp[dots++].y = 0;
            add(sendList, 0, tmp, sizeof(Pos)*dots);
            socket_write(fd, 0, sendList);
            cleanList(sendList);
            dots = 0;
        }
        plotHandwriting(p, plot, 1);
        imshow(windowName, image);
    }
}

void bezierCurve (Point2d *det, int SR, Point2d *p, int length) {
    double t = 1.0/SR;
    double tn = 0.0;
    double s;
    double w[length];
    double distance[length];
    double angle[length-2];

    for (int i = 0; i < length; i++) {
        w[i] = 1;
    }

    for (int j = 0; j < length-2; j++) {
        triangleDist(p+j, distance);
        double a = (pow(distance[2], 2) - pow(distance[1], 2) - pow(distance[0], 2));
        double b = -2*distance[0]*distance[1];
        if (!isnormal(1.0/a) || !isnormal(1.0/b)) {
            angle[j] = 0.0;
        } else {
            angle[j] = a/b;
        }
    }
    for (int i = 1, j = 0; i < length-1; i++, j++) {
        w[i] = (1+sqrt(1 - pow(angle[j], 2)))*(fabs(1000-dist(p[i], p[i+1])))/100;
        if (1-pow(angle[j], 2) < 0 && !isnormal(w[i])) {
            w[i] = fabs(1000-dist(p[i], p[i+1]))/100;     
        }
    }

    for (int i = 0; i < SR; i++, tn+=t) {
        det[i] = Point(0,0);
        s = 0;
        for (int j = 0; j < length; j++) {
            double bz = (bezierPoly(t*i, length, j)) * w[j];
            det[i].x +=  bz * p[j].x;
            det[i].y += bz * p[j].y;
            s += bz; 
        }
        det[i].x /= s;
        det[i].y /= s;
    }
}

int factorial(int n) {
    int back = 1;
    for (int i = 1; i <= n; i++) {
        back *= i;
    }
    return back;
}
double dist (Point2d p1, Point2d p2) {
    return sqrt(pow(fabs(p1.x-p2.x), 2) + pow(fabs(p1.y-p2.y), 2));
}

double bezierPoly (double t, int n, int i) {
    return (factorial(n)/(factorial(i)*factorial(n-i))) * pow(t, i) * pow(1-t, n-i);
}

void triangleDist (Point2d *p, double *dists) {
    int i;
    for (i = 0; i < 2; i++) {
        if (p[i].x != p[i+1].x && p[i].y != p[i+1].y) {
            dists[i] = dist(p[i], p[i+1]);
        } else {
            dists[i] = 0.0;
        }
    }
    if (p[i].x != p[0].x && p[i].y != p[0].y) {
        dists[i] = dist(p[0], p[i]);
    } else {
        dists[i] = 0.0;
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

        if (distance <= 70) {
            for (i = 0; i < TIMES-1; i++) {
                line(image, plot[i], plot[i+1], handwritColor, thk);
            }
            line(image, plot[TIMES-1], p[DOTS-1], handwritColor, thk);
            p[0] = p[DOTS-1];
            i = 1;
        } else {
            for (i = 0; dist(plot[i], p[DOTS-1]) > 30 && i < TIMES-3; i++) {
                line(image, plot[i], plot[i+1], handwritColor, thk);
            }
            for (j = i; dist(plot[j], p[DOTS-1]) > 21.2 && j < TIMES-2; j++);
            p[0] = plot[i];
            p[1] = plot[j];
            p[2] = p[DOTS-1];
            i = 3;
        }
    } else {
        for (i = DOTS; i >= 3; i--) {
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
    }

    for (; i < DOTS; i++) {
        p[i] = Point(-1, -1);
    }
}
