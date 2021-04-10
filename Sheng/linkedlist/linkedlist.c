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
void showN (char *input);

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
    FILE *save, *index;
    char *name_index;
    Node *current;
    int len;

    name_index = (char *)malloc(strlen(fname)+6);
    *(fname + strlen(fname)-1) = '\0';
    save = fopen(fname, "w");
    
    if (strstr(fname,".") != NULL) {
        *(strstr(fname,".")) = '\0';
    }
    strcpy(name_index, fname);
    strcat(name_index, "_index.bin");
    index = fopen(name_index, "wb");

    current = *node;
    len = 0;
    while (current != NULL) {
        fprintf(save, "%s", current->data);
        fwrite(&len, sizeof(int), 1, index);
        len += strlen(current->data);
        current = current->next;
    }
    fwrite(&len, sizeof(int), 1, index);

    fclose(save);
    fclose(index);
    free(name_index);
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

/* 用於顯示檔案中第 N 筆資料的 function
*/

void showN (char *input) {
    FILE *load, *index;
    int len, n, pos, seek;
    char *number, *data, *name_index;
    char *newline = "\n";

    number = strstr(input, ",");
    if (number == NULL) {
        printf("Error Type input!!\n");
        return;
    }

    *(number++) = '\0';
    *(strstr(number, newline)) = '\0';
    n = atoi(number);
    if (n <= 0) {
        printf("list out of range\n");
        return;
    }
    load = fopen(input, "r");
    if (load == NULL) {
        printf("can't not open the file %s\n", input);

        fclose(load);
        return;
    }
    
    name_index = (char *)malloc(strlen(input)+6);
    if (strstr(input, ".") != NULL) {
        *(strstr(input, ".")) = '\0';
    }
    strcpy(name_index, input);
    strcat(name_index, "_index.bin");
    index = fopen(name_index, "rb");
    if (index == NULL) {
        printf("can't not open the file %s\n", name_index);

        fclose(load);
        fclose(index);
        free(name_index);
        return;
    }

    seek = (n-1)*sizeof(int);
    fseek(index, seek, SEEK_SET);
    fread(&pos, sizeof(int), 1, index);
    fread(&len, sizeof(int), 1, index);
    if (pos < 0 || len < 0) {
        printf("list out of range\n");

        fclose(load);
        fclose(index);
        free(name_index);
        return;
    }
    len -= pos;

    fseek(load, pos, SEEK_SET);
    data = (char *)malloc(len);
    if(fread(data, len, 1, load)) {
        printf("data %s\n", data);
    } else {
        printf("data not find!!\n");
    }

    fclose(load);
    fclose(index);
    free(name_index);
    free(data);
} 
