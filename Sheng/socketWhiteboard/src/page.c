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

int addPage (pageNode **pages, int id, char status) {
    pageNode *cur, *newNode;

    cur = (*pages);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    newNode = (pageNode *)malloc(sizeof(pageNode));
    newNode->pid = id;
    newNode->status = status;
    newNode->fileName[0] = '\0';
    initStroke(&newNode->strokes);
    newNode->next = NULL;
    
    cur->next = newNode;
    return 0;
}

/*
int insertImage (pageNode *page, char *fileName) {
    return 0;
}
*/

int delPage (pageNode **pages, int id, char status) {
    pageNode *cur, *next;

    cur = (*pages);
    while (cur->next != NULL) {
        if (cur->next->pid == id && cur->next->status == status) {
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

pageNode *getPage (pageNode **pages, int pid, char status) {
    pageNode *cur;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->pid == pid && cur->next->status == status) {
            return cur->next;
        }
        cur = cur->next;
    }
    return NULL;
}

pageNode *getNextPage (pageNode **pages, int pid, char status) {
    pageNode *cur;
    
    cur = getPage(pages, pid, status);
    if (cur == NULL) {
        return NULL;
    }
    while (cur->next != NULL) {
        if (cur->next->status == status) {
            return cur->next;
        }
        cur = cur->next;
    }

    return NULL;
}

pageNode *getPrevPage (pageNode **pages, int pid, char status) {
    pageNode *cur, *back = NULL;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->status == status) {
            if (cur->next->pid == pid) {
                if (cur == *pages) {
                    break;
                }
                return back;
            } else {
                back = cur->next;
            }
        }
        cur = cur->next;
    }
    return NULL;
}

pageNode *getPagebyOrder (pageNode **pages, int order, char status) {
    pageNode *cur;
    int i = 0;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->status == status) {
            i++;
        }
        if (i == order) {
            return cur->next;
        }
        cur = cur->next;
    }
    return NULL;
}

pageNode *getLastPage (pageNode **pages, char status) {
    pageNode *cur, *back;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->status == status) {
            back = cur->next;
        }
        cur = cur->next;
    }

    if (cur != *pages) {
        return cur;
    }

    return NULL;
}

int getPagesLen (pageNode **pages, char status) {
    pageNode *cur;
    int i = 0;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->status == status) {
            i++;
        }
        cur = cur->next;
    }

    return i;
}

int getPagesOrder (pageNode **pages, int id, char status) {
    pageNode *cur;
    int i = 0;
    
    cur = *pages;
    while (cur->next != NULL) {
        if (cur->next->status == status) {
            i++;
        }
        if (cur->next->pid == id && cur->next->status == status) {
            return i;
        }
        cur = cur->next;
    }

    return 0;
}
