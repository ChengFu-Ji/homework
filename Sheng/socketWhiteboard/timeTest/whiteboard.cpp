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
#include <signal.h>
#include <time.h>

#include "linkedlist.h"

using namespace cv;

void showTime (char *);
void autoTesting (int fd);
void writeCSV(size_t, char *);
void CSVinit();

char windowName[] = "Display Image";
char imageName[] = "background.png";
Mat image;
Node_s **sendList;
int pid = 0;

int main() {
    FILE *fp;
    int fd;
    struct sockaddr_in me;
    struct hostent *host;

    if ((fp = fopen("clientLog.csv", "r"))) {
        fclose(fp);
    } else {
        CSVinit();
    }
    sendList = (Node_s **)malloc(sizeof(Node_s *));
    *sendList = (Node_s *)malloc(sizeof(Node_s));
    (*sendList)->next = NULL;

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

    image = imread(imageName, 1);
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);

    autoTesting(fd);
    waitKey(10);

    close(fd);
    showTime("exit");
    free(*sendList);
    free(sendList);

    return 0;
}

void autoTesting (int fd) {
    Point2d sp, ep;
    Data_s tmp;
    char data[100];
    int count;
    double spacingX, spacingY;

    srand(time(NULL));
    for (int count = 1; count <= 100; count++) {
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

        showTime("LSSend");
        int i;
        for (i = 1; i < 100; i++) {
            ep.x = sp.x + (spacingX);
            ep.y = sp.y + (spacingY);
            line(image, sp, ep, Scalar(0, 255, 0), 2);
            imshow(windowName, image);
            tmp.x = sp.x;
            tmp.y = sp.y;
            add(sendList, tmp);
            sp = ep;
        }

        tmp.x = sp.x;
        tmp.y = sp.y;
        add(sendList, tmp);
        tmp.x = -1;
        tmp.y = 0;
        add(sendList, tmp);
        i++;
        
        socket_write(sendList, fd, i);
        

        showTime("LESend");
        
        cleanList(sendList);

        printf("times %d\n", count);
        waitKey(1);
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

