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

int addStroke (strokeNode **strokes, posNode **pos) {
    strokeNode *cur, *newNode;

    cur = (*strokes);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    newNode = (strokeNode *) malloc(sizeof(strokeNode));
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
