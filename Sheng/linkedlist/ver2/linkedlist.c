#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

int add (Node **list, char *input) {
    Node *cur, *new_node;
    static int id = 0;
    char *data;
    
    id++;
    cur = *list;
    while (cur->next != NULL) {
        cur = cur->next;
    }

    new_node = (Node *)malloc(sizeof(Node));
    new_node->data = (Data *)malloc(sizeof(Data));

    new_node->data->id = id;
    data = input;
    if (atoi(input) != 0 && strstr(input, ",") != NULL) {
        new_node->data->id = atoi(input);
        data = strstr(input, ",")+1;
    }
    strcpy(new_node->data->input, data);
    new_node->next = NULL;
    cur->next = new_node;

    return 0;
}

int del (Node **list, char *input) {
    Node *cur, *tmp;
    int id;

    id = atoi(input);
    cur = *list;
    while (cur->next != NULL) {
        if (id == cur->next->data->id || !strcmp(cur->next->data->input, input)) {
            tmp = cur->next->next;
            free(cur->next->data);
            free(cur->next);
            cur->next = tmp;
            return 0;
        }
        cur = cur->next;
    }

    return 1;
}

int save (Node **list, char *fn) {
    FILE *save, *idx;
    char *idx_fn;
    int len;
    Node *cur;
    
    idx_fn = (char *)malloc(strlen(fn)+4);
    *(strstr(fn, "\n")) = '\0';
    save = fopen(fn, "w");

    if (strstr(fn, ".") != 0) {
        *strstr(idx_fn, ".") = '\0';
    }
    strcpy(idx_fn, fn);
    strcat(idx_fn, ".idx");
    idx = fopen(idx_fn, "w");

    cur = *list;
    len = 0;
    fwrite(&len, sizeof(int), 1, idx);

    while (1) {
        if (cur->next == NULL) {
            break;
        }
        len += strlen(cur->next->data->input) + sizeof(int);

        fwrite(&len, sizeof(int), 1, idx);
        fwrite(&cur->next->data->id, sizeof(int), 1, save);
        fwrite(&cur->next->data->input, strlen(cur->next->data->input), 1, save);
        cur = cur->next;
    }
    
    free(idx_fn);
    fclose(save);
    fclose(idx);
    return 0;
}

int load (Node **list, char *fn) {
    FILE *load, *idx;
    int curpos, nextpos, len, id;
    char *data, *idx_fn, *fileData;

    idx_fn =(char *)malloc(strlen(fn)+4);
    *(strstr(fn, "\n")) = '\0';
    if ((load = fopen(fn, "r")) != NULL) {
        if (strstr(fn, ".") != 0) {
            *strstr(idx_fn, ".")= '\0';
        }
        strcpy(idx_fn, fn);
        strcat(idx_fn, ".idx");

        if ((idx = fopen(idx_fn, "r")) != NULL) {
            fseek(idx, 0, SEEK_END);
            len = ftell(idx)/sizeof(int);
            fseek(idx, 0, SEEK_SET);

            fread(&curpos, sizeof(int), 1, idx);
            data = (char *)malloc(100);
            int n = 1;
            while (len-n) {
                fread(&nextpos, sizeof(int), 1, idx);

                fileData = (char *)malloc(nextpos - curpos);

                fread(&id, sizeof(int), 1, load);
                fread(fileData, nextpos - curpos - sizeof(int), 1, load);
                *(fileData + nextpos - curpos - sizeof(int)) = '\0';
                
                sprintf(data, "%d,", id);
                strcat(data, fileData);

                add(list, data);

                free(fileData);
                curpos = nextpos;
                n++;
            }
            free(data);
            return 0;
        }
    }

    return 1;
}

int showList (Node **list, char *none) {
    Node *cur;

    cur = *list;
    while (cur->next != NULL) {
        printf("id %d, data %s", cur->next->data->id, cur->next->data->input);
        cur = cur->next;
    }
    printf("-----------------------------END-------------------------------------\n");
    return 0;
}

int cleanList (Node **list, char *none) {
    Node *cur, *next;

    cur = *list;
    while (cur->next != NULL) {
        next = cur->next->next;
        free(cur->next->data);
        free(cur->next);
        cur->next = next;
    }
    return 0;
}

int showN (Node **none, char *input) {
    FILE *load, *idx;
    char *idx_fn;
    int state, n, curpos, nextpos, len;
    Data data;

    state = 1;
    if (strstr(input, ",") != NULL) {
        n = atoi(strstr(input, ",")+1);
        *(strstr(input, ",")) = '\0';
        if ((load = fopen(input, "r")) != NULL) {
            idx_fn = (char *)malloc(strlen(input));

            if (strstr(input, ".") != NULL) {
                *strstr(idx_fn, ".") = '\0';
            }
            strcpy(idx_fn, input);
            strcat(idx_fn, ".idx");
            if ((idx = fopen(idx_fn, "r")) != NULL) {
                fseek(idx, 0, SEEK_END);
                len = ftell(idx);
                fseek(idx, 0, SEEK_SET);
                if (len > (n-1)*sizeof(int)) {
                    fseek(idx, (n-1)*sizeof(int), SEEK_SET);
                    fread(&curpos, sizeof(int), 1, idx);
                    fread(&nextpos, sizeof(int), 1, idx);
                    fseek(load, curpos, SEEK_SET);
                    fread(&data.id, sizeof(int), 1, load);
                    fread(data.input, nextpos-curpos-sizeof(int), 1, load);
                    data.input[nextpos-curpos-sizeof(int)] = '\0';

                    printf("id %d, data %s", data.id, data.input);
                    state = 0;
                }
                fclose(idx);
            }
            free(idx_fn);
            fclose(load);
        }
    }

    return state;
}
