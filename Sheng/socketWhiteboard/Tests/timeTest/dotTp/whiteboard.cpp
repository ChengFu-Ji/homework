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

#include "linkedlist.h"

using namespace cv;

void *writefile(void*);
void showTime (char *);
void autoTesting (int fd);
void writeCSV(size_t, char *);
void CSVinit();

char windowName[] = "Display Image";
Mat image;
int pid = 0;

int main() {
    int fd;
    struct sockaddr_in me;
    struct hostent *host;
    FILE *fp;

    if ((fp = fopen("clientLog.csv", "r"))) {
        fclose(fp);
    } else {
        CSVinit();
    }

    host = gethostbyname("localhost");
    fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&me, 0, sizeof(struct sockaddr_in));
    me.sin_family = AF_INET;
    me.sin_port = htons(2244);
    memcpy(&me.sin_addr, host->h_addr_list[0], host->h_length);

    if (connect(fd, (struct sockaddr *) &me, sizeof(struct sockaddr_in)) < 0) {
        printf("Error Connection!\n");
        exit(1);
    } else {
        showTime("connected");
        read(fd, &pid, sizeof(int));
    }

    image = Mat(1000, 600, CV_8UC3, Scalar(255, 255, 255));
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);

    autoTesting(fd);
    waitKey(10);

    close(fd);
    showTime("exit");
    return 0;
}

void autoTesting (int fd) {
    Point2d sp, ep;
    Data_s tmp;
    int count, l = 1;
    char data[100];
    double spacingX, spacingY;

    srand(time(NULL));
    count = 1;
    while (1) {
        sp.x = rand() % 600;
        sp.y = rand() % 1000;
        ep.x = rand() % 600;
        ep.y = rand() % 1000;
        while (sp.x == ep.x && sp.y == ep.y) {
            sp.x = rand() % 600;
            sp.y = rand() % 1000;
        }

        spacingX = (ep.x - sp.x)/100.0;
        spacingY = (ep.y - sp.y)/100.0;

        showTime("DSSend");
        for (int i = 1; i < 100; i++) {
            ep.x = sp.x + (spacingX);
            ep.y = sp.y + (spacingY);
            if (ep.x < 0 || ep.y < 0) {
                printf("ep.x %lf, ep.y %lf\n", ep.x, ep.y);
            }
            line(image, sp, ep, Scalar(0, 255, 0), 2);
            imshow(windowName, image);

            tmp.x = sp.x;
            tmp.y = sp.y;
            write(fd, &l, sizeof(int));
            write(fd, &tmp, sizeof(tmp));

            sp = ep;
        }

        tmp.x = sp.x;
        tmp.y = sp.y;
        write(fd, &l, sizeof(int));
        write(fd, &tmp, sizeof(tmp));

        tmp.x = -1;
        tmp.y = 0;
        write(fd, &l, sizeof(int));
        write(fd, &tmp, sizeof(tmp));
        showTime("DESend");
        
        if (count >= 100) {
            printf("times %d\n", count);
            break;
        } else {
            printf("times %d\n", count);
            waitKey(1);
            count++;
        }
    }
}

void showTime (char *status) {
    struct timespec ts;
    size_t t;
    clock_gettime(CLOCK_REALTIME, &ts);

    t = ts.tv_sec;
    for (int i = 0; i < 9; i++) {
        t *= 10;
    }

    writeCSV(ts.tv_nsec+t, status);
    printf("%s, time[%lu ns]\n", status, t + ts.tv_nsec);
}

void writeCSV(size_t time, char *status) {
    FILE *fp;

    while (1) {
        fp = fopen("clientLog.csv", "a");
        if (fp) {
            break;
        }
    }
    fprintf(fp, "%d, %s, %lu\n", pid, status, time);
    fclose(fp);
}

void CSVinit() {
    FILE *fp;

    fp = fopen("clientLog.csv", "w");
    fprintf(fp, "PID, status, recv_time (unit: ns)\n");
    fclose(fp);
}

