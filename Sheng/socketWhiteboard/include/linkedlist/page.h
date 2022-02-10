#ifndef __PAGE_LINKEDLIST_H__
#define __PAGE_LINKEDLIST_H__

#include "linkedlist/stroke.h"

typedef struct _pageNode {
    int pid;
    strokeNode **strokes;
    char fileName[256];
    struct _pageNode *next;
} pageNode;

int initPage (pageNode ***);
int freePage (pageNode **);

int addPage (pageNode **, int);
int delPage (pageNode **, int);
int deleteAllPages (pageNode **);

int getPagesLen (pageNode **);
int getPagesOrder (pageNode **pages, int id);
pageNode *getLastPage (pageNode **);
pageNode *getPage (pageNode **, int);
pageNode *getPrevPage (pageNode **, int);
pageNode *getPagebyOrder (pageNode **, int);

/* debug use */
int showPageList(pageNode **);
#endif
