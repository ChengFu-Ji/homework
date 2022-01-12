#ifndef __BEZIER_H__
#define __BEZIER_H__

#include <opencv2/opencv.hpp>

int factorial(int);
double dist (cv::Point2d, cv::Point2d);
double bezierPoly (double, int, int); 
void bezierCurve (cv::Point2d *, int, cv::Point2d *, int);
void triangleDist (cv::Point2d *p, double *dists);

#endif
