#ifndef __PAGE_LINKEDLIST_H__
#define __PAGE_LINKEDLIST_H__

#include "linkedlist/stroke.h"

#define PAGE_PERSONAL 1
#define PAGE_COMMON 0

typedef struct _pageNode {
    int pid;
    char status;
    strokeNode **strokes;
    char fileName[255];
    struct _pageNode *next;
} pageNode;

int initPage (pageNode ***);
int freePage (pageNode **);

int addPage (pageNode **, int, char);
int delPage (pageNode **, int, char);
int deleteAllPages (pageNode **);

int getPagesLen (pageNode **, char);
int getPagesOrder (pageNode **pages, int id, char status);

pageNode *getLastPage (pageNode **, char);
pageNode *getPage (pageNode **, int, char);
pageNode *getNextPage (pageNode **, int, char);
pageNode *getPrevPage (pageNode **, int, char);
pageNode *getPagebyOrder (pageNode **, int, char);

/* debug use */
int showPageList(pageNode **);
#endif
