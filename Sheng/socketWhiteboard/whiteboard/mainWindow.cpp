#include <stdio.h>
#include <stdlib.h>
/*memset*/
#include <string.h>
//#include <math.h>

/* man tcp */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* gethostbyname */
#include <netdb.h>

/* write read close functions */
#include <unistd.h>

#include <pthread.h>
#include <signal.h>

/* opencv */
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

/* my include */
extern "C" {
#include "bezier.h"
#include "linkedlist/pos.h"
#include "linkedlist/stroke.h"
#include "linkedlist/page.h"
}

#define TIMES 1000
#define DOTS 6

using namespace cv;

struct _sender {
    pageNode **pageHead;
    posNode **posHead;
    int *id;
};

int connectToServer (char *, int);
void *recvData (void *);
void mouseOnWhiteboard (int, int, int, int, void *);
void plotHandwriting (DPos *p, DPos *plot, int repair);
void changeToPage (pageNode *, int *);
void drawPosList (posNode **);
void drawStrokeList (strokeNode **);

char Winn[] = "whiteboard";
Mat image;
Rect topButton;
int fd;

int main () {
    pthread_t t;
    posNode **recvList, **sendList;
    pageNode **pages;
    int id, curid;

    char host[] = "localhost";
    int port = 2244;

    initPage(&pages);
    curid = (id = 0);
    addPage(pages, id);

    image = Mat(950, 1600, CV_8UC3, Scalar(255, 255, 255));
    topButton = Rect(0, 0, 1600, 50);

    namedWindow(Winn, WINDOW_AUTOSIZE);
    image(topButton) = Scalar(200,200,200);
    imshow(Winn, image);

    initPos(&sendList);
    struct _sender s = {pages, sendList, &curid};
    setMouseCallback(Winn, mouseOnWhiteboard, (void *) &s);

    if ((fd = connectToServer(host, port)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", host, port);
    } else {
        initPos(&recvList);
        pthread_create(&t, NULL, recvData, (void *) recvList);
    }

    char tmp;
    while (1) {
        if ((tmp = pollKey()) ==  27) {
            //waitKey(0);
            break;
        }
        if (tmp != -1) {
            printf("%d %c\n", tmp, tmp);
        }
        /* ctrl + n / ctrl + x / ctrl + z / ctrl + d/ (Mac)*/
        if (tmp == 14) {
            addPage(pages, ++id);
            printf("added\n");
        } else if (tmp == 24) {
            changeToPage(getPage(pages, curid)->next, &curid);
            printf("next page\n");
        } else if (tmp == 26) {
            changeToPage(getPrevPage(pages, curid), &curid);
            printf("prev page\n");
        } else if (tmp == 4) {
            int tmpid = curid;
            changeToPage(getPrevPage(pages, curid), &curid);
            delCurrentPage(pages, tmpid);
        }

        if (fd > 0) {
            if (pthread_kill(t, 0) == ESRCH) {
                pthread_create(&t, NULL, recvData, (void *) &fd);
                imshow(Winn, image);
            }
        }
    }

    deleteAllPages(pages);

    freePos(sendList);
    freePage(pages);

    if (fd > 0) {
        if (pthread_kill(t, 0) != ESRCH) {
            pthread_cancel(t);
        }
        freePos(recvList);
        close(fd);
    }

    return 0;
}


int connectToServer(char *address, int port) {
    int fd;
    struct sockaddr_in addr;
    struct hostent *host;

    if ((host = gethostbyname(address)) == NULL) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr, host->h_addr_list[0], host->h_length);

    if (connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }
    
    return fd;
}


void *recvData(void *recv) {
    posNode **recvList;
    int length, n;
    recvList = (posNode **)recv;

    if (read(fd, &length, sizeof(int)) < 0) {
        pthread_exit(NULL);
    }

    if ((n = socket_read(fd, recvList, length)) > 0) {
        //drawList (recvList);
        //IDdelete(recvList, id);
        //cleanList(recvList);
        deleteAllPos(recvList);
    }
    pthread_exit (0);
}

void mouseOnWhiteboard (int event, int x, int y, int flags, void *sender) {
    static Pos tmp;
    static DPos p[DOTS];
    static int dots = 0;

    DPos plot[TIMES];
    int i = 0, j = 0;
    double distance = 0.0;

    struct _sender s = *(struct _sender *) sender;
    pageNode **pages = s.pageHead;
    posNode **sendList = s.posHead;
    int *curid = s.id;


    if (event == EVENT_LBUTTONDOWN) {
        if (topButton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
            image(topButton) = Scalar(100, 100, 100);
            imshow(Winn, image);

        } else {
            p[0] = (DPos) {(double) x, (double) y};

            if (fd > 0) {
                dots++;
            }
            addPos(sendList, (Pos) {x, y});

            for (i = 1; i < DOTS; i++) {
                p[i] = (DPos) {-1, -1};
            }
        }
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) && p[0].x != -1 && p[0].y != -1) {
        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = (DPos) {(double) x, (double) y};

                if (fd > 0) {
                    dots++;
                }

                addPos(sendList, (Pos) {x, y});
                return;
            }
        }

        p[DOTS-1] = (DPos) {(double) x, (double) y};

        if (fd > 0) {
            dots++;
        }
        addPos(sendList, (Pos) {x, y});

        bezierCurve(plot, TIMES, p, DOTS);
        plotHandwriting(p, plot, 0);
        imshow(Winn, image);
    } else if (event == EVENT_LBUTTONUP) {

        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = (DPos) {(double) x, (double) y};
                if (fd > 0) {
                    dots++;
                }
                addPos(sendList, (Pos) {x, y});
            }
        }

        addPos(sendList, (Pos) {-1, 0});
        if (fd > 0) {
            dots++;
            socket_write(fd, sendList, dots);
            dots = 0;
        }

        pageNode *page = getPage(pages, *curid);
        if (page != NULL) {
            //printf("page ID %d\n", page->pid);
            addStroke(page->strokes, sendList);
            (*sendList)->next = NULL;
        } else {
            deleteAllPos(sendList);
        }

        if (p[0].x != -1 && p[0].y != -1) {
            plotHandwriting(p, plot, 1);
        }
        imshow(Winn, image);
    }

    if (topButton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
        image(topButton) = Scalar(200,200,200);
    }
}


void plotHandwriting (DPos *p, DPos *plot, int repair) {
    int i, j;
    double distance = 0.0;
    Scalar handwritColor = Scalar(170, 190, 110);
    int thk = 5;

    if (!repair) {
        for (i = 0; i < DOTS-1; i++) {
            distance += dist(p[i], p[i+1]);
        }

        if (distance <= 70) {
            for (i = 0; i < DOTS-1; i++) {
                line(image, Point(p[i].x, p[i].y), Point(p[i+1].x, p[i+1].y), handwritColor, thk);
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
                    line(image, Point(plot[i].x, plot[i].y), Point(plot[i+1].x, plot[i+1].y), handwritColor, thk);
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
                    line(image, Point(plot[j].x, plot[j].y), Point(plot[j+1].x, plot[j+1].y), handwritColor, thk);
                }
                line(image, Point(plot[TIMES-1].x, plot[TIMES-1].y), Point(p[i-1].x, p[i-1].y), handwritColor, thk);
                break;
            }
        }

        if (p[1].x != -1 && p[1].y != -1) {
            line(image, Point(p[0].x, p[0].y), Point(p[1].x, p[1].y), handwritColor, thk); 
        }
        i = 0;
    }

    for (; i < DOTS; i++) {
        p[i] = (DPos) {-1, -1};
    }

    image(topButton) = Scalar(200,200,200);
}

void changeToPage (pageNode *page, int *curid) {
    if (page->pid == *curid) {
        printf("same page\n");
        return;
    }


    image.release();
    image = Mat(950, 1600, CV_8UC3, Scalar(255, 255, 255));

    
    drawStrokeList(page->strokes);
    imshow(Winn, image);
    printf("curid %d\n", *curid);
    *curid = page->pid;
    printf("curid %d\n", *curid);
}


void drawStrokeList (strokeNode **strokes) {
    strokeNode *cur;

    cur = (*strokes);
    while (cur->next != NULL) {
        drawPosList(cur->next->pos);

        cur = cur->next;
    }
}

void drawPosList (posNode **pos) {
    DPos p[DOTS], plot[TIMES];
    posNode *cur;
    int i;

    for (i = 0; i < DOTS; i++) {
        p[i] = (DPos) {-1, -1};
    }
    cur = (*pos)->next;
    while (cur != NULL) {
        if (cur->point.x != -1 && cur->point.y != 0) {
            for (i = 0; i < DOTS-1; i++) {
                if (p[i].x == -1 && p[i].y == -1) {
                    p[i] = (DPos) { (double) cur->point.x, (double) cur->point.y};
                    cur = cur->next;
                    break;
                }
            }
            if (i != DOTS-1) {
                continue;
            }
            p[DOTS-1] = (DPos) { (double) cur->point.x, (double) cur->point.y};
            bezierCurve(plot, TIMES, p, DOTS);
            plotHandwriting(p, plot, 0);
            
        } else {
            plotHandwriting(p, plot, 1);

            for (i = 0; i < DOTS; i++) {
                p[i] = (DPos) {-1, -1};
            }
        }
        cur = cur->next;
    }

    //plotHandwriting(p, plot, 1);
}
