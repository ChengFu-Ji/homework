#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <opencv2/opencv.hpp>

#include "linkedlist.h"

#define TIMES 1000
#define DOTS 6

using namespace cv;

int factorial(int);
double dist (Point2d, Point2d);
double bezierPoly (double, int, int);
void triangleDist (Point2d *, double *);
void bezierCurve (Point2d *, int, Point2d *, int);
void plotHandwriting (Point2d *, Point2d *, int);


#endif // WHITEBOARD_H
