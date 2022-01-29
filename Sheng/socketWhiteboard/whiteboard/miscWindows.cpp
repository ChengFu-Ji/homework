#include "wbwindows.h"

using namespace cv;

void getValue (int *value, int maxlength) {
    int i = 0;
    char input[maxlength+1], tmp;
    char name[] = "valueIn";
    Mat mMat = Mat(40, ((maxlength+4)*25), CV_8UC3, Scalar(255, 255, 255));
    //Mat mMat = Mat(40, 500, CV_8UC3, Scalar(255, 255, 255));

    input[i] = '0';
    input[i+1] = '\0';
    putText(mMat, input, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(5, 25, 2), 1, LINE_AA);

    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, mMat);
    while ((tmp = pollKey()) != 13) {
        if (tmp > 47 && tmp < 58 && i < maxlength) {
            input[i++] = tmp;
            input[i] = '\0';
        } else if (tmp > 47 && tmp < 58 && i == maxlength) {
            input[i-1] = tmp;
            input[i] = '\0';
        } else if (tmp == 127) {
            input[--i] = '\0';
        }
        
        /* update window */
        if (tmp != -1) {
            mMat.release();
            mMat = Mat(40, ((maxlength+4)*25), CV_8UC3, Scalar(255, 255, 255));

            putText(mMat, input, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(5, 25, 2), 1, LINE_AA);
            imshow(name, mMat);
        }
    }

    *value = atoi(input);
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

    waitKey(0);
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
                    delCurrentPage(pages, *curid);
                    changeToPage(page, curid);
                }

                printf("delete\n");
            }
        }
    }
}

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


#ifdef __MISC_WINDOW_TEST__
int main() {
    ssize_t a = 0;

    getValue(&a, 4);
    //setbuttonBar();

    printf("get %zd\n", a);
    return 0;
}

#endif
