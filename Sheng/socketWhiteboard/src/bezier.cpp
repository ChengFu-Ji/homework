#include <math.h>
#include <opencv2/opencv.hpp>

#include "bezier.h"
#include "linkedlist.h"


int factorial(int n) {
    int back = 1;
    for (int i = 1; i <= n; i++) {
        back *= i;
    }
    return back;
}

double dist (cv::Point2d p1, cv::Point2d p2) {
    return sqrt(pow(fabs(p1.x-p2.x), 2) + pow(fabs(p1.y-p2.y), 2));
}

double bezierPoly (double t, int n, int i) {
    return (factorial(n)/(factorial(i)*factorial(n-i))) * pow(t, i) * pow(1-t, n-i);
}

void bezierCurve (cv::Point2d *det, int SR, cv::Point2d *p, int length) {
    double t = 1.0/SR;
    double tn = 0.0;
    double distance[3];
    double w[length];
    double angle[length-2];

    w[0] = 1;
    w[length-1] = 1;
    /*
    for (int i = 0; i < length; i++) {
        printf("p %lf %lf\n", p[i].x, p[i].y);
    }
    */

    for (int j = 0; j < length-2; j++) {
        triangleDist(p+j, distance);
        double a = (pow(distance[2], 2) - pow(distance[1], 2) - pow(distance[0], 2));
        double b = -2*distance[0]*distance[1];
        angle[j] = (!isnormal(1.0/a) || !isnormal(1.0/b))? 0.0 : a/b;
        /*
        if (!isnormal(1.0/a) || !isnormal(1.0/b)) {
            angle[j] = 0.0;
        } else {
            angle[j] = a/b;
        }
        printf("angle[j] %lf\n", angle[j]);
        */
    }
    for (int i = 1, j = 0; i < length-1; i++, j++) {
        //w[i] = 2;
        w[i] = fabs(1000-dist(p[i], p[i+1]))/100;
        w[i] *= (!isnormal(sqrt(1 - pow(angle[j], 2))))?  1 : (1+sqrt(1 - pow(angle[j], 2)));
    }

    for (int i = 0; i < SR; i++, tn+=t) {
        double s = 0;
        det[i] = cv::Point(0,0);
        for (int j = 0; j < length; j++) {
            double bz = (bezierPoly(t*i, length, j)) * w[j];
            det[i].x += bz * p[j].x;
            det[i].y += bz * p[j].y;
            s += bz; 
        }
        det[i].x /= s;
        det[i].y /= s;
    }
}

void triangleDist (cv::Point2d *p, double *dists) {
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
