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

#include "linkedlist.h"

#define TIMES 1000
#define DOTS 6

using namespace cv;

int clientInit (int, char *);
Node_s **linkedlistInit ();
void linkedlistFree (Node_s **);
int factorial(int);
double dist (Point2d, Point2d);
double bezierPoly (double, int, int); 
void triangleDist (Point2d *, double *);
void bezierCurve (Point2d *, int, Point2d *, int);
void plotHandwriting (Point2d *, Point2d *, int);
void *recvData (void *);
void MouseWork (int, int, int, int, void *);

char windowName[] = "whiteboard";
Mat image;
Node_s **recvList, **sendList;

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
            }
        }
    }
    if (fd > 0) {
        if (pthread_kill(t, 0) != ESRCH) {
            pthread_cancel(t);
        }
    }
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
    int id, size;
    double distance = 0.0;

    if (read(sockfd, &size, sizeof(int)) < 0) {
        pthread_exit(NULL);
    }

    if (socket_read(recvList, sockfd, size)) {
        for (i = 0; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
        cur = (*recvList)->next;
        while (cur != NULL) {
            //printf("%d:cur %d %d\n", cur->point.id, cur->point.x, cur->point.y);
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
                imshow(windowName, image);
            }
            cur = cur->next;
        }

        plotHandwriting(p, plot, 1);
        imshow(windowName, image);
        //IDdelete(recvList, id);
        cleanList(recvList);
    }
    pthread_exit(0);
}

void MouseWork (int event, int x, int y, int flags, void *userdata) {
    static Data_s tmp;
    static Point2d p[DOTS];
    static int dots = 0;
    Point2d plot[TIMES] ;
    int fd = *(int *) userdata, i = 0, j = 0;
    double distance = 0.0;

    if (event == EVENT_LBUTTONDOWN) {
        p[0] = Point(x, y);

        if (fd > 0) {
            dots++;
            tmp.x = x;
            tmp.y = y;
            add(sendList, tmp);
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
                    tmp.x = x;
                    tmp.y = y;
                    add(sendList, tmp);
                }
                return;
            }
        }

        p[DOTS-1] = Point(x, y);
        if (fd > 0) {
            dots++;
            tmp.x = x;
            tmp.y = y;
            add(sendList, tmp);
        }

        bezierCurve(plot, TIMES, p, DOTS);
        plotHandwriting(p, plot, 0);
        imshow(windowName, image);
    } else if (event == EVENT_LBUTTONUP) {
        if (fd > 0) {
            dots++;
            tmp.x = -1;
            tmp.y = 0;
            add(sendList, tmp);
            socket_write(sendList, fd, dots);
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
        if ((int) (abs(a)-abs(b)) != 0) {
            angle[j] = acos( a/b )/acos(-1)*180;
        } else {
            angle[j] = 180;
            if ((int) a == (int) b) {
                angle[j] = 0;
            }
        }
 
    }
    for (int i = 1, j = 0; i < length-1; i++, j++) {
        w[i] = (1+fabs(sin(angle[j]*acos(-1)/180)))*(fabs(1000-dist(p[i], p[i+1])))/100;
        //w[i] = 2;
    }
    /*
    for (int i = 0; i < length; i++) {
        printf("w[i] %lf\n", w[i]); 
    }
    */

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
        dists[i] = dist(p[i], p[i+1]);
    }
    dists[i] = dist(p[0], p[i]);
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
            p[0] = plot[i-1];
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
