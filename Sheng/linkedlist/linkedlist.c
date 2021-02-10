#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char data[100];
    struct node *next;
} Node;

Node *add (Node *node, char *data); 
Node *del (Node *node, char *data);

/*     目前暫不實作資料輸入功能( CMD,data )，
 *     以主要功能為主。
 *     本次更新--del功能 (如有錯誤還請糾正...)。
 */

int main (void) {
    Node *first = NULL;
    Node *current, *next;
    
    first = add(first, "asdf");
    first = add(first, "aadd");
    first = add(first, "test");
    first = add(first, "aasdf");

    first = del(first, "aadd");
    first = del(first, "aasdf");
    first = del(first, "wws");

    current = first;
    while (current != NULL) {
        printf("current %p data %s next %p\n", current, current->data, current->next);
        next = current->next;
        free(current);
        current = next;
    }
    
    return 0;
}

Node *add (Node *node, char *data) {
    Node *new;

    new = (Node *)malloc(sizeof(Node));
    strcpy(new->data, data);
    new->next = node;
    
    return new;
}

Node *del (Node *node, char *data) {
    Node *previous = NULL;
    Node *current, *next;

    current = node;
    while (current != NULL && strcmp(current->data, data)) {
        previous = current;
        current = current->next;
    }

    if (current != NULL) {
        next = current->next;
        free(current);
        
        if ( previous != NULL) {
            previous->next = next;
        } else {
            node = next;
        }
    } else {
        printf("didn't find the data '%s'\n", data);
    }

    return node;
}

