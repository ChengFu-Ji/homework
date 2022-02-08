#include "linkedlist/pos.h"

int initPos (posNode ***posPtr) {
    if ((*posPtr = (posNode **) malloc(sizeof(posNode *))) == NULL) {
        return -1;
    }

    if ((**posPtr = (posNode *) malloc(sizeof(posNode))) == NULL) {
        return -1;
    }
    (**posPtr)->next = NULL;

    return 0;
}

int freePos (posNode **pos) {
    free(*pos);
    free(pos);
    return 0;
}
/*
 *  新增一筆資料在 Linkedlist 中
 */
int addPos (posNode **node, Pos pt) {
    posNode *new_node, *cur;

    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (posNode *)malloc(sizeof(posNode));
    new_node->point = pt;
    new_node->next = NULL;

    cur->next = new_node;
    return 0;
}

/*
 *  在 Linkedlist 中刪除指定資料
 */
int delPos (posNode **node, Pos pt) {
    posNode *cur, *tmp;

    cur = *node;
    while (cur->next != NULL) {
        if (!memcmp(&(cur->next->point), &pt, sizeof(Pos))) {
            tmp = cur->next->next;
            free(cur->next); 
            cur->next = tmp;
            return 0;
        }
        cur = cur->next;
    }

    return 1;
}

/*
 *  顯示目前 Linkedlist 所有的資料。
 */
int showPosList (posNode **node) {
    posNode *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        printf("x: %d, y: %d\n", cur->point.x, cur->point.y);
        cur = cur->next;
    }
    printf("----------------------------end----------------------------\n");
    return 0;
}

/*
 *  刪除目前 Linkedlist 所有的資料。
 */
int deleteAllPos (posNode **node) {
    posNode *cur, *next;

    cur = (*node)->next;
    while (cur != NULL) {
        next = cur->next;
        
        free(cur);
        cur = next;
    }
    (*node)->next = NULL;
    return 0;
}

int socket_write (int fd, posNode **node, int length) {
    posNode *cur, *next;
    Pos *pts;
    int i;

    pts = (Pos *)malloc(sizeof(Pos)*length);
    i = 0;
    cur = (*node)->next;
    while (cur != NULL) {
        pts[i++] = cur->point;
        /*
        next = cur->next->next;
        del(node, cur->point);
        */
        cur = cur->next;
    }
    //write(fd, &i, sizeof(int));
    write(fd, pts, i*sizeof(Pos));
    free(pts);
    return 0;
}

int socket_read (int fd, posNode **node, int length) {
    Pos *pts;
    int n;

    pts = (Pos *)malloc(sizeof(Pos)*length);
    if ((n = read(fd, pts, sizeof(Pos)*length)) > 0) {
        for (int i = 0; i < n/sizeof(Pos); i++) {
            addPos(node, *(pts+i));
            if ((pts+i)->x == -1 && (pts+i)->y == 0) {
                return n/sizeof(Pos);
            }
        }
    }
    return 0;
}

/*
int IDdelete (posNode **node, int id) {
    posNode *cur, *next;
    int x, y;

    cur = *node;
    while (cur->next != NULL) {
        next = cur->next->next;
        if (id == cur->next->point.id) {
            x = cur->next->point.x;
            y = cur->next->point.y;
            free(cur->next);
            cur->next = next;
            if (x == -1 && y == 0) {
                break;
            }
        } else {
            cur = cur->next;
        }
    }
    return 0;
}

*/
int catPosList (posNode **det, posNode **src) {
    posNode *cur;

    cur = (*det);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    if ((*src)->next != NULL) {
        cur->next = (*src)->next;
        return 0;
    } else {
        return -1;
    }
}

