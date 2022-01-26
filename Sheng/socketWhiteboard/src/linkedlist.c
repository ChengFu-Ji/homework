#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedlist.h"

/*
 *  新增一筆資料在 Linkedlist 中
 */
int add (Node_s **node, Pos pt) {
    Node_s *new_node, *cur;

    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (Node_s *)malloc(sizeof(Node_s));
    new_node->point = pt;
    new_node->next = NULL;

    cur->next = new_node;
    return 0;
}

/*
 *  在 Linkedlist 中刪除指定資料
 */
int del (Node_s **node, Pos pt) {
    Node_s *cur, *tmp;

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
int showList (Node_s **node) {
    Node_s *cur;

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
int cleanList (Node_s **node) {
    Node_s *cur, *next;

    cur = (*node)->next;
    while (cur != NULL) {
        next = cur->next;
        
        free(cur);
        cur = next;
    }
    (*node)->next = NULL;
    return 0;
}

int socket_write (Node_s **node, int fd, int size) {
    Node_s *cur, *next;
    Pos *pts;
    int i;

    pts = (Pos *)malloc(sizeof(Pos)*size);
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
    write(fd, &i, sizeof(int));
    write(fd, pts, i*sizeof(Pos));
    free(pts);
    return 0;
}

int socket_read (Node_s **node, int fd, int size) {
    Pos *pts;
    int n;

    pts = (Pos *)malloc(sizeof(Pos)*size);
    if ((n = read(fd, pts, sizeof(Pos)*size)) > 0) {
        for (int i = 0; i < n/sizeof(Pos); i++) {
            if ((pts+i)->x == -1 && (pts+i)->y == 0) {
                return n/sizeof(Pos);
            }
            add(node, *(pts+i));
        }
    }
    return 0;
}
/*
int IDdelete (Node_s **node, int id) {
    Node_s *cur, *next;
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

void listcat(Node_s **det, Node_s **src) {
    Node_s *cur;

    cur = (*det);
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = (*src)->next;
}
