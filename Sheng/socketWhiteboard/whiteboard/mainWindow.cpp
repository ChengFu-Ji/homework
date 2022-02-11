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
Rect topButton, buttonBar, statusBar;
int fd, winnWidth, winnHeight, fdTmp = -1;

int main () {
    pthread_t recvThread;
    posNode **recvList, **sendList;
    pageNode **pages;
    int curid, is_eraser, thk;

    char host[] = "localhost";
    int port = 2244;

    winnWidth = 1700;
    winnHeight = 990;

    initPage(&pages);
    thk = 3;
    is_eraser = 0;
    curid = 0;

    image = Mat(winnHeight, winnWidth, CV_8UC3, Scalar(255, 255, 255));

    topButton = Rect(0, 0, image.cols, 50);
    statusBar = Rect(0, image.rows-40, image.cols, 40);
    buttonBar = Rect(image.cols-100, topButton.height, 100, image.rows-topButton.height-statusBar.height);

    namedWindow(Winn, WINDOW_AUTOSIZE);

    image(topButton) = Scalar(200,200,200);
    setActionBar();
    setStatusBar(pages, curid, thk);
    imshow(Winn, image);

    initPos(&sendList);
    mainMouseCallback_s s = {pages, recvList, sendList, &recvThread, &thk, &is_eraser, &curid};
    setMouseCallback(Winn, mouseOnWhiteboard, (void *) &s);
    //setbuttonBar(s);

    addPage(pages, curid, PAGE_PERSONAL);
    if ((fd = connectToServer(host, port)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", host, port);
    } else {
        initPos(&recvList);
        
        struct _sender recv = {pages, recvList, NULL, NULL, &curid};
        pthread_create(&recvThread, NULL, recvData, (void *) &recv);
        addPage(pages, curid, PAGE_COMMON);
    }

    //setfileExplorer();
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
            for (int i = 0; i < 1000; i++) {
                if (fd < 0) {
                    pageNode *page = getLastPage(pages, PAGE_PERSONAL);
                    addPage(pages, (page->pid)+1, PAGE_PERSONAL);
                } else {
                    pageNode *page = getLastPage(pages, PAGE_COMMON);
                    addPage(pages, (page->pid)+1, PAGE_COMMON);

                    if (fdTmp > 0) {
                        SockCond addpage = {-1, (page->pid)+1, -1, 0, 0, 0};
                        write(fd, &addpage, sizeof(SockCond));
                    }
                }
            }

            printf("added\n");
        } else if (userKeyIn == 24) {
            if (fd < 0) {
                changeToPage(getPage(pages, curid, PAGE_PERSONAL)->next, &curid);
            } else {
                changeToPage(getPage(pages, curid, PAGE_COMMON)->next, &curid);
            }
            printf("next page\n");
        } else if (userKeyIn == 26) {
            if (fd < 0) {
                changeToPage(getPage(pages, curid, PAGE_PERSONAL), &curid);
            } else {
                changeToPage(getPage(pages, curid, PAGE_COMMON), &curid);
            }
            printf("prev page\n");
        } else if (userKeyIn == 4) {
            /*
            pageNode *page = getPrevPage(pages, curid);
            if (page == NULL) {
                page = getPage(pages, curid)->next;
            }
            if (page != NULL) {
                if (fd >= 0) {
                    SockCond delpage = {-1, -1, curid, 0, 0, 0};
                    write(fd, &delpage, sizeof(SockCond));
                }

                delPage(pages, curid);
                changeToPage(page, &curid);

            }
            */

            printf("delete\n");
        } else if (userKeyIn == 25) {
            getValue(&thk, 4);
            printf("thk now %d\n", thk);
        } else if (userKeyIn == 23) {
            struct _sender recv = {pages, recvList, NULL, NULL, &curid};
            /*load */
            setfileExplorer(&recvThread, fd, recv, 1);
        } else if (userKeyIn == 22) {
            struct _sender recv = {pages, recvList, NULL, NULL, &curid};
            /*save*/
            setfileExplorer(&recvThread, fd, recv, 0);
        }

        if (fdTmp > 0) {
            if (pthread_kill(recvThread, 0) == ESRCH) {
                struct _sender recv = {pages, recvList, NULL, NULL, &curid};
                pthread_create(&recvThread, NULL, recvData, (void *) &recv);
                //imshow(Winn, image);
            }
        }

    }

    deleteAllPages(pages);

    freePos(sendList);
    freePage(pages);

    if (fdTmp > 0) {
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
        addPage(pages, recv.addpid, PAGE_COMMON);
    } 
    if (recv.poslen > 0) {
        if ((n = socket_read(fd, recvList, recv.poslen)) > 0) {
            if (recv.drawpid == *curid) {
                drawPosList(image, recvList, recv.thickness, recv.eraser);
                imshow(Winn, image);
                //waitKey(1);
                //IDdelete(recvList, id);
                //deleteAllPos(recvList);
            }
            pageNode *page = getPage(pages, recv.drawpid, PAGE_COMMON);
            if (page != NULL) {
                addStroke(page->strokes, recvList, recv.thickness, recv.eraser);
                (*recvList)->next = NULL;
            }
        }
    } 

    if (recv.deletepid >= 0) {
        pageNode *page = getPrevPage(pages, recv.deletepid, PAGE_COMMON);
        if (page == NULL) {
            page = getPage(pages, recv.deletepid, PAGE_COMMON)->next;
        }
        if (page != NULL) {
            delPage(pages, recv.deletepid, PAGE_COMMON);
            if (*curid == recv.deletepid) {
                changeToPage(page, curid);
            }
        }
    }
    pthread_exit (0);
}

void mouseOnWhiteboard (int event, int x, int y, int flags, void *sender) {
    static DPos p[DOTS] = { [0 ... DOTS-1]{-1.0, -1.0}};
    static int dots = 0;

    DPos plot[TIMES];
    int i = 0, j = 0;
    double distance = 0.0;

    mainMouseCallback_s s = *(mainMouseCallback_s *) sender;
    pageNode **pages = s.pageHead;
    posNode **sendList = s.sendPosHead;
    posNode **recvList = s.recvPosHead;
    int *curid = s.id;
    int *eraser = s.is_eraser;
    int *thk = s.thk;


    image(topButton) = Scalar(200,200,200);
    if (topButton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
        image(topButton) = Scalar(100, 100, 100);
    }

    if (event == EVENT_LBUTTONDOWN) {
        if (topButton.contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
            if (fd > 0) {
                fdTmp = fd;
                fd = -1;

                pageNode *cur = *pages;
                while (cur->next != NULL) {
                    if (cur->next->status == PAGE_PERSONAL) { 
                        *curid = (cur->next->pid);
                        reloadPage(cur->next);
                        break;
                    }
                    cur = cur->next;
                }
            } else if (fdTmp != -1) {
                fd = fdTmp;

                pageNode *cur = *pages;
                while (cur->next != NULL) {
                    if (cur->next->status == PAGE_COMMON) { 
                        *curid = (cur->next->pid);
                        reloadPage(cur->next);
                        break;
                    }
                    cur = cur->next;
                }
            }

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
                getValue(&pageCount, 9);
                if (fd < 0) {
                    changeToPage(getPagebyOrder(pages, pageCount, PAGE_PERSONAL), curid);
                } else {
                    changeToPage(getPagebyOrder(pages, pageCount, PAGE_COMMON), curid);
                }
                printf("change to page %d\n", pageCount);
            } else if (y < 350) {
                if (fd < 0) {
                    changeToPage(getPrevPage(pages, *curid, PAGE_PERSONAL), curid);
                } else {
                    changeToPage(getPrevPage(pages, *curid, PAGE_COMMON), curid);
                }
                printf("prev\n");
            } else if (y < 450) {
                if (fd < 0) {
                    changeToPage(getNextPage(pages, *curid, PAGE_PERSONAL), curid);
                } else {
                    changeToPage(getNextPage(pages, *curid, PAGE_COMMON), curid);
                }
                printf("next\n");
            } else if (y < 550) {
                if (fd >0) {
                    pageNode *page = getLastPage(pages, PAGE_COMMON);
                    addPage(pages, (page->pid)+1, PAGE_COMMON);
                    SockCond addpage = {-1, (page->pid)+1, -1, 0, 0, 0};
                    write(fd, &addpage, sizeof(SockCond));
                } else {
                    pageNode *page = getLastPage(pages, PAGE_PERSONAL);
                    addPage(pages, (page->pid)+1, PAGE_PERSONAL);
                }
                printf("added\n");
            } else if (y < 650) {
                pageNode *page = getPrevPage(pages, *curid, PAGE_COMMON);
                if (page == NULL) {
                    page = getPage(pages, *curid, PAGE_COMMON)->next;
                }
                if (fd < 0) {
                    pageNode *page = getPrevPage(pages, *curid, PAGE_PERSONAL);
                    if (page == NULL) {
                        page = getPage(pages, *curid, PAGE_PERSONAL)->next;
                    }
                }

                if (page != NULL) {
                    if (fd < 0) {
                        delPage(pages, *curid, PAGE_PERSONAL);
                        changeToPage(page, curid);
                    } else {
                        delPage(pages, *curid, PAGE_COMMON);
                        changeToPage(page, curid);
                    }
                    printf("delete\n");
                }
            } else if (y < 750) {
                getValue(thk, 4);
                //printf("thk now %d\n", thk);
            } else if (y < 850) {
                struct _sender recv = {pages, recvList, NULL, NULL, curid};
                /*load */
                setfileExplorer(s.thread, fd, recv, 1);
            } else if (y < 950) {
                struct _sender recv = {pages, recvList, NULL, NULL, curid};
                /*save*/
                setfileExplorer(s.thread, fd, recv, 0);
            }
            //setActionBar();
        } else if(p[0].x == -1 && p[0].y == -1) {
            p[0] = (DPos) {(double) x, (double) y};

            if (fd > 0) {
                dots++;
            }
            addPos(sendList, (Pos) {x, y});
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
        //imshow(Winn, image);
    } else if (event == EVENT_LBUTTONUP && p[0].x != -1 && p[0].y != -1) {

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

        pageNode *page = getPage(pages, *curid, PAGE_COMMON);
        if (fd < 0) {
            page = getPage(pages, *curid, PAGE_PERSONAL);
        }
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
        //imshow(Winn, image);
    }

    setStatusBar(pages, *curid, *thk);
    imshow(Winn, image);
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

    drawEraserIcon(image, 1600, 50, 100);
    drawSelectPageIcon(image, 1600, 200, 100);
    drawPrevPageIcon(image, 1600, 250, 100);
    drawNextPageIcon(image, 1600, 350, 100);
    drawAddPageIcon(image, 1600, 450, 100, Scalar(0, 0, 0));
    drawDeletePageIcon(image, 1600, 550, 100);


    drawChangeThicknessIcon(image, 1615, 665, 70);
    drawLoadFileIcon(image, 1615, 765, 70);
    drawSaveFileIcon(image, 1615, 865, 70);
}

void setStatusBar (pageNode **pages, int curid, int thk) {
    char Msg[200];

    if (fd < 0) {
        sprintf(Msg, "Personal Page page: %d/%d  thickness: %d", getPagesOrder(pages, curid, (fd < 0)? PAGE_PERSONAL : PAGE_COMMON), getPagesLen(pages, (fd < 0)? PAGE_PERSONAL : PAGE_COMMON), thk);
    } else {
        sprintf(Msg, "Common Page page: %d/%d  thickness: %d", getPagesOrder(pages, curid, (fd < 0)? PAGE_PERSONAL : PAGE_COMMON), getPagesLen(pages, (fd < 0)? PAGE_PERSONAL : PAGE_COMMON), thk);
    }
    image(statusBar) = Scalar(177,157,145);
    
    putText(image, Msg, Point(statusBar.x + 12, statusBar.y + 30), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(250, 250, 180), 1, LINE_AA);
}

void changeToPage (pageNode *page, int *curid) {
    if (page == NULL) {
        return;
    }

    image.release();
    image = Mat(winnHeight, winnWidth, CV_8UC3, Scalar(255, 255, 255));
    if (page->fileName[0] != 0) {
        Mat loadTmp;
        resize(imread(page->fileName), loadTmp, Size(1600, 900), 0, 0, INTER_AREA);

        Mat detTmp = image(Rect(0, 50, 1600, 900));
        loadTmp.copyTo(detTmp);
        //resize(loadTmp, image, Size(winnHeight, winnWidth), 0, 0, INTER_AREA);
    }

    drawStrokeList(image, page->strokes);
    image(topButton) = Scalar(200,200,200);
    setActionBar();
    imshow(Winn, image);
    *curid = page->pid;
}

void reloadPage (pageNode *page) {
    image.release();

    image = Mat(winnHeight, winnWidth, CV_8UC3, Scalar(255, 255, 255));
    if (page->fileName[0] != 0) {
        Mat loadTmp;
        resize(imread(page->fileName), loadTmp, Size(1600, 900), 0, 0, INTER_AREA);

        Mat detTmp = image(Rect(0, 50, 1600, 900));
        loadTmp.copyTo(detTmp);
    }

    drawStrokeList(image, page->strokes);
    image(topButton) = Scalar(200,200,200);
    setActionBar();
    imshow(Winn, image);
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

            /*
            for (i = 0; i < DOTS; i++) {
                p[i] = (DPos) {-1, -1};
            }
            */
        }
        cur = cur->next;
    }

}

void storeImage (pageNode *page, char *fileName) {
    Mat StorageTmp;

    StorageTmp = Mat(winnHeight, winnWidth, CV_8UC3, Scalar(255, 255, 255));

    drawStrokeList(StorageTmp, page->strokes);
    imwrite(fileName, StorageTmp(Rect(0, 50, 1600, 900)));
}


