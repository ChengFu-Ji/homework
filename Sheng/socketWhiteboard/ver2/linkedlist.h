#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct position {
    int x;
    int y;
} Pos;

// 更改 Node 內含的型態。
typedef struct PosNode {
    int id;
    int len;
    Pos *p;
    struct node_s *next;
} Node_p;

int add (Node_p **node, int id, Pos *pts, size_t length);
int del (Node_p **node, Pos *pts);
int showList (Node_p **);
int cleanList (Node_p **);
int socket_write (int fd, int id, Node_p **node, int size);
int socket_read (int fd, int id, Node_p **node, int size);

#endif
