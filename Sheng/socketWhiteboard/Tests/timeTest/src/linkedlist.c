#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedlist.h"

/*
 *  新增一筆資料在 Linkedlist 中
 */
int add (Node_s **node, Data_s pt) {
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
int del (Node_s **node, Data_s pt) {
    Node_s *cur, *tmp;

    cur = *node;
    while (cur->next != NULL) {
        if (!memcmp(&(cur->next->point), &pt, sizeof(Data_s))) {
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
    Data_s *ptp, ptarr[size];
    int i, n, times = 5;

    ptp = ptarr;
    cur = (*node)->next;
    while (cur != NULL) {
        *(ptp++) = cur->point;
        cur = cur->next;
    }
    while ((n = write(fd, &size, sizeof(int))) != sizeof(int) && times--);
    if (n == sizeof(int)) {
        n = 0;
        times = 5;
        n = write(fd, ptarr, size*sizeof(Data_s));
        while (n < sizeof(Data_s)*size && times--) {
            n += write(fd, ptarr+(n/sizeof(Data_s)), (size)*sizeof(Data_s)-n);
        }
        if (n < sizeof(Data_s) * size) {
            return -1;
        }
    } else {
        return -1;
    }
    return 0;
}

int socket_read (Node_s **node, int fd, int size) {
    Data_s *pts;
    int n, times = 5;

    pts = (Data_s *)malloc(sizeof(Data_s)*size);
    if ((n = read(fd, pts, sizeof(Data_s)*size)) > 0) {
        while (n < sizeof(Data_s)*size && times--) {
            n += read(fd, pts, sizeof(Data_s)*size);
        }
        for (int i = 0; i < n/sizeof(Data_s); i++) {
            if ((pts+i)->x == -1 && (pts+i)->y == 0) {
                return i;
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
