#ifndef __POS_LINKEDLIST_H__
#define __POS_LINKEDLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _position {
    int x;
    int y;
} Pos;

typedef struct _position_double {
    double x;
    double y;
} DPos;

typedef struct _posNode {
    Pos point;
    struct _posNode *next;
} posNode;

int initPos(posNode ***);
int freePos(posNode **);

int addPos (posNode **, Pos);
int delPos (posNode **, Pos);
int deleteAllPos (posNode **);
int catPosList (posNode **, posNode **);

/* debug use */
int showPosList (posNode **);

int socket_write (int, posNode **, int);
int socket_read (int, posNode **, int);


#endif
