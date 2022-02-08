#include "wbwindows.h"

using namespace cv;

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

    strcpy(input, "untitled");
    i = strlen(input);
    putText(mMat, input, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(5, 25, 2), 1, LINE_AA);

    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, mMat);
    while ((userKeyIn = pollKey()) != 13) {
        if (userKeyIn == 27) {
            break;
            /*\?*<":>*/
            //31 34 42 58 60 62 63 92
        } else if (userKeyIn == 127 && i > 0) {
            input[--i] = '\0';
        } else if (userKeyIn > 31 && userKeyIn != 92 && 
              userKeyIn != 63 && userKeyIn != 42 && 
              userKeyIn != 60 && userKeyIn != 34 && 
              userKeyIn != 58 && userKeyIn != 62 && userKeyIn != 127) {
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

    if (event == EVENT_LBUTTONDOWN) {
        if ((y >= 0 && y < 40)) {
            if ((x >= 0 && x < 40)) {
                /* eraser */
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
                /*delete*/

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

void drawAddPageIcon (Mat image, int x, int y, double Size) {
    double parts = Size/10;
    line(image, Point(x + (int) (parts), y + (int) (Size/2)), Point(x + (int) (parts*9), y + (int) (Size/2)), Scalar(0, 0, 0), 3);
    line(image, Point(x + (int) (Size/2), y + (int) (parts)), Point(x + (int) (Size/2), y + (int) (parts*9)), Scalar(0, 0, 0), 3);
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

void setfileExplorer () {
    Mat Manager;
    Rect list[10];
    Rect Scrollbar, scrollbar_thumb;
    fileNode **files;
    char save_path[256];
    int head = 0;
    //char *original_path = getenv("HOME");
    strcpy(save_path, getenv("HOME"));

    initFiles(&files);
    Manager = Mat(700, 915, CV_8UC3, Scalar(250, 250, 250));

    //setListView;
    for (int i = 0; i < 10; i++) {
        list[i] = Rect(0, i*40, 900, 40);
    }
    Scrollbar = Rect(900, 0, 15, 700);

    findPathFiles(files, save_path);
    int lenOfFiles = getFilesLen(files);
    int stepDist = (700.0/(lenOfFiles));

    if (lenOfFiles > 10) {
        scrollbar_thumb = Rect(902, 4, 12, stepDist*10);
    } else {
        scrollbar_thumb = Rect(902, 4, 12, 692);
    }
    showPathFiles(Manager, files, head, list, 10);

    Manager(Scrollbar) = Scalar(100, 100, 100);
    Manager(scrollbar_thumb) = Scalar(50, 50, 50);
    imshow("file Explorer", Manager);

    
    Exp_s sender = {files, &Manager, list, &Scrollbar, &scrollbar_thumb, 10, &head, stepDist};
    setMouseCallback("file Explorer", ExplorerEvent, (void *) &sender);
    while (pollKey() != 27) {
    }
    destroyWindow("file Explorer");
    //setMouseCallback("test", ExplorerEvent, NULL);
}

void showPathFiles (Mat image, fileNode **files, int head, Rect *list, int listLen) {
    int j = 0;
    fileNode *cur = *files;
    while (cur->next != NULL && j < head) {
        cur = cur->next;
        j++;
    }

    for (int i = 0; i < listLen && cur->next != NULL; i++) {
        if (i%2 == 0) {
            image(list[i]) = Scalar(240, 240, 240);
        } else {
            image(list[i]) = Scalar(200, 200, 200);
        }
        putText(image, cur->next->fileName, Point(list[i].x + 20, list[i].y + 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(5, 25, 2), 1, LINE_AA);
        cur = cur->next;
    }
}

void ExplorerEvent (int event, int x, int y, int flags, void *sender) {
    Exp_s s = *(Exp_s *) sender;
    fileNode **files = s.fileHead;
    int listlen = s.listlen;
    int stepDist = s.stepDist;
    int *head = s.head;
    static Point start;

    /*
    for (int i = 0; i < listlen; i++) {
        ListView[i] = s.ListView[i];
    }
    */

    /*
    Mat *drawMat;
    Rect *ListView;
    Rect *scrollbar;
    Rect *scrollbar_thumb;
    */
        for (int i = 0; i < listlen; i++) {
            if (s.ListView[i].contains(Point(x, y)) && (flags & EVENT_FLAG_LBUTTON)) {
                if (event == EVENT_LBUTTONDOWN) {

                    printf("clicked\n");
                } else if (event == EVENT_LBUTTONUP) {

                    printf("free\n");
                }
            }
        }
    
        if (s.scrollbar_thumb->contains(Point(x, y))) {
            if (event == EVENT_LBUTTONDOWN) {
                start = Point(x, y);
            } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) {
                /* 目前只能向下拉 */
                double nowDist = dist((DPos) {(double) start.x, (double) start.y}, (DPos) {(double) x, (double) y});
                //printf("%lf\n", nowDist);
                if (nowDist > stepDist) {
                    *head += (nowDist/stepDist);
                    start = Point(x, y);
                    (*s.drawMat)(*(s.scrollbar)) = Scalar(100, 100, 100);
                    *(s.scrollbar_thumb) = Rect(902, (4 + *head*stepDist), 12, stepDist*listlen);
                    (*s.drawMat)(*(s.scrollbar_thumb)) = Scalar(50, 50, 50);

                    showPathFiles (*s.drawMat, files, *head, s.ListView, listlen);
                    imshow("file Explorer", *s.drawMat);
                }
            }
        }
    /*

    if (event == EVENT_LBUTTONDOWN) {
        
        printf("clicked_original\n");
    } else if (event == EVENT_LBUTTONUP) {

    }
    */
}

void findPathFiles (fileNode **files, char *path) {
    DIR *dir;
    struct dirent *ptr;

    dir = opendir(path);
    while((ptr = readdir(dir)) != NULL)
    {
        addFile(files, ptr->d_name, ptr->d_type);

        //(ptr->d_type == DT_DIR)? printf("is d\n") : (ptr->d_type == DT_REG)? printf("is f\n") : (ptr->d_type == DT_LNK)? printf("is lnk\n") : printf("not three of these\n");
    }
    closedir(dir);
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
