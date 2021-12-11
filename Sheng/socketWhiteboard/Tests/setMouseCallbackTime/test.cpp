#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

using namespace cv;

void *recvData(void*);
void *recvControl(void*);
void onMouse (int, int, int, int, void*);
void writeCSV(double time);
void CSVinit();

char windowName[] = "Display Image";
Mat image;

int main() {
    CSVinit();

    image = Mat(990, 1690, CV_8UC3, Scalar(255, 255, 255));
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);

    setMouseCallback(windowName, onMouse, NULL);

    while (waitKey(0) != 27);
    
    return 0;
}

void onMouse (int event, int x, int y, int flags, void *userdata) {
    static struct timespec t1 = {0, 0};
    if (event == EVENT_MOUSEMOVE) {
        if (t1.tv_sec != 0) {
            struct timespec t2;
            clock_gettime(CLOCK_REALTIME, &t2);
            size_t t = t2.tv_sec - t1.tv_sec;
            for (int i = 0; i < 9; i++) {
                t *= 10;
            }
            t += (t2.tv_nsec-t1.tv_nsec);
            double tn = t; 
            for (int i = 0; i < 9; i++) {
                tn/=10.0;
            }

            printf("time: %lf s\n", tn);
            writeCSV(tn);
        }
        clock_gettime(CLOCK_REALTIME, &t1);
    }
}


void writeCSV(double time) {
    FILE *fp;

    while (1) {
        fp = fopen("periodLog.csv", "a");
        if (fp) {
            break;
        }
    }
    fprintf(fp, "%.6lf\n", time);
    fclose(fp);
}

void CSVinit() {
    FILE *fp;

    fp = fopen("periodLog.csv", "w");
    fprintf(fp, "recv_time (unit: ns)\n");
    fclose(fp);
}
