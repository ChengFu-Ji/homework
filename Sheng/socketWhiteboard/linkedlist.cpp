#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedlist.h"

/*
 *  新增一筆資料在 Linkedlist 中，
 *  如果使用者未輸入 id 則會自動補上。
 */
int add (Node_s **node, int x, int y) {
    Node_s *new_node, *cur;

    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (Node_s *)malloc(sizeof(Node_s));
    new_node->point.x = x;
    new_node->point.y = y;
    new_node->next = NULL;

    cur->next = new_node;
    return 0;
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

int socket_write (Node_s **node, int fd) {
    Node_s *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        write(fd, &(cur->point), sizeof(cur->point));

        cur = cur->next;
    }
    return 0;
}

int socket_read (Node_s **node, int fd) {
    Data_s pt;

    while (read(fd, &(pt), sizeof(pt)) > 0) {
        if (pt.x == -1) {
            return 0;
        }
        add(node, pt.x, pt.y);
    }
    return 1;
}
