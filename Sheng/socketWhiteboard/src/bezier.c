#include "bezier.h"

unsigned int factorial(int n) {
    unsigned int back = 1;
    for (int i = 1; i < n; i++, back *= i);

    return back;
}

double dist (DPos p1, DPos p2) {
    return sqrt(pow(fabs(p1.x-p2.x), 2) + pow(fabs(p1.y-p2.y), 2));
}

double bezierPoly (double t, int n, int i) {
    return (factorial(n)/(factorial(i)*factorial(n-i))) * pow(t, i) * pow(1-t, n-i);
}

void bezierCurve (DPos *det, int SR, DPos *p, int length) {
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
        //det[i] = setDPos(0,0);
        det[i] = (DPos) {0, 0};
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

void triangleDist (DPos *p, double *dists) {
    for (int i = 0, inext = 2; i < 3; i++, inext = (i+2)%3) {
        dists[inext] = (p[i].x != p[inext].x || p[i].y != p[inext].y)? dist(p[i], p[inext]) : 0.0;
    }
}

DPos *PosToDPos (Pos *pos, int len) {
    DPos *dpos = (DPos *)malloc(sizeof(DPos)*len);

    for (int i = 0; i < len; i++) {
        dpos[i] = (DPos){(double)pos[i].x, (double)pos[i].y};
    }

    return dpos;
}

Pos *DPosToPos (DPos *dpos, int len) {
    Pos *pos = (Pos *)malloc(sizeof(Pos)*len);

    for (int i = 0; i < len; i++) {
        pos[i] = (Pos){(double)dpos[i].x, (double)dpos[i].y};
    }

    return pos;
}

/*
Pos setPos (int x, int y) {
    return (Pos){x, y};
}

DPos setDPos (double x, double y) {
    return (DPos){x, y};
}
*/

/*
#include <stdio.h>
#include <stdlib.h>

int main () {
    Pos *x;
    double dis[3] = {0};
    x = (Pos *) malloc(3*sizeof(Pos));
    x[0] = (Pos){1, 1};
    x[1] = (Pos){2, 1};
    x[2] = (Pos){4, 1};
    DPos *xx;
    xx = PosToDPos(x, 3);
    //xx = (DPos *) &x;
    triangleDist(xx, dis);
    
    for (int i = 0; i < 3; i++) {
        printf("i %d ", i);
        printf("xx [%lf, %lf] ", xx[i].x, xx[i].y);
        printf("x [%d, %d] ", x[i].x, x[i].y);
        printf("dis %lf\n", dis[i]);
    }
    //printf("sizeof(pos) %lu sizeof(dpos) %lu\n", sizeof(Pos), sizeof(DPos));
    //x = (Pos) {x.x+1, x.y+1};
    //printf ("x [%d, %d]\n", x.x, x.y);
    //x = setPos(0, 0);
    //printf ("x [%d, %d]\n", x.x, x.y);
    free(xx);
    return 0;
}

*/
