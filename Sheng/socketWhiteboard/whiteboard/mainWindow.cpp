/* header for all of whiteboard needs */
#include "wbwindows.h"

using namespace cv;

/* 
 *   image is used to show user's strokes in current page.
 *
 *   topButton switch personal page and common page.
 *   buttonBar contains buttons that do actions, 
 *   (like: prev page , next page , add page , delete page, change page ...).
 *
 *   fd - socket client's file descriptor.
 */

char Winn[] = "whiteboard";
Mat image;
Rect topButton, buttonBar;
int fd;

int main () {
    pthread_t recvThread;
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

    image = Mat(950, 1700, CV_8UC3, Scalar(255, 255, 255));
    topButton = Rect(0, 0, image.cols, 50);
    buttonBar = Rect(image.cols-100, topButton.height, 100, image.rows-topButton.height);

    namedWindow(Winn, WINDOW_AUTOSIZE);

    image(topButton) = Scalar(200,200,200);
    setActionBar();
    imshow(Winn, image);

    initPos(&sendList);
    struct _sender s = {pages, sendList, &thk, &is_eraser, &curid};
    setMouseCallback(Winn, mouseOnWhiteboard, (void *) &s);
    //setbuttonBar(s);

    if ((fd = connectToServer(host, port)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", host, port);
    } else {
        initPos(&recvList);
        
        struct _sender recv = {pages, recvList, NULL, NULL, &curid};
        pthread_create(&recvThread, NULL, recvData, (void *) &recv);
    }

    char userKeyIn;
    while (1) {
        if ((userKeyIn = pollKey()) ==  27) {
            break;
        }

        /*
        if (tmp != -1) {
            printf("%d %c\n", tmp, tmp);
        }
        */
        /* ctrl + n / ctrl + x / ctrl + z / ctrl + d/ (Mac)*/
        if (userKeyIn == 14) {
            pageNode *page = getLastPage(pages);
            addPage(pages, (page->pid)+1);

            if (fd > 0) {
                SockCond addpage = {-1, (page->pid)+1, -1, 0, 0, 0};
                write(fd, &addpage, sizeof(SockCond));
            }

            //addPage(pages, ++id);
            printf("added\n");
        } else if (userKeyIn == 24) {
            changeToPage(getPage(pages, curid)->next, &curid);
            printf("next page\n");
        } else if (userKeyIn == 26) {
            changeToPage(getPrevPage(pages, curid), &curid);
            printf("prev page\n");
        } else if (userKeyIn == 4) {
            pageNode *page = getPrevPage(pages, curid);
            if (page == NULL) {
                page = getPage(pages, curid)->next;
            }
            if (page != NULL) {
                if (fd > 0) {
                    SockCond delpage = {-1, -1, curid, 0, 0, 0};
                    write(fd, &delpage, sizeof(SockCond));
                }

                delPage(pages, curid);
                changeToPage(page, &curid);

            }

            printf("delete\n");
        } else if (userKeyIn == 25) {
            getValue(&thk, 4);
            printf("thk now %d\n", thk);
        } else if (userKeyIn == 23) {
            storeImage(getPage(pages, curid), "tmp.png");
        }

        if (fd > 0) {
            if (pthread_kill(recvThread, 0) == ESRCH) {
                struct _sender recv = {pages, recvList, NULL, NULL, &curid};
                pthread_create(&recvThread, NULL, recvData, (void *) &recv);
                imshow(Winn, image);
            }
        }
    }

    deleteAllPages(pages);

    freePos(sendList);
    freePage(pages);

    if (fd > 0) {
        if (pthread_kill(recvThread, 0) != ESRCH) {
            pthread_cancel(recvThread);
        }
        freePos(recvList);
        close(fd);
    }

    return 0;
}

/* 
 *  use address and port connect to server
 *  if failed return -1 else return client's fd
 */
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

/*
 *  a thread function used to receive handwriting stroke and plot on whiteboard.
 */
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
                drawPosList(image, recvList, recv.thickness, recv.eraser);
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

        } else if (buttonBar.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
            /*input y get action (tmp) */
            if (y < 50) {
                return;
            } else if (y < 150) {
                /* eraser */
                if (*eraser) {
                    *eraser = 0;
                    printf("eraser off!!\n");
                } else {
                    *eraser = 1;
                    printf("eraser on!!\n");
                }

            } else if (y < 250) {
                int pageCount;
                getValue(&pageCount, 4);
                changeToPage(getPagebyOrder(pages, pageCount), curid);
                printf("change to page %d\n", pageCount);
            } else if (y < 350) {
                changeToPage(getPrevPage(pages, *curid), curid);
                printf("prev\n");
            } else if (y < 450) {
                changeToPage(getPage(pages, *curid)->next, curid);
                printf("next\n");
            } else if (y < 550) {
                pageNode *page = getLastPage(pages);
                addPage(pages, (page->pid)+1);
                printf("added\n");
            } else if (y < 650) {
                pageNode *page = getPrevPage(pages, *curid);
                if (page == NULL) {
                    page = getPage(pages, *curid)->next;
                }

                if (page != NULL) {
                    delPage(pages, *curid);
                    changeToPage(page, curid);
                    printf("delete\n");
                }

            }
            //setActionBar();
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
        plotHandwriting(image, p, plot, 0, *thk, *eraser);
        imshow(Winn, image);
    } else if (event == EVENT_LBUTTONUP) {

        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = (DPos) {(double) x, (double) y};
                if (fd > 0) {
                    dots++;
                }
                addPos(sendList, (Pos) {x, y});
                break;
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
            plotHandwriting(image, p, plot, 1, *thk, *eraser);
        }
        imshow(Winn, image);
    }

    if (topButton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
        image(topButton) = Scalar(200,200,200);
    }
}


void plotHandwriting (Mat image, DPos *p, DPos *plot, int repair, int thickness, int eraser) {
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

    /* redraw button area */
    image(topButton) = Scalar(200,200,200);
    setActionBar();
}

void setActionBar () {

    image(buttonBar) = Scalar(100,100,100);
    line(image, Point(buttonBar.x, buttonBar.y), Point(buttonBar.x, image.rows), Scalar(0, 0, 0), 2);
    for (int i = 0; i <= buttonBar.height; i += (buttonBar.height/9)) {
        line(image, Point(buttonBar.x, buttonBar.y + i), Point(image.cols, buttonBar.y + i), Scalar(0, 0, 0), 2); 
    }

    /* draw icon (testing)*/
    /*eraser icon */
    drawEraserIcon(image, 1602, 52, 99*99);

    /* page */
    drawSelectPageIcon(image, 1600, 200, 100*100);
    //putText(image, "page", Point(1612, 210), FONT_HERSHEY_SIMPLEX, 1, Scalar(5, 25, 2), 2, LINE_AA);
    
    /* prev page */
    drawPrevPageIcon(image, 1600, 250, 100*100);
    /*
    line(image, Point(1670, 270), Point(1670, 330), Scalar(0, 0, 0), 3);
    line(image, Point(1670, 270), Point(1620, 300), Scalar(0, 0, 0), 3);
    line(image, Point(1670, 330), Point(1620, 300), Scalar(0, 0, 0), 3);
    */

    /* next page */
    drawNextPageIcon(image, 1600, 350, 100*100);
    /*
    line(image, Point(1630, 370), Point(1630, 430), Scalar(0, 0, 0), 3);
    line(image, Point(1630, 370), Point(1680, 400), Scalar(0, 0, 0), 3);
    line(image, Point(1630, 430), Point(1680, 400), Scalar(0, 0, 0), 3);
    */
    
    /* add page */
    line(image, Point(1620, 500), Point(1690, 500), Scalar(0, 0, 0), 3);
    line(image, Point(1655, 470), Point(1655, 530), Scalar(0, 0, 0), 3);

    /* del page */
    line(image, Point(1620, 600), Point(1690, 600), Scalar(0, 0, 0), 3);
}

void changeToPage (pageNode *page, int *curid) {
    if (page == NULL) {
        return;
    } else if (page->pid == *curid) {
        //printf("same page\n");
        return;
    }

    image.release();
    image = Mat(950, 1700, CV_8UC3, Scalar(255, 255, 255));

    drawStrokeList(image, page->strokes);
    image(topButton) = Scalar(200,200,200);
    setActionBar();
    imshow(Winn, image);
    *curid = page->pid;
}

/* draw all strokes of list*/
void drawStrokeList (Mat image, strokeNode **strokes) {
    strokeNode *cur;

    cur = (*strokes);
    while (cur->next != NULL) {
        drawPosList(image, cur->next->pos, cur->next->thickness, cur->next->is_eraser);

        cur = cur->next;
    }
}

/* draw all position of list on whitboard */
void drawPosList (Mat image, posNode **pos, int thk, int is_eraser) {
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
            plotHandwriting(image, p, plot, 0, thk, is_eraser);
            
        } else {
            plotHandwriting(image, p, plot, 1, thk, is_eraser);

            for (i = 0; i < DOTS; i++) {
                p[i] = (DPos) {-1, -1};
            }
        }
        cur = cur->next;
    }

}

void storeImage (pageNode *page, char *fileName) {
    Mat StorageTmp;

    StorageTmp = Mat(950, 1700, CV_8UC3, Scalar(255, 255, 255));

    drawStrokeList(StorageTmp, page->strokes);
    imwrite(fileName, StorageTmp(Rect(0, 50, 1600, 900)));
}
