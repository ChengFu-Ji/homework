#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct position {
    int x;
    int y;
} Pos;

typedef struct node_s {
    Pos point;
    struct node_s *next;
} Node_s;

int add (Node_s **, Pos);
int del (Node_s **, Pos);
void listcat(Node_s **, Node_s **);
int showList (Node_s **);
int cleanList (Node_s **);
int socket_write (Node_s **, int, int);
int socket_read (Node_s **, int, int);

#endif
