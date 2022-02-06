#include "wbwindows.h"

using namespace cv;

char Winn[] = "whiteboard";
Mat image;
Rect topButton;
int fd;

int main () {
    pthread_t t;
    posNode **recvList, **sendList;
    pageNode **pages;
    int curid, is_eraser, thk;

    char host[] = "localhost";
    int port = 2244;

    initPage(&pages);
    thk = 3;
    is_eraser = 0;
    curid = 0;
    addPage(pages, curid);

    image = Mat(950, 1600, CV_8UC3, Scalar(255, 255, 255));
    topButton = Rect(0, 0, 1600, 50);

    namedWindow(Winn, WINDOW_AUTOSIZE);
    image(topButton) = Scalar(200,200,200);
    imshow(Winn, image);

    initPos(&sendList);
    struct _sender s = {pages, sendList, &thk, &is_eraser, &curid};
    setMouseCallback(Winn, mouseOnWhiteboard, (void *) &s);

    setbuttonBar(s);

    if ((fd = connectToServer(host, port)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", host, port);
    } else {
        initPos(&recvList);
        
        struct _sender recv = {pages, recvList, NULL, NULL, &curid};
        pthread_create(&t, NULL, recvData, (void *) &recv);
    }

    char tmp;
    while (1) {
        if ((tmp = pollKey()) ==  27) {
            //waitKey(0);
            break;
        }
        /*
        if (tmp != -1) {
            printf("%d %c\n", tmp, tmp);
        }
        */
        /* ctrl + n / ctrl + x / ctrl + z / ctrl + d/ (Mac)*/
        if (tmp == 14) {
            pageNode *page = getLastPage(pages);
            addPage(pages, (page->pid)+1);

            SockCond addpage = {-1, (page->pid)+1, -1, 0, 0, 0};
            write(fd, &addpage, sizeof(SockCond));

            //addPage(pages, ++id);
            printf("added\n");
        } else if (tmp == 24) {
            changeToPage(getPage(pages, curid)->next, &curid);
            printf("next page\n");
        } else if (tmp == 26) {
            changeToPage(getPrevPage(pages, curid), &curid);
            printf("prev page\n");
        } else if (tmp == 4) {
            pageNode *page = getPrevPage(pages, curid);
            if (page == NULL) {
                page = getPage(pages, curid)->next;
            }
            if (page != NULL) {
                SockCond delpage = {-1, -1, curid, 0, 0, 0};
                write(fd, &delpage, sizeof(SockCond));

                delPage(pages, curid);
                changeToPage(page, &curid);

            }

            printf("delete\n");
        } else if (tmp == 25) {
            getValue(&thk, 4);
            printf("thk now %d\n", thk);
        }

        if (fd > 0) {
            if (pthread_kill(t, 0) == ESRCH) {
                struct _sender recv = {pages, recvList, NULL, NULL, &curid};
                pthread_create(&t, NULL, recvData, (void *) &recv);
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


void *recvData (void *sender) {
    int n;
    //recvList = (posNode **)recv;
    struct _sender s = *(struct _sender *) sender;
    posNode **recvList = s.posHead;
    pageNode **pages = s.pageHead;
    int *curid = s.id;
    SockCond recv;
    
    if (read(fd, &recv, sizeof(SockCond)) < 0) {
        pthread_exit(NULL);
    }

    if (recv.addpid > 0) {
        addPage(pages, recv.addpid);
    } 
    if (recv.poslen > 0) {
        if ((n = socket_read(fd, recvList, recv.poslen)) > 0) {
            if (recv.drawpid == *curid) {
                drawPosList(recvList, recv.thickness, recv.eraser);
                //IDdelete(recvList, id);
                //deleteAllPos(recvList);
            }
            pageNode *page = getPage(pages, recv.drawpid);
            if (page != NULL) {
                addStroke(page->strokes, recvList, recv.thickness, recv.eraser);
                (*recvList)->next = NULL;
            }
        }
    } 

    if (recv.deletepid >= 0) {
        pageNode *page = getPrevPage(pages, recv.deletepid);
        if (page == NULL) {
            page = getPage(pages, recv.deletepid)->next;
        }
        if (page != NULL) {
            delPage(pages, recv.deletepid);
            if (*curid == recv.deletepid) {
                changeToPage(page, curid);
            }
        }
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
    int *eraser = s.is_eraser;
    int *thk = s.thk;


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
        plotHandwriting(p, plot, 0, *thk, *eraser);
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

        pageNode *page = getPage(pages, *curid);
        if (page != NULL) {
            //printf("page ID %d\n", page->pid);
            addStroke(page->strokes, sendList, *thk, *eraser);
            if (fd > 0) {
                dots++;
                //socket_write(fd, sendList, dots);
                SockCond send = { *curid, -1, -1, *eraser, *thk, dots};
                sendStroke(fd, getLastStroke(page->strokes), send);
                dots = 0;
            }
            (*sendList)->next = NULL;
        } else {
            deleteAllPos(sendList);
        }

        if (p[0].x != -1 && p[0].y != -1) {
            plotHandwriting(p, plot, 1, *thk, *eraser);
        }
        imshow(Winn, image);
    }

    if (topButton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
        image(topButton) = Scalar(200,200,200);
    }
}


void plotHandwriting (DPos *p, DPos *plot, int repair, int thickness, int eraser) {
    int i, j;
    double distance = 0.0;
    Scalar handwritColor = Scalar(170, 190, 110);
    if (eraser) {
        handwritColor = Scalar(255, 255, 255);
    }
    int thk = thickness;
    if (thickness == 0) {
        thk = 3;
    }
    

    if (!repair) {
        for (i = 0; i < DOTS-1; i++) {
            distance += dist(p[i], p[i+1]);
        }

        if (distance <= 70) {
            for (i = 0; i < DOTS-1; i++) {
                line(image, Point(p[i].x, p[i].y), Point(p[i+1].x, p[i+1].y), handwritColor, thk+9);
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
    if (page == NULL) {
        return;
    }

    if (page->pid == *curid) {
        printf("same page\n");
        return;
    }


    image.release();
    image = Mat(950, 1600, CV_8UC3, Scalar(255, 255, 255));

    
    drawStrokeList(page->strokes);
    imshow(Winn, image);
    *curid = page->pid;
}


void drawStrokeList (strokeNode **strokes) {
    strokeNode *cur;

    cur = (*strokes);
    while (cur->next != NULL) {
        drawPosList(cur->next->pos, cur->next->thickness, cur->next->is_eraser);

        cur = cur->next;
    }
}

void drawPosList (posNode **pos, int thk, int is_eraser) {
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
                    //printf("p[%d] %d %d\n", i, cur->point.x, cur->point.y);
                    cur = cur->next;
                    break;
                }
            }
            if (i != DOTS-1) {
                continue;
            }
            p[DOTS-1] = (DPos) { (double) cur->point.x, (double) cur->point.y};
            bezierCurve(plot, TIMES, p, DOTS);
            //printf("plot[0] %lf %lf\n", plot[0].x, plot[0].y);
            //printf("p[0] %lf %lf\n", p[0].x, p[0].y);
            plotHandwriting(p, plot, 0, thk, is_eraser);
            
        } else {
            plotHandwriting(p, plot, 1, thk, is_eraser);

            for (i = 0; i < DOTS; i++) {
                p[i] = (DPos) {-1, -1};
            }
        }
        cur = cur->next;
    }

}
