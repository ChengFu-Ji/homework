#ifndef __STROKE_LINKEDLIST_H__
#define __STROKE_LINKEDLIST_H__

#include "linkedlist/pos.h"

typedef struct _strokeNode {
    posNode **pos;
    struct _strokeNode *next;
} strokeNode;

int initStroke(strokeNode ***);
int freeStroke(strokeNode **);

int addStroke(strokeNode **, posNode **);
int delLastStroke(strokeNode **);
int deleteAllStrokes(strokeNode **);

/* debug use */
int showStrokeList(strokeNode **);
#endif
