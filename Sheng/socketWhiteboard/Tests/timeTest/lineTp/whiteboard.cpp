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

extern "C" {
#include "linkedlist.h"
}

#define TIMES 1000
#define DOTS 6

using namespace cv;

void showTime (char *);
void autoTesting (int fd);
void writeCSV(size_t, char *);
void CSVinit();
int factorial(int);
double dist (Point2d, Point2d);
double bezierPoly (double, int, int); 
void triangleDist (Point2d *, double *);
void bezierCurve (Point2d *, int, Point2d *, int);
void plotHandwriting (Point2d *, Point2d *, int, Scalar);

char windowName[] = "Display Image";
Mat image;
Node_s **sendList;
int height = 900;
int width = 1600;
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

    image = Mat(height, width, CV_8UC3, Scalar(255, 255, 255));
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
    Point2d p[DOTS], plot[TIMES];
    Data_s tmp;
    double waitTime, avg = 0.0, bouns = 0.0;
    int PosNum, tmpNum, i, totalPos = 0, overTimes = 0;

    srand(time(NULL));
    // 再取得 2 ~ 8 個位置後 等待時間 暴增
    overTimes = (rand() % 6) + 2;
    for (int count = 1; count <= 100; count++) {
        // 一筆畫中隨機取得 5 ~ 60 個點
        PosNum = (rand() % 55) + 5;
        tmpNum = PosNum;
        showTime("LSSend");
        Scalar tmpScalar = Scalar(rand() % 255, rand() % 255, rand() % 255);

        for (i = 0; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
        while (PosNum > 0) {
            for (i = 0; i < DOTS && PosNum-i != 0; i++) {
                if (p[i].x == -1 && p[i].y == -1) {
                    if (overTimes-- == 0) {
                        overTimes = rand() % 6 + 2;
                        // 模擬取得每個點的間隔時間並不固定，有時會爆增，等待時間的爆增值，介於 20 ~ 54 毫秒之間
                        bouns =  rand() % 34 + 20;
                    } else {
                        bouns = 0.0;
                    }
                    // 模擬取得每一個點之間的間隔時間，等待時間的值，介於 10 ~ 90 毫秒之間。
                    waitTime = (rand() % 26 )+ 10 + bouns;
                    avg += waitTime;
                    p[i].x = rand() % width;
                    p[i].y = rand() % height;
                    for (int j = 0; j < i; j++) {
                        while (p[i].x == p[j].x && p[i].y == p[j].y) {
                            p[i].x = rand() % width;
                            p[i].y = rand() % height;
                        }
                    }
                    waitKey(waitTime);
                    // sleep() 只能放入整數，使用 waitKey() 代替， waitKey() 是以豪秒計算。
                }
            }

            for (i = 0; i < DOTS-1 && PosNum-(i+1) != 0; i++) {
                if (p[i+1].x != -1 && p[i+1].y != -1) {
                    tmp.x = p[i].x;
                    tmp.y = p[i].y;
                    add(sendList, tmp);
                }
            }

            if (p[i].x != -1 && p[i].y != -1) {
                tmp.x = p[i].x;
                tmp.y = p[i].y;
                add(sendList, tmp);
            }

            PosNum -= (++i);
            bezierCurve(plot, TIMES, p, DOTS);

            if (i == DOTS) {
                plotHandwriting(p, plot, 0, tmpScalar);
            } else {
                plotHandwriting(p, plot, 1, tmpScalar);
            }

            imshow(windowName, image);
            waitKey(1);
        }

        tmp.x = -1;
        tmp.y = 0;
        add(sendList, tmp);
        
        totalPos += tmpNum;
        socket_write(sendList, fd, ++tmpNum);

        showTime("LESend");
        
        cleanList(sendList);

        printf("times %d\n", count);

        sleep(1);

        for (i = 0; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
    }
    printf("avg %lf\n", avg/totalPos);
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

void bezierCurve (Point2d *det, int SR, Point2d *p, int length) {
    double t = 1.0/SR;
    double tn = 0.0;
    double s;
    double w[length];
    double distance[length];
    double angle[length-2];

    for (int i = 0; i < length; i++) {
        w[i] = 1;
    }

    for (int j = 0; j < length-2; j++) {
        triangleDist(p+j, distance);
        double a = (pow(distance[2], 2) - pow(distance[1], 2) - pow(distance[0], 2));
        double b = -2*distance[0]*distance[1];
        if (fabs(b) == 0.0) {
            angle[j] = 0;
        } else if ((int) a == (int) -b) {
            angle[j] = -1;
        } else if (a/b > 1.0) {
            angle[j] = a/b-1.0;
        } else {
            angle[j] = a/b;
        }
    }

    for (int i = 1, j = 0; i < length-1; i++, j++) {
        w[i] = (1+fabs(sqrt(1-pow(angle[j], 2))))*(fabs(1000-dist(p[i], p[i+1])))/100;
        //w[i] = 2;
    }

    for (int i = 0; i < SR; i++, tn+=t) {
        det[i] = Point(0,0);
        s = 0;
        for (int j = 0; j < length; j++) {
            double bz = (bezierPoly(t*i, length, j)) * w[j];
            det[i].x +=  bz * p[j].x;
            det[i].y += bz * p[j].y;
            s += bz; 
        }
        det[i].x /= s;
        det[i].y /= s;
    }
}

int factorial(int n) {
    int back = 1;
    for (int i = 1; i <= n; i++) {
        back *= i;
    }
    return back;
}
double dist (Point2d p1, Point2d p2) {
    return sqrt(pow(fabs(p1.x-p2.x), 2) + pow(fabs(p1.y-p2.y), 2));
}

double bezierPoly (double t, int n, int i) {
    return (factorial(n)/(factorial(i)*factorial(n-i))) * pow(t, i) * pow(1-t, n-i);
}

void triangleDist (Point2d *p, double *dists) {
    int i;
    for (i = 0; i < 2; i++) {
        if (p[i].x != p[i+1].x && p[i].y != p[i+1].y) {
            dists[i] = dist(p[i], p[i+1]);
        } else {
            dists[i] = 0.0;
        }
    }
    if (p[i].x != p[0].x && p[i].y != p[0].y) {
        dists[i] = dist(p[0], p[i]);
    } else {
        dists[i] = 0.0;
    }
}

void plotHandwriting (Point2d *p, Point2d *plot, int repair, Scalar color) {
    int i, j;
    double distance = 0.0;
    //Scalar handwritColor = Scalar(17, 10, 120);
    Scalar handwritColor = color;

    int thk = 6;


    if (!repair) {
        for (i = 0; i < DOTS-1; i++) {
            distance += dist(p[i], p[i+1]);
        }

        if (distance <= 70) {
            for (i = 0; i < TIMES-1; i++) {
                line(image, plot[i], plot[i+1], handwritColor, thk);
            }
            line(image, plot[TIMES-1], p[DOTS-1], handwritColor, thk);
            p[0] = p[DOTS-1];
            i = 1;
        } else {
            for (i = 0; dist(plot[i], p[DOTS-1]) > 30 && i < TIMES-3; i++) {
                line(image, plot[i], plot[i+1], handwritColor, thk);
            }
            for (j = i; dist(plot[j], p[DOTS-1]) > 21.2 && j < TIMES-2; j++);
            p[0] = plot[i];
            p[1] = plot[j];
            p[2] = p[DOTS-1];
            i = 3;
        }
    } else {
        for (i = DOTS; i >= 3; i--) {
            if (p[i-1].x != -1 && p[i-1].y != -1) {
                bezierCurve(plot, TIMES, p, i);
                for (j = 0; j < TIMES-1; j++) {
                    line(image, plot[j], plot[j+1], handwritColor, thk);
                }
                line(image, plot[TIMES-1], p[i-1], handwritColor, thk);
                break;
            }
        }
        if (p[1].x != -1 && p[1].y != -1) {
            line(image, p[0], p[1], handwritColor, thk); 
        }
        i = 0;
    }

    for (; i < DOTS; i++) {
        p[i] = Point(-1, -1);
    }
}
