#include "linkedlist/stroke.h"

int initStroke (strokeNode ***strokePtr) {
    if ((*strokePtr = (strokeNode **) malloc(sizeof(strokeNode *))) == NULL) {
        return -1;
    }

    if ((**strokePtr = (strokeNode *) malloc(sizeof(strokeNode))) == NULL) {
        return -1;
    }
    (**strokePtr)->next = NULL;

    return 0;
}

int freeStroke (strokeNode **stroke) {
    free(*stroke);
    free(stroke);
    return 0;
}

int addStroke(strokeNode **strokes, posNode **pos, int thk, int is_eraser) { 
    strokeNode *cur, *newNode;

    cur = (*strokes);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    newNode = (strokeNode *) malloc(sizeof(strokeNode));
    newNode->is_eraser = is_eraser;
    newNode->thickness = thk;
    initPos(&newNode->pos);
    catPosList(newNode->pos, pos);

    newNode->next = NULL;
    
    cur->next = newNode;
    return 0;
}

int delLastStroke(strokeNode **strokes) {
    strokeNode *cur, *next;

    cur = (*strokes);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    next = cur->next->next;

    deleteAllPos(cur->next->pos);
    freePos(cur->next->pos);

    free(cur->next);

    return 0;
}

int deleteAllStrokes(strokeNode **strokes) {
    strokeNode *cur, *next;

    cur = (*strokes);
    while (cur->next != NULL) {
        next = cur->next->next;

        deleteAllPos(cur->next->pos);
        freePos(cur->next->pos);
        free(cur->next);

        cur->next = next;
    }
    return 0;
}

int showStrokeList(strokeNode **strokes) {
    strokeNode *cur;

    cur = (*strokes);
    while (cur->next != NULL) {
        showPosList(cur->next->pos);

        cur = cur->next;
    }
    return 0;
}

strokeNode *getLastStroke (strokeNode **strokes) {
    strokeNode *cur;
    cur = (*strokes);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    return cur; 
}

int sendStroke (int fd, strokeNode *stroke, SockCond sender) {
    int n = write(fd, &sender, sizeof(SockCond));
    if (n < sizeof(SockCond)) {
        return -1;
    } else if (sender.drawpid != -1 && sender.poslen != 0) {
        socket_write(fd, stroke->pos, sender.poslen);
    }

    return 0;
}

int sendStrokeList (int fd, strokeNode **strokes, SockCond sender) {
    strokeNode *cur;

    cur = *strokes;
    while (cur->next != NULL) {
        if (sendStroke(fd, cur, sender)) {
            return -1;
        }
        cur = cur->next;
    }

    return 0;
}


