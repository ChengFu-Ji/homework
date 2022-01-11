#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <opencv2/opencv.hpp>

#include "linkedlist.h"
#include "whiteboard.h"


using namespace cv;

extern char windowName[];
extern Mat image;
extern Node_s **recvList, **sendList;

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

    /*
    for (int i = 0; i < length; i++) {
        printf("points [%lf, %lf]\n", p[i].x, p[i].y);
    }
    */

    for (int j = 0; j < length-2; j++) {
        triangleDist(p+j, distance);
        /*
        for (int i = 0; i < 3; i++) {
            printf("distance %lf\n", distance[i]);
        }
        */
        double a = (pow(distance[2], 2) - pow(distance[1], 2) - pow(distance[0], 2));
        double b = -2*distance[0]*distance[1];
        if (!isnormal(1.0/a) || !isnormal(1.0/b)) {
            /*
            printf("not normal 1/a %lf, 1/b %lf\n", 1.0/a, 1.0/b);
            printf("a %lf b %lf\n", a, b);
            */
            angle[j] = 0.0;
        } else {
            /*
            printf("normal 1/a %lf, 1/b %lf\n", 1.0/a, 1.0/b);
            printf("a %lf b %lf\n", a, b);
            */
            angle[j] = a/b;
        }
        //printf("angle[j] %lf\n", angle[j]);
    }
    for (int i = 1, j = 0; i < length-1; i++, j++) {
        w[i] = (1+sqrt(1 - pow(angle[j], 2)))*(fabs(1000-dist(p[i], p[i+1])))/100;
        if (1-pow(angle[j], 2) < 0 && !isnormal(w[i])) {
            w[i] = fabs(1000-dist(p[i], p[i+1]))/100;
        }
        //w[i] = 2;
    }
    /*
    for (int i = 0; i < length; i++) {
        printf("w[i] %lf\n", w[i]);
    }
    */

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
    //printf("bezier %lf, %lf\n", det[0].x, det[0].y);
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
        if (p[i].x != p[i+1].x || p[i].y != p[i+1].y) {
            dists[i] = dist(p[i], p[i+1]);
        } else {
            dists[i] = 0.0;
        }
    }
    if (p[i].x != p[0].x || p[i].y != p[0].y) {
        dists[i] = dist(p[0], p[i]);
    } else {
        dists[i] = 0.0;
    }
}

void plotHandwriting (Point2d *p, Point2d *plot, int repair, Scalar color, int thickness) {
    int i, j;
    double distance = 0.0;
    //Scalar handwritColor = Scalar(17, 10, 120);
    Scalar handwritColor = color;
    //int thk = 6;
    int thk = thickness;


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
            /*
            printf("dist(plot[i], p[DOTS-1]) %lf\n", dist(plot[i], p[DOTS-1]));
            printf("plot[i] %lf %lf\n", plot[i].x, plot[i].y);
            printf("p[DOTS-1] %lf %lf\n", p[DOTS-1].x, plot[i].y);
            */
            for (i = 0; dist(plot[i], p[DOTS-1]) > 30 && i < TIMES-3; i++) {
                line(image, plot[i], plot[i+1], handwritColor, thk);
                //printf("dot %lf, %lf\n", plot[i].x, plot[i].y);
            }
            for (j = i; dist(plot[j], p[DOTS-1]) > 21.2 && j < TIMES-2; j++);
            //printf("i and j %d, %d\n", i, j);
            p[0] = plot[i];
            p[1] = plot[j];
            p[2] = p[DOTS-1];
            i = 3;
            //printf("%lf %lf\n", p[0].x, p[0].y);

            /*
            for (int i = 0; i < DOTS-1; i++) {
                printf("\n");
                printf("dones points [%lf, %lf]\n", p[i].x, p[i].y);
            }
            */
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
        //printf("in\n");
    }

    for (; i < DOTS; i++) {
        p[i] = Point(-1, -1);
    }
}
