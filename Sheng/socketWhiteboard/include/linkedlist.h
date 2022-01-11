#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct data_s {
    int x;
    int y;
} Data_s;

typedef struct node_s {
    Data_s point;
    struct node_s *next;
} Node_s;

typedef struct handwrite_Control {
    int r;
    int g;
    int b;
    int thk;
    int id;
    int length;
} HWCtrl;

int add (Node_s **, Data_s);
int del (Node_s **, Data_s);
int showList (Node_s **);
int cleanList (Node_s **);
int socket_write (Node_s **, int, int);
int socket_read (Node_s **, int, int);

#endif
