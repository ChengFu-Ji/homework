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
#include <opencv2/highgui/highgui_c.h>

#include "bezier.h"
extern "C" {
#include "linkedlist.h"
}

#define TIMES 1000
#define DOTS 6

using namespace cv;

typedef struct _page {
    int pid;
    Node_s **history;
    struct _page *next;
} pageNode;

int clientInit (int, char *);
Node_s **linkedlistInit ();
void linkedlistFree (Node_s **);
void plotHandwriting (Point2d *, Point2d *, int);
void *recvData (void *);
void MouseWork (int, int, int, int, void *);
void buttonWork (int, int, int, int, void *);
Pos toPos (int, int);
void startSelector ();
void trackcolorSelector (int pos, void *colorChannel);
void trackThicknessPicker (int pos, void *userdata);
void addPage (pageNode **);
void deleteAllPages (pageNode **);
void prevPage (pageNode **);
void nextPage (pageNode **);
void drawList (Node_s **);
pageNode *getPage (int);

char windowName[] = "whiteboard";
char colorPickerWinn[] =  "selector";
Mat image;
Scalar color = Scalar(0, 0, 0);
Rect Bigbutton;
Node_s **recvList, **sendList;
pageNode **pages;
int pass = 0, thk = 1, curid = 0;

int main () {
    pthread_t t;
    int fd;

    /*button bar*/
    Mat img = Mat(400, 80, CV_8UC3, Scalar(255, 255, 255));
    line(img, Point(img.cols/2, 0), Point(img.cols/2, img.rows), Scalar(0, 0, 0), 2); 
    for (int i = 40; i < img.rows; i+=40) {
        line(img, Point(0, i), Point(img.cols , i), Scalar(0, 0, 0), 2); 
    }

    namedWindow("button", WINDOW_AUTOSIZE);
    imshow("button", img);
    //img.release();

    image = Mat(950, 1600, CV_8UC3, Scalar(255, 255, 255));
    Bigbutton = Rect(0, 0, 1600, 50);

    int port = 2244;
    char address[] = "localhost";

    sendList = linkedlistInit();

    pages = (pageNode **)malloc(sizeof(pageNode *));
    *pages = (pageNode *)malloc(sizeof(pageNode));
    (*pages)->pid = -1;
    (*pages)->next = NULL;
    addPage(pages);

    startSelector();
    namedWindow(windowName, WINDOW_FULLSCREEN);

    image(Bigbutton) = Scalar(200,200,200);
    /*
    putText(image, "12312342", Point(400, 400), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(5, 25, 2),2);
    */
    imshow(windowName, image);

    setMouseCallback(windowName, MouseWork, (void *) &fd);
    setMouseCallback("button", buttonWork, NULL);

    if ((fd = clientInit(port, address)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", address, port);
    } else {
        pthread_create(&t, NULL, recvData, (void *) &fd);
        recvList = linkedlistInit();
    }

    int id = 0, key = 0;
    while (1) {
        if (getWindowProperty(windowName, WINDOW_NORMAL) < 0 && getWindowProperty(colorPickerWinn, WINDOW_NORMAL) < 0 || pollKey() == 27) {
            break;
        }

        if (fd > 0) {
            if (pthread_kill(t, 0) == ESRCH) {
                pthread_create(&t, NULL, recvData, (void *) &fd);
                imshow(windowName, image);
            }
        }
    }

    linkedlistFree(sendList);

    deleteAllPages(pages);

    free(*pages);
    free(pages);

    printf("Exiting...\n");
    if (fd > 0) {
        if (pthread_kill(t, 0) != ESRCH) {
            pthread_cancel(t);
        }
        linkedlistFree(recvList);
        close(fd);
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
    int sockfd = *(int *) fd, i = 0, j = 0;
    int id, size, n;
    double distance = 0.0;

    if (read(sockfd, &size, sizeof(int)) < 0) {
        pthread_exit(NULL);
    }

    if ((n = socket_read(recvList, sockfd, size)) > 0) {
        drawList (recvList);
        //IDdelete(recvList, id);
        cleanList(recvList);
    }
    pthread_exit(0);
}

void buttonWork (int event, int x, int y, int flags, void *userdata) {

    if (event == EVENT_LBUTTONDOWN) {
        if ((y >= 0 && y < 40)) {
            if ((x >= 0 && x < 40)) {
                /* eraser */
                color = Scalar(255, 255, 255);
            } else if ((x >= 40 && x < 80)){
                addPage(pages);
            }
        } else if (y >= 40 && y < 80) {
            if ((x >= 0 && x < 40)) {
                prevPage(pages);
            } else if ((x >= 40 && x < 80)){
                nextPage(pages);
            }
        }
    }
}

void MouseWork (int event, int x, int y, int flags, void *userdata) {
    static Pos tmp;
    static Point2d p[DOTS];
    static int dots = 0;
    Point2d plot[TIMES] ;
    int fd = *(int *) userdata, i = 0, j = 0;
    double distance = 0.0;

    if (event == EVENT_LBUTTONDOWN) {

        if (Bigbutton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
            image(Bigbutton) = Scalar(100, 100, 100);
            imshow(windowName, image);

        } else {
            p[0] = Point(x, y);

            if (fd > 0) {
                dots++;
                add(sendList, toPos(x, y));
            }
            for (i = 1; i < DOTS; i++) {
                p[i] = Point(-1, -1);
            }
        }
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) && p[0].x != -1 && p[0].y != -1) {

        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = Point(x, y);
                dots++;
                add(sendList, toPos(x, y));
                return;
            }
        }

        p[DOTS-1] = Point(x, y);
        dots++;
        add(sendList, toPos(x, y));

        bezierCurve(plot, TIMES, p, DOTS);
        plotHandwriting(p, plot, 0);
        imshow(windowName, image);
    } else if (event == EVENT_LBUTTONUP) {

        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = Point(x, y);
                dots++;
                add(sendList, toPos(x, y));
            }
        }

        dots++;
        add(sendList, toPos(-1, 0));
        if (fd > 0) {
            socket_write(sendList, fd, dots);
        }
        dots = 0;

        pageNode *page = getPage(curid);
        if (page != NULL) {
            printf("page ID %d\n", page->pid);
            listcat(page->history, sendList);
            showList(page->history);
            (*sendList)->next = NULL;
        } else {
            cleanList(sendList);
        }

        if (p[0].x != -1 && p[0].y != -1) {
            plotHandwriting(p, plot, 1);
        }
        imshow(windowName, image);
    }

    if (Bigbutton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
        image(Bigbutton) = Scalar(200,200,200);
    }
}

void plotHandwriting (Point2d *p, Point2d *plot, int repair) {
    int i, j;
    double distance = 0.0;
    Scalar handwritColor = color;

    if (!repair) {
        for (i = 0; i < DOTS-1; i++) {
            distance += dist(p[i], p[i+1]);
        }

        if (distance <= 70) {
            for (i = 0; i < DOTS-1; i++) {
                line(image, p[i], p[i+1], handwritColor, thk);
            }
            p[0] = p[DOTS-1];
            i = 1;
        } else {
            distance = 0;
            for (i = 0; i < TIMES-1; i++) {
                distance += dist(plot[i], plot[i+1]);
            }

            for (i = 0; distance > 20 && i < TIMES-2; distance -= dist(plot[i], plot[i+1]), i++) {
                if (distance > 40) {
                    line(image, plot[i], plot[i+1], handwritColor, thk);
                    j = i+1;
                }
            }

            p[0] = plot[j];
            p[1] = plot[i];
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

    image(Bigbutton) = Scalar(200,200,200);
}

Pos toPos(int x, int y) {
    Pos tmp;
    tmp.x = x;
    tmp.y = y;
    return tmp;
}


void trackcolorSelector(int pos, void *colorChannel) {
    static int RChannel = 0, GChannel = 0, BChannel = 0;
    char t = *(char *) colorChannel;
    if (t == 'r') {
        RChannel = pos;
    } else if (t == 'g') {
        GChannel = pos;
    } else if (t == 'b') {
        BChannel = pos;
    }

    color = Scalar(BChannel, GChannel, RChannel);
    Mat show = Mat(250, 250, CV_8UC3, color);
    imshow(colorPickerWinn, show);
}

/*
 *  筆畫厚度軌跡條
 */
void trackThicknessPicker(int pos, void *userdata) {
    thk = pos+1;
}

void startSelector() {
    static char channel[3] = {'r', 'g', 'b'};

    if (cvGetWindowHandle(colorPickerWinn) == 0) {
        namedWindow(colorPickerWinn, WINDOW_NORMAL | WINDOW_GUI_NORMAL);
        imshow(colorPickerWinn, Mat(100, 100, CV_8UC3, color));
        createTrackbar("R", colorPickerWinn, 0, 255, trackcolorSelector, (void *) &channel[0]);
        createTrackbar("G", colorPickerWinn, 0, 255, trackcolorSelector, (void *) &channel[1]);
        createTrackbar("B", colorPickerWinn, 0, 255, trackcolorSelector, (void *) &channel[2]);
        createTrackbar("thickness", "button", 0, 999, trackThicknessPicker, NULL);
    }
}

void addPage (pageNode **pageHead) {
    static int id = 0;
    pageNode *cur, *newNode;

    cur = *pageHead;
    while (cur->next != NULL) {
        cur = cur->next;
    }

    newNode = (pageNode *)malloc(sizeof(pageNode));
    newNode->pid = id++;
    newNode->history = linkedlistInit();
    newNode->next = NULL;

    cur->next = newNode;
}

void deleteAllPages (pageNode **pageHead) {
    pageNode *cur, *next;
    cur = (*pageHead);   

    while (cur->next != NULL) {
        next = cur->next->next;
        cleanList(cur->next->history);
        linkedlistFree(cur->next->history);
        free(cur->next);
        cur->next = next;
    }
}

void prevPage (pageNode **pageHead) {
    pageNode *cur;

    cur = (*pageHead)->next;

    /* if first page */
    if (cur->pid == curid) {
        return;
    }

    while (cur->next != NULL && cur->next->pid != curid) {
        cur = cur->next;
    }

    image.release();
    image = Mat(950, 1600, CV_8UC3, Scalar(255, 255, 255));

    drawList(cur->history);
    imshow(windowName, image);
    curid = cur->pid;
}

void nextPage (pageNode **pageHead) {
    pageNode *cur;

    cur = (*pageHead)->next;
    while (cur->next != NULL && cur->pid != curid) {
        cur = cur->next;
    }

    /* if last page */
    if (cur->next == NULL) {
        return;
    }

    image.release();
    image = Mat(950, 1600, CV_8UC3, Scalar(255, 255, 255));

    drawList (cur->next->history);
    imshow(windowName, image);
    curid = cur->next->pid;
}

void drawList (Node_s **list) {
    Point2d p[DOTS], plot[TIMES];
    Node_s *cur;
    int i;

    for (i = 0; i < DOTS; i++) {
        p[i] = Point(-1, -1);
    }
    cur = (*list)->next;
    while (cur != NULL) {
        if (cur->point.x != -1 && cur->point.y != 0) {
            for (i = 0; i < DOTS-1; i++) {
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
            
        } else {
            plotHandwriting(p, plot, 1);

            for (i = 0; i < DOTS; i++) {
                p[i] = Point(-1, -1);
            }
        }
        cur = cur->next;
    }

    plotHandwriting(p, plot, 1);
} 
  
pageNode *getPage (int id) {
    pageNode *cur;

    cur = (*pages)->next;
    while (cur->next != NULL && cur->pid != id) {
        cur = cur->next;
    }

    if (cur->next == NULL && cur->pid != id) {
        return NULL;
    } else {
        return cur;
    }
}
