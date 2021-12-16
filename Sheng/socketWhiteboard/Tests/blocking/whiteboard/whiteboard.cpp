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
void writeCSV(Data_s);
void CSVinit();

char windowName[] = "Display Image";
Mat image;
int readOrNot = 1;

int main(int argc, char *argv[]) {
    int fd;
    struct sockaddr_in me;
    struct hostent *host;
    FILE *fp;

    if (argc < 2) {
        printf("useage: ./Whiteboard [0(no read)/1(use read)] \n");
        exit(1);
    }

    if ((fp = fopen("clientLog.csv", "r"))) {
        fclose(fp);
    }
    readOrNot = atoi(argv[1]);

    host = gethostbyname("localhost");
    fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&me, 0, sizeof(struct sockaddr_in));
    me.sin_family = AF_INET;
    me.sin_port = htons(2244);
    memcpy(&me.sin_addr, host->h_addr_list[0], host->h_length);

    if (connect(fd, (struct sockaddr *) &me, sizeof(struct sockaddr_in)) < 0) {
        printf("Error Connection!\n");
        exit(1);
    /*
    } else {
        showTime("connected");
        read(fd, &pid, sizeof(int));
    */
    }

    image = Mat(1000, 600, CV_8UC3, Scalar(255, 255, 255));
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);

    CSVinit();
    autoTesting(fd);
    waitKey(10);

    close(fd);
    return 0;
}

void autoTesting (int fd) {
    Point2d sp, ep;
    Data_s tmp, data;
    int count, l = 1;
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

        for (int i = 1; i < 100; i++) {
            ep.x = sp.x + (spacingX);
            ep.y = sp.y + (spacingY);
            if (ep.x < 0 || ep.y < 0) {
                printf("ep.x %lf, ep.y %lf\n", ep.x, ep.y);
            }
            line(image, sp, ep, Scalar(0, 255, 0), 2);
            imshow(windowName, image);
            tmp.x = (int) sp.x;
            tmp.y = (int) sp.y;
            write(fd, &tmp, sizeof(tmp));
            sp = ep;
        }

        tmp.x = sp.x;
        tmp.y = sp.y;
        write(fd, &tmp, sizeof(tmp));

        tmp.x = -1;
        tmp.y = 0;
        write(fd, &tmp, sizeof(tmp));
        
        if (readOrNot) {
            if (count == 90) {
                for (int i = 0; i < count*101; i++) {
                    for (int j = 0; j < 9; j++) {
                        read(fd, &data, sizeof(Data_s));
                    }
                    writeCSV(data);
                }
            }
        }
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

    //writeCSV(ts.tv_nsec+t, status);
    printf("%s, time[%lu ns]\n", status, t + ts.tv_nsec);
}

void writeCSV(Data_s data) {
    FILE *fp;
    static int id = 1;

    while (1) {
        fp = fopen("dataLog.csv", "a");
        if (fp) {
            break;
        }
    }
    fprintf(fp, "%d, %d, %d\n", id++, data.x, data.y);
    fclose(fp);
}

void CSVinit() {
    FILE *fp;

    fp = fopen("dataLog.csv", "w");
    fprintf(fp, "id, x, y\n");
    fclose(fp);
}

