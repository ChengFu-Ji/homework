#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct data {
    int id;
    char input[101];
} Data;

typedef struct node_s {
    Data *data;
    struct node_s *next;
} Node;

int add(Node **, char *);
int del(Node **, char *);
int save(Node **, char *);
int load(Node **, char *);
int showList(Node **, char *);
int cleanList(Node **, char *);
int showN(Node **, char *);

#endif
