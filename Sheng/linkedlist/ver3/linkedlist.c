#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

int add (Node **list, char *input) {
    Node *new_node, *cur;
    static int id = 0;
    
    id++;
    cur = *list;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    
    new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->data, "");
    if (atoi(input) == 0 || strstr(input, ",") == NULL) {
        sprintf(new_node->data, "%d,", id);
    }
    strcat(new_node->data, input);
    new_node->show = 0;
    new_node->next = NULL;

    cur->next = new_node;
    return 0;
}

int del (Node **node, char *input) {
    Node *cur;
    char *data;
    int id;

    data = (char *)malloc(101);
    cur = *node;
    while (cur->next != NULL) {
        strcpy(data, strstr(cur->next->data, ",")+1);
        id = atoi(cur->next->data);
        if (!strcmp(cur->next->data, input) || id == atoi(input) || !strcmp(input, data)) {
            cur->next->show = 1;
            free(data);
            return 0;
        }
        cur = cur->next;
    }
    free(data);
    return 1;
}

int insert (Node **node, char *insert) {
    Node *cur, *tmp;
    char *data, *input;
    int id;

    if(strstr(insert, ",") != NULL) {
        input = strstr(insert, ",")+1;
        *(strstr(insert, ",")) = '\0';
        cur = *node;
        while (cur->next != NULL) {
            data = (char *)malloc(101);
            strcpy(data, strstr(cur->next->data, ",")+1);
            id = atoi(cur->next->data);
            if (id == atoi(input) || !strcmp(input, data)) {
                if (cur->show == 1) {
                    cur->show = 0;
                    id = atoi(cur->data);
                    strcpy(cur->data, "");
                    sprintf(cur->data, "%d,", id);
                    strcat(cur->data, insert);
                } else {
                    tmp = cur->next;
                    cur->next = NULL;
                    add(node, strcat(insert, "\n\0"));
                    cur->next->next = tmp;
                }
                free(data);
                return 0;
            }
            free(data);
            cur = cur->next;
        }
    }
    return 1;
}

int save (Node **node, char *fn) {
    FILE *save, *index;
    char *index_fn, *data;
    int len, id;
    Node *cur;

    *(strstr(fn, "\n")) = '\0';
    if ((save = fopen(fn, "w")) != NULL) {

        if (strstr(fn, ".") != NULL) 
            *(strstr(fn, ".")) = '\0';
        index_fn = (char *)malloc(strlen(fn)+4);
        strcpy(index_fn, fn);
        strcat(index_fn, ".idx");

        if ((index = fopen(index_fn, "wb")) != NULL) {
            cur = (*node)->next;
            len = 0; 
            fwrite(&len, sizeof(int), 1, index);

            while (cur != NULL) {
                id = atoi(cur->data);
                if ((data = strstr(cur->data, ",")+1) == NULL)
                    break;
                if (cur->show == 1) 
                    continue;
                len += strlen(data)+sizeof(int);

                fwrite(&id, sizeof(int), 1, save);
                fwrite(data, strlen(data), 1, save);
                fwrite(&len, sizeof(int), 1, index);

                cur = cur->next;
            }
            fclose(index);
        }
        free(index_fn);
        fclose(save);
    }
    return 0;
}

/*
 *  讀取使用者選定的檔案資料進 Linkedlist 。
 *  (不使用 .idx 檔)。
 */
int load (Node **node, char *fn) {
    FILE *load;
    char *data, *output;
    int i, len, id;

    *(strstr(fn, "\n")) = '\0';
    if ((load = fopen(fn, "r")) != NULL) {
        fseek(load, 0, SEEK_END);
        len = ftell(load);
        fseek(load, 0, SEEK_SET);
        
        data = (char *)malloc(100);
        while (len) {
            output = (char *)malloc(100);
            fread(&id, sizeof(int), 1, load);
            len -= sizeof(int);
            i = 0;
            while (1) {
                fread((data+i), 1, 1, load);
                if (*(data+i) == '\n') {
                    *(data+i+1) = '\0';
                    break;
                }
                i++;
            }
            len -= i+1;

            sprintf(output, "%d,%s", id, data);
            add(node, output);
            free(output);
        }
        free(data);
        fclose(load);
    }
    return 0;
}

/*
 *  顯示目前 Linkedlist 所有的資料。
 */
int showList (Node **node, char *none) {
    Node *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        (cur->show == 1)? 0 : printf("%s\n", cur->data);
        cur = cur->next;
    }
    printf("----------------------------end----------------------------\n");
    return 0;
}

/*
 *  刪除目前 Linkedlist 所有的資料。
 */
int cleanList (Node **node, char *none) {
    Node *cur, *next;

    cur = (*node)->next;
    while (cur != NULL) {
        next = cur->next;
        
        free(cur);
        cur = next;
    }
    (*node)->next = NULL;
    return 0;
}

/*
 *  讀取使用者選定檔案中，指定的該行資料並顯示。
 *  (使用 .idx 檔)。
 */
int showN (Node **none, char *input) {
    FILE *show, *index;
    char *index_name, *data;
    int n, pos, nextpos, dataId, status;

    status = 1;
    if (strstr(input, ",") != NULL) {
        if ((n = atoi(strstr(input, ",")+1)) != 0) {

            *(strstr(input, ",")) = '\0';
            if ((show = fopen(input, "r")) == NULL) 
                return status;

            index_name = (char *)malloc(strlen(input)+4);
            strcpy(index_name, input);
            if (strstr(index_name, ".") != NULL) 
                *(strstr(index_name, ".")) = '\0';
            
            strcat(index_name, ".idx");
            if ((index = fopen(index_name, "rb")) == NULL)
                return status;

            fseek(index, 0, SEEK_END);

            if (ftell(index)/sizeof(int) > n) {
                fseek(index, (n-1)*sizeof(int), SEEK_SET); 
                fread(&pos, sizeof(int), 1, index);
                fread(&nextpos, sizeof(int), 1, index);

                data = (char *)malloc(nextpos-pos+1);
                fseek(show, pos, SEEK_SET);
                fread(&dataId, sizeof(int), 1, show);
                fread(data, nextpos-pos-sizeof(int), 1, show);
                *(data+strlen(data)) = '\0';

                printf("id = %d, data = %s\n", dataId, data);
                
                free(data);
                status = 0;
            }
            fclose(show);
            fclose(index);
            free(index_name);
        } 
    } 
    return status;
}
