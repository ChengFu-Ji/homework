#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list {
    char data[100];
    struct list *next;
};

typedef struct list List;

int main (void) {
    List *first, *current, *prev;
    int i;

    current = (List *)malloc(sizeof(List));
    fgets(current->data, 100, stdin);
    first = current;
    current->next = NULL;
    prev = current;
    current = (List *)malloc(sizeof(List));
    fgets(current->data, 100, stdin);
    prev->next = current;
    current->next = NULL;
    prev = current;

    current = first;
    printf("current %p data %s, next %p\n", current, current->data, current->next);
    current= current->next;
    printf("current %p data %s, next %p\n", current, current->data, current->next);
    return 0;
}

