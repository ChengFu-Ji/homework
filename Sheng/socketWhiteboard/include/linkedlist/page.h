#ifndef __PAGE_LINKEDLIST_H__
#define __PAGE_LINKEDLIST_H__

#include "linkedlist/stroke.h"

typedef struct _pageNode {
    int pid;
    strokeNode **strokes;
    struct _pageNode *next;
} pageNode;

int initPage (pageNode ***);
int freePage (pageNode **);

int addPage (pageNode **, int);
int delCurrentPage (pageNode **, int);
int deleteAllPages (pageNode **);

pageNode *getLastPage (pageNode **);
pageNode *getPage (pageNode **, int);
pageNode *getPrevPage (pageNode **, int);
pageNode *getPagebyOrder (pageNode **, int);

/* debug use */
int showPageList(pageNode **);
#endif
