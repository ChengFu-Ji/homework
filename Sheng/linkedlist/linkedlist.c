#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  建立新的形態Node
 */

typedef struct node {
    char data[100];
    struct node *next;
} Node;

void add (Node **node, char *data); 
int del (Node **node, char *data);
int load (Node **node, char *fname);
void save (Node **node, char *fname);
void showList (Node **list);
void showN (char *data);

int main (void) {
    Node **first;
    Node *current, *next;
    char *cmd;
    char cmdspac[105];

    first = (Node **)malloc(sizeof(Node *));
    cmd = cmdspac;
    
    printf("Welcome to linked list service!\n");
    printf("\nThe command format is <CMD,data>.\n"); 
    printf("commands have <add> ,<del>, <save>, <load>, <showlist>, <show>!\n");
    printf("ex. 'add,data' , 'save,filename' , 'showlist' , 'show,filename,number'.\n");
    printf("\nEnter 'exit' to Exit!\n");
    
    while (1) {
        printf(">> ");
        fgets(cmd, 105, stdin);

        if(!strncmp(cmd, "exit", 4)) {
            printf("bye!\n");
            break;
        } else if (!strncmp(cmd, "save,", 5)) {
            save(first, cmd+5);
            printf("Command done!\n");
        } else if (!strncmp(cmd, "load,", 5)) {
            if (!load(first, cmd+5)) {
                printf("Command done!\n");
            }
        } else if (!strncmp(cmd, "del,", 4)) {
            if (!del(first, cmd+4)) {
                printf("Command done!\n");
            }
        } else if (!strncmp(cmd, "add,", 4)) {
            add(first, cmd+4);
            printf("Command done!\n");
        } else if (!strncmp(cmd, "showlist", 7)) {
            showList(first);
        } else if (!strncmp(cmd, "show,", 5)) {
            showN(cmd+5);
        } else {
            printf("error input!!Try again!~\n");
        }
    }

    current = *first;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(first);
    return 0;
}

/*  用於將新的資料加入 linked list 的 function
 *  作法:將新的資料加於最前面
 */

void add (Node **node, char *data) {
    Node *new;

    new = (Node *)malloc(sizeof(Node));
    strcpy(new->data, data);
    new->next = *node;
    
    *node = new;
}

/*  用於將指定資料從目前的 linked list 刪除的 function
 */

int del (Node **node, char *data) {
    Node *previous = NULL;
    Node *current, *next;
    

    current = *node;
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
            *node = next;
        }
    } else {
        printf("didn't find the data '%s'\n", data);
        return 1;
    }

    return 0;
}

/*  將目前 linked list 的資料存入檔案的 function
 */

void save (Node **node, char *fname) {
    FILE *save;
    Node *current;

    *(fname + strlen(fname)-1) = '\0';
    save = fopen(fname, "w");
    current = *node;

    while (current != NULL) {
        fprintf(save, "%s", current->data);
        current = current->next;
    }

    fclose(save);
}

/*  用於讀取目標檔案並將資料存入 linked list 的 function
 */

int load (Node **node, char *fname) {
    FILE *load;
    Node *templist, *next;
    char *data, *current, *temp;
    int len, i, lenstr;

    *(fname + strlen(fname)-1) = '\0';
    load = fopen(fname, "r");

    if (load == NULL) {
        printf("file does not find!\n");
        return 1;
    }

    temp = (char *)malloc(100);
    templist = NULL;
    lenstr = 0;
    i = 0;
    
    fseek(load, 0, SEEK_END);
    len = ftell(load);
    data = (char *)malloc(len+1);

    fseek(load, 0, SEEK_SET);
    fread(data, len, 1, load);

    current = data;
    while (len-i) {
        if (*(current+i) == '\n') {
            *(current+i) = '\0';

            strcpy(temp, current+lenstr);
            strcat(temp, "\n");
            
            add(&templist, temp);
            lenstr = i+1;
        }
        i++;
    }

    while (templist != NULL) {
        add(node, templist->data);
        next = templist->next; 
        free(templist);
        templist = next;
    }

    fclose(load);
    free(data);
    free(temp);

    return 0;
}

/*  用於顯示目前 linked list 的所有資料的 function
 */

void showList (Node **list) {
    Node *current;

    current = *list;
    while (current != NULL) {
        printf("data %s\n", current->data);
        current = current->next;
    }
}

void showN (char *data) {
    FILE *load;
    int count, len, n;
    char *charN, *cur, *fdata, *dataN;
    char *newline = "\n";

    charN = strstr(data, ",");
    if (charN == NULL) {
        printf("Error Type input!!\n");
        return;
    }

    *(charN++) = '\0';
    *(strstr(charN, newline)) = '\0';
    n = atoi(charN);
    load = fopen(data, "r");
    
    fseek(load, 0, SEEK_END);
    len = ftell(load);
    fdata = (char *)malloc(len+1);

    fseek(load, 0, SEEK_SET);
    fread(fdata, len, 1, load);

    cur = fdata;
    dataN = cur;
    count = 1;
    while ((cur = strstr(cur, newline)) != NULL) {
        if (count == n-1) {
            dataN = cur+1;
        } else if (count == n) {
           *(cur) = '\0'; 
           break;
        }
        cur++;
        count++;
    }

    if (cur != NULL) {
        printf("data '%s'\n", dataN);
    } else {
        printf("data not find\n");
    }
    
    fclose(load);
    free(fdata);
} 
