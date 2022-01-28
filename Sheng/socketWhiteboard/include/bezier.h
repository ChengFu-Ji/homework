#ifndef __BEZIER_H__
#define __BEZIER_H__


#include <math.h>
#include "linkedlist/pos.h"

//#ifndef __LINKEDLIST_H__
//#endif

unsigned int factorial(int);
double dist (DPos, DPos);
double bezierPoly (double, int, int); 
void bezierCurve (DPos *, int, DPos *, int);
void triangleDist (DPos *, double *);

Pos *DPosToPos (DPos*, int);
DPos *PosToDPos (Pos*, int);
/*
Pos setPos(int, int);
DPos setDPos(double, double);
*/
#endif
