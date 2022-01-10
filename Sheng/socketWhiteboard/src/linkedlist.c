#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedlist.h"

/*
 *  新增資料在 Linkedlist 中
 */
int add (Node_p **node, int id, Pos *pts, size_t length) {
    Node_p *new_node, *cur;

    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (Node_p *)malloc(sizeof(Node_p));
    new_node->id = id;
    new_node->len = length/sizeof(Pos);
    new_node->p = (Pos *)malloc(length);
    memcpy(new_node->p, pts, length);
    new_node->next = NULL;

    cur->next = new_node;
    return 0;
}

/*
 *  在 Linkedlist 中刪除指定資料
 */
int del (Node_p **node, Pos *pts) {
    Node_p *cur, *tmp;

    cur = *node;
    while (cur->next != NULL) {
        if (!memcmp(cur->next->p, pts, sizeof(Pos)*cur->next->len)) {
            tmp = cur->next->next;
            free(cur->next->p);
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
int showList (Node_p **node) {
    Node_p *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        for (int i = 0; i < cur->len; i++) {
            printf("x: %d, y: %d\n", cur->p[i].x, cur->p[i].y);
        }
        printf("\n");
        cur = cur->next;
    }
    printf("----------------------------end----------------------------\n");
    return 0;
}

/*
 *  刪除目前 Linkedlist 所有的資料。
 */
int cleanList (Node_p **node) {
    Node_p *cur, *next;

    cur = (*node)->next;
    while (cur != NULL) {
        next = cur->next;

        free(cur->p);
        free(cur);
        cur = next;
    }
    (*node)->next = NULL;
    return 0;
}

int socket_write (int fd, int id, Node_p **node) {
    Node_p *cur, *next;
    Pos tmp;

    cur = (*node);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    tmp.x = cur->len;
    tmp.y = id;
    write(fd, &tmp, sizeof(Pos));
    write(fd, cur->p, cur->len*sizeof(Pos));

    return 0;
}

int socket_read (int fd, int id, Node_p **node, int size) {
    Pos *pts;
    int n;

    pts = (Pos *)malloc(sizeof(Pos)*size);
    if ((n = read(fd, pts, sizeof(Pos)*size)) > 0) {
        add(node, id, pts, n);
        free(pts);
        return n;
    }
    free(pts);
    return 0;
}
/*
int IDdelete (Node_p **node, int id) {
    Node_p *cur, *next;
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
