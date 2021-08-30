#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct node_s {
    unsigned show : 1;
    char data[101];
    struct node_s *next;
} Node;

int add (Node **, char *);
int del (Node **, char *);
int insert (Node **, char *);
int save (Node **, char *);
int load (Node **, char *);
int showList (Node **, char *);
int cleanList (Node **, char *);
int showN (Node **, char *);

#endif
