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
void cleanList (Node **list);

int main () {
    Node **first;
    Node *current, *next;
    char *cmd;
    char cmdspac[105];

    first = (Node **)malloc(sizeof(Node *));
    cmd = cmdspac;
    
    printf("Welcome to linked list service!\n");
    printf("\nThe command format is <CMD,data>.\n"); 
    printf("commands have <add> ,<del>, <save>, <load>, <showlist>, <show>, <clear>!\n");
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
            } else {
                printf("file not found!\n");
            }
        } else if (!strncmp(cmd, "del,", 4)) {
            if (!del(first, cmd+4)) {
                printf("Command done!\n");
            } else {
                printf("didn't find the data '%s'\n", data);
            }
        } else if (!strncmp(cmd, "add,", 4)) {
            add(first, cmd+4);
            printf("Command done!\n");
        } else if (!strncmp(cmd, "showlist", 7)) {
            showList(first);
        } else if (!strncmp(cmd, "show,", 5)) {
            showN(cmd+5);
        } else if (!strncmp(cmd, "clear", 5)) {
            cleanList(first);
        } else {
            printf("error input!!Try again!~\n");
        }
    }

    cleanList(first);
    
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
    Node *current, *previous;
    
    current = *node;
    while (current != NULL ) {
        if (strcmp(current->data, data) == 0) {
            if(current == *node)
                *node = current->next;
            else
                previous->next = current->next;

            free(current);
            return 0;
        }
        previous = current;
        current = current->next;
    } 
    return 1;
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
    FILE *load, *index;
    char *temp, *name_index;
    int len, i, cur, prev, total;

    *(fname + strlen(fname)-1) = '\0';
    if ((load = fopen(fname, "r")) == NULL) {
        return 1;
    }

    name_index = (char *)malloc(strlen(fname)+6);
    if (strstr(fname,".") != NULL) {
        *(strstr(fname,".")) = '\0';
    }
    strcpy(name_index, fname);
    strcat(name_index, "_index.bin");
    if ((index = fopen(name_index, "rb")) == NULL) {
        free(name_index);
        fclose(load);
        return 1;
    }

    i = 0;
    fseek(index, -sizeof(int), SEEK_END);
    len = ftell(index)/4;

    fread(&cur, sizeof(int), 1, index);
    while (len >= i) {
        fseek(index, -2*sizeof(int), SEEK_CUR);
        fread(&prev, sizeof(int), 1, index);
        temp = (char *)malloc(cur-prev+1);

        fseek(load, prev, SEEK_SET);
        fread(temp, cur-prev, 1, load);
        *(temp+ cur-prev+1) = '\0';
        add(node, temp);
        
        free(temp);
        if (prev == 0) break;
        cur = prev;
        i++;
    }

    fclose(load);
    fclose(index);
    free(name_index);

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

void cleanList (Node **list) {
    Node *current, *next;

    current = *list;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *list = NULL;
}

/* 用於顯示檔案中第 N 筆資料的 function
*/

void showN (char *input) {
    FILE *load, *index;
    int len, n, pos, seek, fileLen;
    char *number, *data, *name_index;
    char *newline = "\n";

    if ((number = strstr(input, ",")) == NULL) {
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
    if ((load = fopen(input, "r")) == NULL) {
        printf("can't not open the file %s\n", input);
        return;
    }
    
    name_index = (char *)malloc(strlen(input)+6);
    if (strstr(input, ".") != NULL) {
        *(strstr(input, ".")) = '\0';
    }
    strcpy(name_index, input);
    strcat(name_index, "_index.bin");
    if ((index = fopen(name_index, "rb")) == NULL) {
        printf("can't not open the file %s\n", name_index);

        fclose(load);
        free(name_index);
        return;
    }

    seek = (n-1)*sizeof(int);
    fseek(index, 0, SEEK_END);
    fileLen = ftell(index);
    if (fileLen < seek) {
        printf("list out of range!\n");

        fclose(load);
        fclose(index);
        free(name_index);
        return;
    }
    fseek(index, seek, SEEK_SET);
    fread(&pos, sizeof(int), 1, index);
    fread(&len, sizeof(int), 1, index);
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
