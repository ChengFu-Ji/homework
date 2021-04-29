#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data_s {
    char data[100];
    int id;
} Data;
typedef struct node_s {
    Data *input;
    struct node_s *next;
} Node;

void add(Node **, Data);
void save(Node **, char *);
void load(Node **, char *);
void showList(Node **);
void cleanList(Node **);
void showN(char *);
int del(Node **, Data);

int main() {
    Node **first;
    Data data;
    char *cmd, cmdspc[105];
    int id;

    cmd = cmdspc;
    first = (Node **)malloc(sizeof(Node *));
    *first = (Node *)malloc(sizeof(Node));
    (*first)->input = NULL;
    (*first)->next = NULL;
    id = 1;

    printf("Welcome!\n");
    printf("\ncommands : <add>, <del,[id/data]>, <save>, <load>, <showlist>, <cleanlist>\n");
    printf("ex : [add,<data>], [del,id=<id> or del,data=<data>], [save,filename], [showlist]\n\n");

    while (1) {
        printf(">> ");
        fgets(cmd, 105, stdin);

        if (!strncmp(cmd, "add,", 4)) {
            strcpy(data.data, cmd+4); 
            data.id = id++;
            add(first, data);
            printf("Command Success\n");
        } else if (!strncmp(cmd, "del,", 4)) {
            if (!strncmp(cmd+4, "id=", 3)) {
                data.id = atoi(cmd+7);
                strcpy(data.data, "\0");
            } else if (!strncmp(cmd+4, "data=", 5)) {
                data.id = 0;
                strcpy(data.data, cmd+9);
            } else {
                printf("wrong input of del!\n");
                continue;
            }
            if (!del(first, data)) 
                printf("Command Success\n");
            else 
                printf("Failed to process\n");
        } else if (!strncmp(cmd, "save,", 5)) {
            save(first, cmd+5);
            printf("Command Done\n");
        } else if (!strncmp(cmd, "load,", 5)) {
            load(first, cmd+5);
            printf("Command Done\n");
        } else if (!strncmp(cmd, "show,", 5)) {
            showN(cmd+5);
        } else if (!strcmp(cmd, "showlist\n")) {
            showList(first);
        } else if (!strcmp(cmd, "cleanlist\n")) {
            cleanList(first);
        } else if (!strcmp(cmd, "exit\n")) {
            printf("exiting...\n");
            break;
        } else {
            printf("wrong input. Try again!\n");
        }
    }

    cleanList(first);
    free(*first);
    free(first);
    return 0;
}

void add (Node **node, Data data) {
    Node *new_node, *cur;

    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (Node *)malloc(sizeof(Node));
    new_node->input = (Data *)malloc(sizeof(Data));

    strcpy(new_node->input->data, data.data);
    new_node->input->id = data.id;
    new_node->next = NULL;

    cur->next = new_node;
}

int del (Node **node, Data data) {
    Node *cur, *rm;

    cur = *node;
    while (cur->next != NULL) {
        if (!strcmp(cur->next->input->data, data.data) || cur->next->input->id == data.id) {
            rm = cur->next;
            cur->next = rm->next;
            free(rm);
            return 0;
        }
        cur = cur->next;
    } 

    return 1;
}

void save (Node **node, char *fn) {
    FILE *save, *index;
    char *index_name;
    int len;
    Node *cur;

    *(strstr(fn, "\n")) = '\0';
    if ((save = fopen(fn, "w")) != NULL) {

        if (strstr(fn, ".") != NULL) 
            *(strstr(fn, ".")) = '\0';
        index_name = (char *)malloc(strlen(fn)+4);
        strcpy(index_name, fn);
        strcat(index_name, ".idx");
        if ((index = fopen(index_name, "wb")) != NULL) {
            cur = (*node)->next;
            len = 0; 
            fwrite(&len, sizeof(int), 1, index);
            while (cur != NULL) {
                fwrite(&cur->input->id, sizeof(int), 1, save);
                fwrite(cur->input->data, strlen(cur->input->data), 1, save);
                len += sizeof(int)+strlen(cur->input->data);
                fwrite(&len, sizeof(int), 1, index);
                cur = cur->next;
            }

            fclose(index);
        }

        free(index_name);
        fclose(save);
    }
}

void load (Node **node, char *fn) {
    Data file_in;
    FILE *load, *index;
    char *index_name;
    int i, len, cur, next;

    *(strstr(fn, "\n")) = '\0';
    if ((load = fopen(fn, "r")) != NULL) {
        if (strstr(fn, ".") != NULL) 
            *(strstr(fn, ".")) = '\0';
        index_name = (char *)malloc(strlen(fn)+4);
        strcpy(index_name, fn);
        strcat(index_name, ".idx");
        if ((index = fopen(index_name, "rb")) != NULL) {
            fseek(index, 0, SEEK_END);
            len = ftell(index)/sizeof(int);
            fseek(index, 0, SEEK_SET);

            i = 1;
            fread(&cur, sizeof(int), 1, index);
            while (1) {
                if (i == len) break;
                fread(&next, sizeof(int), 1, index);
                fread(&file_in.id, sizeof(int), 1, load);
                fread(&file_in.data, next-cur-sizeof(int), 1, load);

                file_in.data[next-cur-sizeof(int)] = '\0';
                add(node, file_in);

                cur = next;
                i++;
            }
            
            fclose(index);
        }
        
        free(index_name);
        fclose(load);
    }
}

void showList (Node **node) {
    Node *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        printf("id %d, Data %s\n", cur->input->id, cur->input->data);
        cur = cur->next;
    }
    printf("----------------------------end----------------------------\n");
}

void cleanList (Node **node) {
    Node *cur, *next;

    cur = (*node)->next;
    while (cur != NULL) {
        next = cur->next;
        
        free(cur->input);
        free(cur);
        cur = next;
    }
    (*node)->next = NULL;
    printf("Clean Done!\n");
}

void showN (char *input) {
    FILE *show, *index;
    char *index_name, *data;
    int n, Flen, pos, nextpos, dataId;

    if (strstr(input, ",") != NULL) {
        if ((n = atoi(strstr(input, ",")+1)) != 0) {

            *(strstr(input, ",")) = '\0';
            if ((show = fopen(input, "r")) == NULL) 
                return;

            index_name = (char *)malloc(strlen(input)+4);
            if (strstr(index_name, ".") != NULL) 
                *(strstr(index_name, ".")) = '\0';
            
            strcpy(index_name, input);
            strcat(index_name, ".idx");
            if ((index = fopen(index_name, "rb")) == NULL)
                return;

            fseek(index, 0, SEEK_END);
            Flen = ftell(index);
            if (Flen/sizeof(int) > n) {
                fseek(index, (n-1)*sizeof(int), SEEK_SET); 
                fread(&pos, sizeof(int), 1, index);
                fread(&nextpos, sizeof(int), 1, index);

                data = (char *)malloc(nextpos-pos-sizeof(int)+1);
                fseek(show, pos, SEEK_SET);
                fread(&dataId, sizeof(int), 1, show);
                fread(data, nextpos-pos-sizeof(int), 1, show);
                *(data+strlen(data)) = '\0';
                printf("id = %d, data = %s\n", dataId, data);
                
                free(data);
            } else {
                printf("error range!\n");
            }
            fclose(show);
            fclose(index);
            free(index_name);
        } else {
            printf("error range!\n");
        }
    } else {
        printf("wrong input!\n");
    }
}
