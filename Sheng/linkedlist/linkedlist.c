#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char data[100];
    struct node *next;
} Node;

Node *add (Node *node, char *data); 
Node *del (Node *node, char *data);
void save (Node *node, char *fname);
Node *load (Node *node, char *fname);

/*     目前暫不實作資料輸入功能( CMD,data )，
 *     以主要功能為主。
 *     本次更新-- save and load 功能 (如有錯誤還請糾正...)。
 *     save 功能在實作時還好沒什麼大礙，
 *     但是，load 功能實作時因為自己 linked list 的建立是加在最前面，
 *     所以，在使用 save 功能後，使用 load 功能資料會是倒過來的情況
 *     最後，使用了不怎麼好的方法解決...
 */

int main (void) {
    Node *first = NULL;
    Node *current, *next;
    
    first = add(first, "asdf ls is ok");
    first = add(first, "aadd\n");
    first = add(first, "test");

    save(first, "test.txt");

    first = load(first, "test.txt");

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

void save (Node *node, char *fname) {
    FILE *save;
    Node *current;

    save = fopen(fname, "w");
    current = node;

    while (current != NULL) {
        fprintf(save, "%s\n", current->data);
        current = current->next;
    }

    fclose(save);
}

Node *load (Node *node, char *fname) {
    FILE *load;
    char *data[100];
    int i;

    load = fopen(fname, "r");
    i = 0;

    while (!feof(load)) {
        data[i] = (char *)malloc(100);
        fgets(data[i], 100, load);
        
        if (!strcmp(data[i], "\n") || !strcmp(data[i], "")) {
            continue;
        }
        i++;
    }

    while (i--) {
        node = add(node, data[i]);
        free(data[i]);
    }

    fclose(load);
    return node;
}
