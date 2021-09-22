#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;


void onMouse (int, int, int, int, void*);

char windowName[] = "Display Image";
char imageName[] = "background.png";
Mat image;


int main() {

    image = imread(imageName, 1);

    namedWindow(windowName, WINDOW_NORMAL);
    /*  WINDOW_NORMAL: 0x00000000 can resize
        WINDOW_AUTOSIZE: 0x00000001 can't resize
    */
    imshow(windowName, image);

    setMouseCallback(windowName, onMouse, 0);

    waitKey(0);
    return 0;
}

void onMouse (int event, int x, int y, int flags, void *none) {
    static Point pre_pos;

    if (event == EVENT_LBUTTONDOWN) {
        pre_pos.x = x;
        pre_pos.y = y;
        printf("L click pos x: %d, y: %d\n", pre_pos.x, pre_pos.y);
    } else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON) {
        Point now_pos;

        now_pos.x = x;
        now_pos.y = y;

        line(image, pre_pos, now_pos, Scalar(0, 0, 0), 2);
        imshow(windowName, image);

        pre_pos.x = x;
        pre_pos.y = y;

        printf("moving pos x: %d, y: %d\n", x, y);
    }
}
