#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node_s {
    char data[100];
    struct node_s *next;
} Node;

void add(Node **, char *);
void save(Node **, char *);
void load(Node **, char *);
void showList(Node **);
void cleanList(Node **);
void showN(char *);
int del(Node **, char *);

int main() {
    Node **first;
    char *cmd, cmdspc[105], *addon;
    int id;

    cmd = cmdspc;
    addon = (char *)malloc(100);
    first = (Node **)malloc(sizeof(Node *));
    *first = (Node *)malloc(sizeof(Node));
    (*first)->next = NULL;
    id = 1;

    printf("Welcome!\n");
    printf("\ncommands : <add>, <del>, <save>, <load>, <showlist>, <cleanlist>, <show,,>\n");
    printf("ex : [add,(id,)'data'], [del,id or del,'data'], [save,filename], [showlist], [show,filename,number]\n");
    printf("p.s. id can auto-fill and must use '' to encase data\n\n");


    while (1) {
        printf(">> ");
        fgets(cmd, 105, stdin);

        if (!strncmp(cmd, "add,", 4)) {
            if (strstr(cmd, "'") != NULL && strstr(strstr(cmd, "'")+1, "'") != NULL) {
                if (strstr(cmd+4, ",") > strstr(cmd+4, "'") || strstr(cmd+4, ",") == NULL) 
                    sprintf(addon, "%d,%s", id, cmd+4);
                else 
                    strcpy(addon, cmd+4);
                
                add(first, addon);
                id++;
                printf("Command Success\n");
            } else {
                printf("Use '' to encase data\n");
            }

        } else if (!strncmp(cmd, "del,", 4)) {
            if (strstr(cmd, "'") != NULL && strstr(strstr(cmd, "'")+1, "'") != NULL || atoi(cmd+4) != 0) {
                if (!del(first, cmd+4)) 
                    printf("Command Success\n");
                else 
                    printf("Failed to process\n");
            } else {
                printf("Use '' to encase data or type in number to del\n");
            }

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

void add (Node **node, char *input) {
    Node *new_node, *cur;
    char *data;

    *(strstr(input, "\n")) = '\0'; 
    *(strstr(strstr(input, "'")+1, "'")) = '\n';
    data = strstr(input, "'")+1;
    *(data-1) = '\0';

    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (Node *)malloc(sizeof(Node));

    strcpy(new_node->data, input);
    strcat(new_node->data, data);
    new_node->next = NULL;

    cur->next = new_node;
}

int del (Node **node, char *input) {
    Node *cur, *tmp;
    char *data;
    int id;

    id = 0;
    if (strstr(input, "'") == NULL) { 
        id = atoi(input);
    } else {
        *(strstr(input, "\n")) = '\0'; 
        *(strstr(strstr(input, "'")+1, "'")) = '\n';
        data = strstr(input, "'")+1;
    }


    cur = *node;
    while (cur->next != NULL) {
        if (id == 0) {
            if (strcmp(strstr(cur->next->data, ",")+1, data) != 0) {
                cur = cur->next;
                continue;
            }
        } else {
            if (atoi(cur->next->data) != id) {
                cur = cur->next;
                continue;
            }
        }

        tmp = cur->next->next;
        free(cur->next); 
        cur->next = tmp;
        return 0;

    } 

    return 1;
}

void save (Node **node, char *fn) {
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
}

void load (Node **node, char *fn) {
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
            len-=4;
            i = 0;
            while (1) {
                fread((data+i), 1, 1, load);
                if (*(data+i) == '\n') {
                    *(data+i+1) = '\0';
                    break;
                }
                i++;
            }
            len-=i+1;

            sprintf(output,"%d,%s", id, data);
            add(node, output);
            free(output);
        }
        free(data);
        fclose(load);
    }
}

void showList (Node **node) {
    Node *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        printf("%s\n", cur->data);
        cur = cur->next;
    }
    printf("----------------------------end----------------------------\n");
}

void cleanList (Node **node) {
    Node *cur, *next;

    cur = (*node)->next;
    while (cur != NULL) {
        next = cur->next;
        
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

                data = (char *)malloc(nextpos-pos+1);
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
