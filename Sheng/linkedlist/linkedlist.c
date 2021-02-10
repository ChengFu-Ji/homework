#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char data[100];
    struct node *next;
} Node;

Node *add(Node *node, char *data); 
void del (int *len, char *data);

/*     目前暫不實作資料輸入功能( CMD,data )，
 *     以主要功能為主。
 *     del 功能暫時關閉。
 *     本次更新--add功能修正 (如有錯誤還請糾正...)。
 */

int main (void) {
    Node *first = NULL;
    Node *current, *next;
    
    first = add(first, "asdf");
    first = add(first, "aadd");
    first = add(first, "test");

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

/* void del (char *data) {
 * }
 */
