#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  建立新的形態Node
 */

typedef struct node {
    char data[100];
    struct node *next;
} Node;

Node *add (Node *node, char *data); 
Node *del (Node *node, char *data);
Node *load (Node *node, char *fname);
void save (Node *node, char *fname);
void showList (Node *list);

int main (void) {
    Node *first = NULL;
    Node *current, *next;
    char *cmd;
    char cmdspac[103];

    cmd = cmdspac;
    
    printf("Welcome to linked list service!\n");
    printf("\nThe command format is <CMD,data>.\n"); 
    printf("commands have <add> ,<del>, <save>, <load>, <showlist>!\n");
    printf("ex. 'add,data' , 'save,filename' , 'showlist'.\n");
    printf("\nEnter '--exit' to Exit!\n");
    
    while (1) {
        printf(">> ");
        fgets(cmd, 103, stdin);

         if(!strncmp(cmd, "--exit", 6)) {
             printf("bye!\n");
             break;
         }


        if (!strncmp(cmd, "save", 4) 
         || !strncmp(cmd, "load", 4)
         || !strncmp(cmd, "del", 3)
         || !strncmp(cmd, "add", 3)) {
            switch (*cmd) {
                case 'a':
                    first = add(first, cmd+4);
                    break;
                case 'd':
                    first = del(first, cmd+4);
                    break;
                case 'l':
                    first = load(first, cmd+5);
                    break;
                case 's':
                    save(first, cmd+5);
                    break;
            }
            printf("Command done!\n");
        } else {
            if (!strncmp(cmd, "showlist", 7)) {
                showList(first);
                continue;
            }
            printf("error input!!Try again!~\n");
        }
    }

    current = first;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    return 0;
}

/*  用於將新的資料加入 linked list 的 function
 *  作法:將新的資料加於最前面
 */

Node *add (Node *node, char *data) {
    Node *new;

    new = (Node *)malloc(sizeof(Node));
    strcpy(new->data, data);
    new->next = node;
    
    return new;
}

/*  用於將指定資料從目前的 linked list 刪除的 function
 */

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

/*  將目前 linked list 的資料存入檔案的 function
 */

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

/*  用於讀取目標檔案並將資料存入 linked list 的 function
 */

Node *load (Node *node, char *fname) {
    FILE *load;
    Node *templist, *next;
    char data[100];

    load = fopen(fname, "r");
    templist = NULL;

    while (fgets(data, 100, load) != NULL) {
        if (!strcmp(data, "\n") || !strcmp(data, "")) {
            continue;
        }
        templist = add(templist, data);
    } 

    while (templist != NULL) {
        node = add(node, templist->data);
        next = templist->next; 
        free(templist);
        templist = next;
    }

    fclose(load);
    return node;
}

/*  用於顯示目前 linked list 的所有資料的 function
 */

void showList (Node *list) {
    Node *current;

    current = list;
    while (current != NULL) {
        printf("data %s\n", current->data);
        current = current->next;
    }
}
