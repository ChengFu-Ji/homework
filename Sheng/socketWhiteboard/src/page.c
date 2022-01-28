#include "linkedlist/page.h"

int initPage (pageNode ***pagePtr) {
    if ((*pagePtr = (pageNode **) malloc(sizeof(pageNode *))) == NULL) {
        return -1;
    }

    if ((**pagePtr = (pageNode *) malloc(sizeof(pageNode))) == NULL) {
        return -1;
    }
    (**pagePtr)->next = NULL;

    return 0;
}

int freePage (pageNode **page) {
    free(*page);
    free(page);
    return 0;
}

int addPage (pageNode **pages, int id) {
    pageNode *cur, *newNode;

    cur = (*pages);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    newNode = (pageNode *)malloc(sizeof(pageNode));
    newNode->pid = id;
    initStroke(&newNode->strokes);
    newNode->next = NULL;
    
    cur->next = newNode;
    return 0;
}

int delCurrentPage(pageNode **pages, int id) {
    pageNode *cur, *next;

    cur = (*pages);
    while (cur->next != NULL) {
        if (cur->next->pid == id) {
            next = cur->next->next;

            deleteAllStrokes(cur->next->strokes);
            freeStroke(cur->next->strokes);

            free(cur->next);
            cur->next = next;
            return 0;
        }
        cur = cur->next;
    }
    return -1;
}

int deleteAllPages(pageNode **pages) {
    pageNode *cur, *next;

    cur = (*pages);
    while (cur->next != NULL) {
        next = cur->next->next;
        deleteAllStrokes(cur->next->strokes);
        freeStroke(cur->next->strokes);
        free(cur->next);

        cur->next = next;
    }
    return 0;
}

int showPageList(pageNode **pages) {
    pageNode *cur;

    cur = (*pages);
    while (cur->next != NULL) {
        showStrokeList(cur->next->strokes);

        cur = cur->next;
    }
    return 0;
}


pageNode *getPage (pageNode **pages, int pid) {
    pageNode *cur;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->pid == pid) {
            return cur->next;
        }
        cur = cur->next;
    }
    return NULL;
}

pageNode *getPrevPage (pageNode **pages, int pid) {
    pageNode *cur;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->pid == pid) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}
