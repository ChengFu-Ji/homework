#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add(int *len, char *data); 
void del (int *len, char *data);

struct list {
    char data[100];
    struct list *next;
};

typedef struct list List;
List *first, *current, *prev;

int main (void) {
    int len = 0;
    int *lptr = &len;
    char data[100];
    char *dataptr = data;

    add(lptr, "123");
    fgets(dataptr, 100, stdin);
    add(lptr, dataptr);
    fgets(dataptr, 100, stdin);
    add(lptr, dataptr);
    del(lptr, "123");

    current = first;
    while (current != NULL) {
        printf("current %p  data %s next %p\n", current, current->data, current->next);
        prev = current->next;
        free(current);
        current = prev;
    }
    return 0;
}

void add (int *len, char *data) {
    current = (List *)malloc(sizeof(List));
    
    if (*len == 0) {
        first = current;
    } else {
        prev->next = current;
    }
    strcpy(current->data, data);
    current->next = NULL;
    prev = current;

    *len = *len + 1;
}

void del (int *len, char *data) {
    int i = *len;

    current = first;
    for (; i > 0; i--) {
        if (strcmp(current->data, data)) {
            current = current->next;
        } else {
            memset(current->data, '\0', 1);
        }
    }
}
