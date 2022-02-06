#ifndef __STROKE_LINKEDLIST_H__
#define __STROKE_LINKEDLIST_H__

#include "linkedlist/pos.h"

/*
#ifdef __COLORFUL_WHITEBOARD__
typedef struct _strokeColor {
    int r;
    int g;
    int b;
} strokeColor;

typedef struct _strokeNode {
    strokeColor sColor;
    int thickness;
    int is_eraser;
    posNode **pos;
    struct _strokeNode *next;
} strokeNode;

#else 
*/

typedef struct _strokeNode {
    int thickness;
    int is_eraser;
    posNode **pos;
    struct _strokeNode *next;
} strokeNode;

/*
#endif 
*/

typedef struct _socket_condition {
    int drawpid;
    int addpid;
    int deletepid;
    int eraser;
    int thickness;
    int poslen;
} SockCond;

int initStroke (strokeNode ***);
int freeStroke (strokeNode **);

int addStroke (strokeNode **, posNode **, int, int);
int delLastStroke (strokeNode **);
int deleteAllStrokes (strokeNode **);


//int setLastThickness (strokeNode **, int);

//int sendCond (int, SockCond);
int sendStroke (int, strokeNode *, SockCond);
int sendStrokeList (int, strokeNode **, SockCond);

strokeNode *getLastStroke (strokeNode **strokes);
/* debug use */
int showStrokeList(strokeNode **);
#endif
