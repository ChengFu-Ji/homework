#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct data_s {
    int id;
    int x;
    int y;
} Data_s;
typedef struct node_s {
    Data_s point;
    struct node_s *next;
} Node_s;

int add (Node_s **, int, int);
int showList (Node_s **);
int cleanList (Node_s **);
int socket_write (Node_s **, int);
int socket_read (Node_s **, int);

#endif
