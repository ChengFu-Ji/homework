#include "wbwindows.h"

using namespace cv;
extern int fd;

void getValue (int *value, int maxlength) {
    int i = 0;
    char input[maxlength+1], userKeyIn;
    char name[] = "valueIn";
    Mat mMat = Mat(40, ((maxlength+4)*25), CV_8UC3, Scalar(255, 255, 255));
    //Mat mMat = Mat(40, 500, CV_8UC3, Scalar(255, 255, 255));

    input[i] = '0';
    input[i+1] = '\0';
    putText(mMat, input, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(5, 25, 2), 1, LINE_AA);

    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, mMat);
    while ((userKeyIn = pollKey()) != 13) {
        if (userKeyIn == 27) {
            break;
        } if (userKeyIn > 47 && userKeyIn < 58 ) {
            input[(i < maxlength)? i++ : i-1] = userKeyIn;
            /*
            if (i < maxlength) {
                input[i++] = userKeyIn;
            } else if (i == maxlength) {
                input[i-1] = userKeyIn;
            }
            */
            input[i] = '\0';
        } else if (userKeyIn == 127) {
            input[--i] = '\0';
        }
        
        /* update window */
        if (userKeyIn != -1) {
            mMat.release();
            mMat = Mat(40, ((maxlength+4)*25), CV_8UC3, Scalar(255, 255, 255));

            putText(mMat, input, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(5, 25, 2), 1, LINE_AA);
            imshow(name, mMat);
        }
    }

    *value = atoi(input);
    destroyWindow(name);
}

void getString (char *string, int maxlength) {
    int i = 0;
    char input[maxlength+1], userKeyIn;
    char name[] = "input";
    Mat mMat = Mat(40, ((maxlength+4)*25), CV_8UC3, Scalar(255, 255, 255));

    if (strlen(string) == 0) {
        strcpy(input, "untitled");
    } else {
        strcpy(input, string);
    }
    i = strlen(input);
    putText(mMat, input, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(5, 25, 2), 1, LINE_AA);

    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, mMat);
    while ((userKeyIn = pollKey()) != 13) {
        if (userKeyIn == 27) {
            string[0] = 0;
            destroyWindow(name);

            return;
            /*\?*<":>*/
            //31 34 42 58 60 62 63 92
        } else if (userKeyIn == 127) {
            input[(i == 0)? i : --i] = '\0';
            if (i == 0) {
                
            }
        } else if (userKeyIn > 31 && userKeyIn != 92 && 
              userKeyIn != 63 && userKeyIn != 42 && 
              userKeyIn != 60 && userKeyIn != 34 && 
              userKeyIn != 58 && userKeyIn != 62) {
            input[(i < maxlength)? i++ : i-1] = userKeyIn;

            input[i] = '\0';
        }
        
        if (userKeyIn != -1) {
            mMat.release();
            mMat = Mat(40, ((maxlength+4)*25), CV_8UC3, Scalar(255, 255, 255));

            putText(mMat, input, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(5, 25, 2), 1, LINE_AA);
            imshow(name, mMat);
        }
    }

    strcpy(string, input);
    destroyWindow(name);
}

void setbuttonBar (struct _sender sender) {
    char barName[] = "buttonBar";
    Mat img = Mat(400, 80, CV_8UC3, Scalar(255, 255, 255));

    line(img, Point(img.cols/2, 0), Point(img.cols/2, img.rows), Scalar(0, 0, 0), 2); 
    for (int i = 40; i < img.rows; i+=40) {
        line(img, Point(0, i), Point(img.cols , i), Scalar(0, 0, 0), 2); 
    }

    namedWindow(barName, WINDOW_NORMAL);
    imshow(barName, img);

    setMouseCallback(barName, buttonBarEvent, (void *) &sender);
}

void buttonBarEvent (int event, int x, int y, int flags, void *sender) {
    struct _sender s = *(struct _sender *) sender;
    pageNode **pages = s.pageHead;
    int *curid = s.id;
    int *eraser = s.is_eraser;

    /*
    if (event == EVENT_LBUTTONDOWN) {
        if ((y >= 0 && y < 40)) {
            if ((x >= 0 && x < 40)) {
                if (*eraser) {
                    *eraser = 0;
                    printf("eraser off!!\n");
                } else {
                    *eraser = 1;
                    printf("eraser on!!\n");
                }
                
            } else if ((x >= 40 && x < 80)){
                int pageCount;
                getValue(&pageCount, 4);
                changeToPage(getPagebyOrder(pages, pageCount), curid);
                printf("get %d\n", pageCount);
            }
        } else if (y >= 40 && y < 80) {
            if ((x >= 0 && x < 40)) {
                changeToPage(getPrevPage(pages, *curid), curid);
                printf("prev\n");
            } else if ((x >= 40 && x < 80)){
                changeToPage(getPage(pages, *curid)->next, curid);
                printf("next\n");
            }
        } else if (y >= 80 && y < 120) {
            if ((x >= 0 && x < 40)) {
                pageNode *page = getLastPage(pages);
                addPage(pages, (page->pid)+1);
                printf("added\n");
            } else if ((x >= 40 && x < 80)){

                pageNode *page = getPrevPage(pages, *curid);
                if (page == NULL) {
                    page = getPage(pages, *curid)->next;
                }
                if (page != NULL) {
                    delPage(pages, *curid);
                    changeToPage(page, curid);
                }

                printf("delete\n");
            }
        }
    }
    */
}

void drawEraserIcon (Mat image, int x, int y, double Size) {
    /* parallelogram */
    int bias = 3;
    double parts = Size/10;
    Pos topPos = {x + (int) (parts*7) - bias, y + (int) parts};
    Pos leftPos = {x + (int) parts, y + (int) (parts*7) - bias};
    Pos rightPos = {x + (int) (parts*9), y + (int) (parts*3) + bias};
    Pos bottomPos = {x + (int) (parts*3) + bias, y + (int) (parts*9)};
    //printf("%d %d\n", topPos.x, topPos.y);

    line(image, Point(topPos.x, topPos.y), Point(leftPos.x, leftPos.y), Scalar(0, 0, 0), 3);
    line(image, Point(topPos.x, topPos.y), Point(rightPos.x, rightPos.y), Scalar(0, 0, 0), 3);
    line(image, Point(rightPos.x, rightPos.y), Point(bottomPos.x, bottomPos.y), Scalar(0, 0, 0), 3);
    line(image, Point(leftPos.x, leftPos.y), Point(bottomPos.x, bottomPos.y), Scalar(0, 0, 0), 3);

    line(image, Point(topPos.x - parts, topPos.y + parts + 5), Point(rightPos.x - parts - 2, rightPos.y + parts + 2), Scalar(0, 0, 0), 3);
}

void drawAddPageIcon (Mat image, int x, int y, double Size, Scalar color) {
    double parts = Size/10;
    line(image, Point(x + (int) (parts), y + (int) (Size/2)), Point(x + (int) (parts*9), y + (int) (Size/2)), color, 3);
    line(image, Point(x + (int) (Size/2), y + (int) (parts)), Point(x + (int) (Size/2), y + (int) (parts*9)), color, 3);
}

void drawDeletePageIcon (Mat image, int x, int y, double Size) {
    double parts = Size/10;
    line(image, Point(x + (int) (parts), y + (int) (Size/2)), Point(x + (int) (parts*9), y + (int) (Size/2)), Scalar(0, 0, 0), 3);
}

void drawSelectPageIcon (Mat image, int x, int y, double Size) {
    double parts = Size/10;
    putText(image, "page", Point(x + (int) (parts), y + (int) (parts)), FONT_HERSHEY_SIMPLEX, 1, Scalar(5, 25, 2), 2, LINE_AA);
}

void drawNextPageIcon (Mat image, int x, int y, double Size) {
    double parts = Size/10;
    line(image, Point(x + (int) (parts*3), y + (int) (parts*2)),
                Point(x + (int) (parts*3), y + (int) (parts*8)), Scalar(0, 0, 0), 3);
    line(image, Point(x + (int) (parts*3), y + (int) (parts*2)), 
                Point(x + (int) (parts*3*(1 + sqrt(3))), y + (int) (Size/2)), Scalar(0, 0, 0), 3);
    line(image, Point(x + (int) (parts*3), y + (int) (parts*8)),
                Point(x + (int) (parts*3*(1 + sqrt(3))), y + (int) (Size/2)), Scalar(0, 0, 0), 3);
}

void drawPrevPageIcon (Mat image, int x, int y, double Size) {
    double parts = Size/10;
    line(image, Point(x + (int) (parts*7), y + (int) (parts*2)),
                Point(x + (int) (parts*7), y + (int) (parts*8)), Scalar(0, 0, 0), 3);
    line(image, Point(x + (int) (parts*7), y + (int) (parts*2)), 
                Point(x + (int) (parts*(7 - 3*sqrt(3))), y + (int) (Size/2)), Scalar(0, 0, 0), 3);
    line(image, Point(x + (int) (parts*7), y + (int) (parts*8)),
                Point(x + (int) (parts*(7 - 3*sqrt(3))), y + (int) (Size/2)), Scalar(0, 0, 0), 3);
}

void drawLoadFileIcon (Mat image, int x, int y, double Size) {
    int icon_thk = 3;
    int bias = 2;
    double parts = (Size - 2*bias)/10;

    line(image, Point(x + bias + (int) (parts*2), y + bias),
                Point(x + bias + (int) (parts*6), y + bias), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*2), y + bias),
                Point(x + bias + (int) (parts*2), y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*6), y + bias),
                Point(x + bias + (int) (parts*8), y + bias + (int) (parts*2)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*8), y + bias + (int) (parts*2)),
                Point(x + bias + (int) (parts*8), y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*8), y + bias + (int) (parts*10)),
                Point(x + bias + (int) (parts*2), y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*7), y + bias + (int) (parts*8)),
                Point(x + bias + (int) (parts*3), y + bias + (int) (parts*8)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*7), y + bias + (int) (parts*6)),
                Point(x + bias + (int) (parts*3), y + bias + (int) (parts*6)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*7), y + bias + (int) (parts*4)),
                Point(x + bias + (int) (parts*3), y + bias + (int) (parts*4)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*6), y + bias + (int) (parts*2)),
                Point(x + bias + (int) (parts*3), y + bias + (int) (parts*2)), Scalar(0, 0, 0), icon_thk);

    circle(image, Point(x + bias + (int) (parts*2), y + bias + (int) (parts)), 15, Scalar(0, 0, 0), -1);
    drawAddPageIcon(image, x + bias*2 + 1, y - bias, 20, Scalar(100, 100, 100));
}

void drawSaveFileIcon (Mat image, int x, int y, double Size) {
    int icon_thk = 3;
    int bias = 2;
    double parts = (Size - 2*bias)/10;

    line(image, Point(x + bias, y + bias),
                Point(x + bias + (int) (parts*8), y + bias), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias, y + bias),
                Point(x + bias, y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*8), y + bias),
                Point(x + bias + (int) (parts*10), y + bias + (int) (parts*2)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*10), y + bias + (int) (parts*2)),
                Point(x + bias + (int) (parts*10), y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*10), y + bias + (int) (parts*10)),
                Point(x + bias , y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);


    line(image, Point(x + bias + (int) (parts*2.5), y + bias), 
                Point(x + bias + (int) (parts*2.5), y + bias + (int) (parts*3)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*7.2), y + bias), 
                Point(x + bias + (int) (parts*7.2), y + bias + (int) (parts*3)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*2.5), y + bias + (int) (parts*3)), 
                Point(x + bias + (int) (parts*7.2), y + bias + (int) (parts*3)), Scalar(0, 0, 0), icon_thk);


    line(image, Point(x + bias + (int) (parts*2), y + bias + (int) (parts*10)), 
                Point(x + bias + (int) (parts*2), y + bias + (int) (parts*5)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*8), y + bias + (int) (parts*10)), 
                Point(x + bias + (int) (parts*8), y + bias + (int) (parts*5)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*8), y + bias + (int) (parts*5)), 
                Point(x + bias + (int) (parts*2), y + bias + (int) (parts*5)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*6.5), y + bias + (int) (parts*6)), 
                Point(x + bias + (int) (parts*3.5), y + bias + (int) (parts*6)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*6.5), y + bias + (int) (parts*7)), 
                Point(x + bias + (int) (parts*3.5), y + bias + (int) (parts*7)), Scalar(0, 0, 0), icon_thk);

}

void drawChangeThicknessIcon (Mat image, int x, int y, double Size) {
    int icon_thk = 3;
    int bias = 2;
    double parts = (Size - 2*bias)/10;

    line(image, Point(x + bias + (int) (parts*5), y + bias), 
                Point(x + bias + (int) (parts*4), y + bias + (int) (parts*2)), Scalar(0, 0, 0), icon_thk-1);

    line(image, Point(x + bias + (int) (parts*5), y + bias), 
                Point(x + bias + (int) (parts*6), y + bias + (int) (parts*2)), Scalar(0, 0, 0), icon_thk-1);

    line(image, Point(x + bias + (int) (parts*6), y + bias + (int) (parts*2)), 
                Point(x + bias + (int) (parts*4), y + bias + (int) (parts*2)), Scalar(0, 0, 0), icon_thk-1);

    line(image, Point(x + bias + (int) (parts*6), y + bias + (int) (parts*2)), 
                Point(x + bias + (int) (parts*6), y + bias + (int) (parts*3)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*4), y + bias + (int) (parts*2)), 
                Point(x + bias + (int) (parts*4), y + bias + (int) (parts*3)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*4), y + bias + (int) (parts*3)), 
                Point(x + bias + (int) (parts*3.5), y + bias + (int) (parts*5)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*6), y + bias + (int) (parts*3)), 
                Point(x + bias + (int) (parts*6.5), y + bias + (int) (parts*5)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*3.5), y + bias + (int) (parts*5)), 
                Point(x + bias + (int) (parts*3.5), y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*6.5), y + bias + (int) (parts*5)), 
                Point(x + bias + (int) (parts*6.5), y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*3.5), y + bias + (int) (parts*10)), 
                Point(x + bias + (int) (parts*6.5), y + bias + (int) (parts*10)), Scalar(0, 0, 0), icon_thk);

    line(image, Point(x + bias + (int) (parts*3.5), y + bias + (int) (parts*5)), 
                Point(x + bias + (int) (parts*6.5), y + bias + (int) (parts*5)), Scalar(0, 0, 0), icon_thk);

    rectangle(image, Rect(x + bias + (int) (parts*3.5), y + bias + (int) (parts*5), parts*3, parts*5), Scalar(0, 0, 0), -1);

    line(image, Point(x + bias + (int) (parts*4), y + bias + (int) (parts*5.5)), 
                Point(x + bias + (int) (parts*4), y + bias + (int) (parts*9.5)), Scalar(70, 70, 70), icon_thk);
}

void setfileExplorer (pthread_t *thread, int fd, struct _sender recv, int state) {
    Mat Manager;
    Rect list[10];
    Rect Scrollbar, scrollbar_thumb;
    Rect button;
    fileNode **files;
    char filestate[2][8] = {"save", "load"};
    char path[256];
    char windowName[] = "file Explorer";
;
    int head = 0;
    
    //char *original_path = getenv("HOME");
    strcpy(path, getenv("HOME"));

    initFiles(&files);
    Manager = Mat(700, 925, CV_8UC3, Scalar(57, 50, 47));

    //setListView;
    for (int i = 1; i <= 10; i++) {
        list[i-1] = Rect(0, i*40, 900, 40);
    }
    Scrollbar = Rect(900, 40, 25, 400);

    findPathFiles(files, path);
    int lenOfFiles = getFilesLen(files);
    //scrollbar_thumb = Rect(902, 44, 22, 392);
    int stepDist = setScrollbarHeight(files, &scrollbar_thumb, 10);

    showPathFiles(Manager, files, head, list, 10);

    Manager(Scrollbar) = Scalar(140, 119, 110);
    Manager(scrollbar_thumb) = Scalar(90, 92, 90);
    
    updatePath (Manager, path);

    button = Rect(700, 600, 80, 30);
    setButton(Manager, button, filestate[state]);

    imshow(windowName, Manager);

    
    Exp_s sender = {recv.pageHead, files, &Manager, list, &Scrollbar, &scrollbar_thumb, &button, 10, state, &head, &stepDist, recv.id, path, windowName};
    setMouseCallback(windowName, ExplorerEvent, (void *) &sender);
    while (pollKey() != 27 && cvGetWindowHandle(windowName) != 0) {
        if (fd > 0) {
            if (pthread_kill(*thread, 0) == ESRCH) {
                //struct _sender recv = {pages, recvList, NULL, NULL, &curid};
                pthread_create(thread, NULL, recvData, (void *) &recv);
                //imshow(Winn, image);
            }
        }
    }
    destroyWindow(windowName);
    //setMouseCallback("test", ExplorerEvent, NULL);
}

void showPathFiles (Mat image, fileNode **files, int head, Rect *list, int listLen) {
    int j = 0;
    fileNode *cur = *files;
    int lenOfFiles = getFilesLen(files);

    while (cur->next != NULL && j < head && (lenOfFiles - j) > 10) {
        cur = cur->next;
        j++;
    }

    for (int i = 0; i < listLen && cur->next != NULL; i++) {
        if (i%2 == 0) {
            image(list[i]) = Scalar(164, 147, 130);
        } else {
            image(list[i]) = Scalar(170, 156, 140);
        }
        if (cur->next->fileType == DT_REG) {
            putText(image, cur->next->fileName, Point(list[i].x + 20, list[i].y + 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(15, 5, 12), 1, LINE_AA);
        } else {
            putText(image, cur->next->fileName, Point(list[i].x + 20, list[i].y + 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(94, 94, 0), 1, LINE_AA);
        }
        cur = cur->next;
    }
}

void ExplorerEvent (int event, int x, int y, int flags, void *sender) {
    Exp_s s = *(Exp_s *) sender;
    fileNode **files = s.fileHead;
    int listlen = s.listlen;
    int *stepDist = s.stepDist;
    int *head = s.head;
    char state[2][8] = {"save", "load"};
    static Point start = Point(s.scrollbar_thumb->x, s.scrollbar_thumb->y);
    static struct timespec firstClick = {0, 0};
    static int clickList;
    static char filename[256];

    /*
    Mat *drawMat;
    Rect *ListView;
    Rect *scrollbar;
    Rect *scrollbar_thumb;
    */
    if (s.button->contains(Point(x, y))) {
        if (event == EVENT_LBUTTONDOWN) {
            (*s.drawMat)(*s.button) = Scalar(50, 70, 30);
            putText(*s.drawMat, state[s.state], Point(s.button->x + 12, s.button->y + 20), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 250), 1, LINE_AA);
            imshow(s.winn, *s.drawMat);
        } else if (event == EVENT_LBUTTONUP) {
            setButton(*(s.drawMat), *(s.button), state[s.state]);
            imshow(s.winn, *s.drawMat);

            if (!s.state) {
                getString(filename, 40);
                if (strlen(filename) != 0) {
                    char fileType[3][6] = {".jpg", ".tiff", ".png"};
                    char *fptr;
                    for (int i = 0; i < 3; i++, (i==3)? strcat(filename, fileType[0]): 0) {
                        if ((fptr = strstr(filename, fileType[i])) != NULL) {
                            if (!strcmp(fptr, fileType[i])) {
                                break;
                            }
                        }
                    }
                    strcat(s.path, "/");
                    strcat(s.path, filename);
                    if (fd < 0) {
                        storeImage(getPage(s.pageHead, *s.id, PAGE_PERSONAL), s.path);
                    } else {
                        storeImage(getPage(s.pageHead, *s.id, PAGE_COMMON), s.path);
                    }
                    destroyWindow(s.winn);
                }
            } else {
                printf("in\n");
                char fileType[][6] = {".jpg", ".tiff", ".png", ".gif", ".jpeg"};
                char *fptr;
                for (int i = 0; i < 5; i++) {
                    if ((fptr = strstr(filename, fileType[i])) != NULL) {
                        if (!strcmp(fptr, fileType[i])) {
                            strcat(s.path, "/");
                            strcat(s.path, filename);

                            if (fd < 0) {
                                readImage(getPage(s.pageHead, *s.id, PAGE_PERSONAL), s.path);
                                reloadPage(getPage(s.pageHead, *s.id, PAGE_PERSONAL));
                            } else {
                                readImage(getPage(s.pageHead, *s.id, PAGE_COMMON), s.path);
                                reloadPage(getPage(s.pageHead, *s.id, PAGE_COMMON));
                            }
                            destroyWindow(s.winn);
                            break;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < listlen; i++) {
        if (s.ListView[i].contains(Point(x, y))) {
            if (event == EVENT_LBUTTONDOWN) {
                fileNode *file = getFilesbyOrder(files, *head + i);
                if (file != NULL) {
                    if (file->fileType == DT_REG) {
                        strcpy(filename, file->fileName);
                    }
                }
                if (firstClick.tv_sec == 0 && firstClick.tv_nsec == 0) {
                    clock_gettime(CLOCK_REALTIME, &firstClick); 
                    clickList = i;
                } else {
                    struct timespec secondClick;
                    clock_gettime(CLOCK_REALTIME, &secondClick); 

                    if (clickList != i) {
                        clickList = i;
                        memcpy(&firstClick, &secondClick, sizeof(struct timespec));
                        break;
                    }

                    long dclick_maxtime = 3 * pow(10, 8);
                    long long user_dclickTime;
                    time_t secDiff = secondClick.tv_sec - firstClick.tv_sec;
                    long nsecDiff = secondClick.tv_nsec - firstClick.tv_nsec;
                    user_dclickTime = (secDiff > 0 && nsecDiff < 0)? secDiff * pow(10, 9) + nsecDiff : (secDiff <= 0)? nsecDiff : dclick_maxtime;

                    if (user_dclickTime < dclick_maxtime) {
                        //file = getFilesbyOrder(files, *head + i);
                        if (file != NULL) {
                            if (file->fileType == DT_REG) {
                                strcpy(filename, file->fileName);
                            }
                            //printf("file->fileType %d\n", file->fileType);
                            if (file->fileType == DT_DIR || file->fileType == DT_LNK || !strcmp(file->fileName, "..")) {
                                //printf("*head %d\n", *head);
                                s.drawMat->release();
                                *s.drawMat = Mat(700, 925, CV_8UC3, Scalar(57, 50, 47));
                                if (strcmp(file->fileName, "..")) {
                                    if (strcmp(s.path, "/")) {
                                        strcat(s.path, "/");
                                    }
                                    strcat(s.path, file->fileName);
                                } else {
                                    for (int i = strlen(s.path)-1; i >= 0; i--) {
                                        if (s.path[i] == '/') {
                                            s.path[i] = '\0';
                                            if (i == 0) {
                                                strcat(s.path, "/");
                                            }
                                            break;
                                        }
                                    }
                                }

                                deleteAllFiles(files);
                                updatePath(*(s.drawMat), s.path);
                                findPathFiles(files, s.path);
                                showPathFiles(*(s.drawMat), files, (*head = 0), s.ListView, listlen);
                                setButton(*(s.drawMat), *(s.button), state[s.state]);
                                *stepDist = setScrollbarHeight(files, s.scrollbar_thumb, 10);

                                (*s.drawMat)(*(s.scrollbar)) = Scalar(140, 119, 110);
                                (*s.drawMat)(*(s.scrollbar_thumb)) = Scalar(90, 92, 90);
                                imshow(s.winn, *s.drawMat);
                            }
                        }
                    }

                    memcpy(&firstClick, &secondClick, sizeof(struct timespec));
                }
            } 
        }
    }

    if ((s.scrollbar->contains(Point(x, y)) || s.scrollbar_thumb->contains(Point(x, y))) && getFilesLen(files) > 10) {
        if (event == EVENT_LBUTTONDOWN) {
            if (!s.scrollbar_thumb->contains(Point(x, y))) {
                moveScrollbarThumb(start, x, y, s);
            }
            start = Point(x, y);
        } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) {
            moveScrollbarThumb(start, x, y, s);
            start = Point(x, y);
        } else if (event == EVENT_LBUTTONUP) {
            (*s.drawMat)(*(s.scrollbar_thumb)) = Scalar(90, 92, 90);
        }

        showPathFiles (*s.drawMat, files, *head, s.ListView, listlen);
        imshow(s.winn, *s.drawMat);
    }
    if (!s.scrollbar_thumb->contains(Point(x, y)) && (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))) {
        (*s.drawMat)(*(s.scrollbar_thumb)) = Scalar(90, 92, 90);
        imshow(s.winn, *s.drawMat);
    }

}

void setButton (Mat image, Rect button, char *state) {
    image(button) = Scalar(60, 120, 20);
    putText(image, state, Point(button.x + 12, button.y + 20), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 250), 1, LINE_AA);
}

void moveScrollbarThumb (Point start, int x, int y, Exp_s s) {
    int *stepDist = s.stepDist;
    int *head = s.head;
    int listlen = s.listlen;

    double nowDist = dist((DPos) {(double) start.x, (double) start.y}, (DPos) {(double) x, (double) y});

    int moveDist = (start.y < y)? (int) (nowDist/(*stepDist)) : -((int) (nowDist/(*stepDist)));

    if (s.scrollbar_thumb->y + s.scrollbar_thumb->height < s.scrollbar->y + s.scrollbar->height &&
            (s.scrollbar_thumb->y > s.scrollbar->y)) {
        *head += moveDist; 
        while (*head < 0) {
            (*head)++;
        }

        *(s.scrollbar_thumb) = Rect(902, (44 + *head*(*stepDist)), 22, (*stepDist)*listlen);
        while (!s.scrollbar_thumb->contains(Point(s.scrollbar_thumb->x, y))) {
            *head += (s.scrollbar_thumb->y + (s.scrollbar_thumb->height/2) < y)? 1 : -1; 
            *(s.scrollbar_thumb) = Rect(902, (44 + *head*(*stepDist)), 22, (*stepDist)*listlen);
        }

        /* out of bar */
        while (s.scrollbar_thumb->y + s.scrollbar_thumb->height > s.scrollbar->y + s.scrollbar->height || 
                (s.scrollbar_thumb->y < s.scrollbar->y)) {
            *head += (s.scrollbar_thumb->y < s.scrollbar->y)? 1 : -1; 
            *(s.scrollbar_thumb) = Rect(902, (44 + *head*(*stepDist)), 22, (*stepDist)*listlen);
        }

        (*s.drawMat)(*(s.scrollbar)) = Scalar(140, 119, 110);
        (*s.drawMat)(*(s.scrollbar_thumb)) = Scalar(50, 52, 50);
    }
}

void findPathFiles (fileNode **files, char *path) {
    DIR *dir;
    struct dirent *ptr;

    dir = opendir(path);
    /*
    DT_DIR
    DT_REG
    DT_LINK
    */
    while ((ptr = readdir(dir)) != NULL) {        
        if (strcmp(ptr->d_name, ".")) {
            addFile(files, ptr->d_name, ptr->d_type);
        }
    }
    closedir(dir);
}


void updatePath (Mat image, char *path) {
    char curPath[272];
    strcpy(curPath, "current path: ");
    strcat(curPath, path);
    putText(image, curPath, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(235, 235, 162), 1, LINE_AA);
}

int setScrollbarHeight (fileNode **files, Rect *scrollbar_thumb, int listlen) {
    int lenOfFiles = getFilesLen(files);
    int stepDist = (400.0/(lenOfFiles));

    if (lenOfFiles > listlen) {
        *scrollbar_thumb = Rect(902, 44, 22, stepDist*listlen);
    } else {
        *scrollbar_thumb = Rect(902, 44, 22, 392);
    }
    return stepDist;
}


void readImage (pageNode *page, char *fileName) {
    strcpy(page->fileName, fileName);
    deleteAllStrokes(page->strokes);
}

/*
#ifdef __COLORFUL_WHITEBOARD__
void setColorSelector (struct _sender sender) {
    char barName[] = "buttonBar";
    static char channel[3] = {'r', 'g', 'b'};

    if (cvGetWindowHandle(colorPickerWinn) == 0) {
        namedWindow(colorPickerWinn, WINDOW_NORMAL | WINDOW_GUI_NORMAL);
        //imshow(colorPickerWinn, Mat(100, 100, CV_8UC3, color));
        createTrackbar("R", colorPickerWinn, 0, 255, trackcolorSelector, (void *) &channel[0]);
        createTrackbar("G", colorPickerWinn, 0, 255, trackcolorSelector, (void *) &channel[1]);
        createTrackbar("B", colorPickerWinn, 0, 255, trackcolorSelector, (void *) &channel[2]);
        //createTrackbar("thickness", "button", 0, 999, trackThicknessPicker, NULL);
    }
}
#endif
*/


#ifdef __MISC_WINDOW_TEST__
int main() {
    ssize_t a = 0;

    getValue(&a, 4);
    //setbuttonBar();

    printf("get %zd\n", a);
    return 0;
}

#endif
