#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node_s {
    char data[100];
    struct node_s *next;
} Node;
typedef struct cmd {
    char cmd[10];
    int cmdlen;
    int (*cmdfp) (Node **, char *);
} fcmd;

int add(Node **, char *);
int del(Node **, char *);
int save(Node **, char *);
int load(Node **, char *);
int showList(Node **, char *);
int cleanList(Node **, char *);
int showN(Node **, char *);

int main() {
    Node **first;
    char *cmd, cmdspc[105];
    fcmd cmds[] = {
        {"add,", 4, add},
        {"del,", 4, del},
        {"save,", 5, save},
        {"load,", 5, load},
        {"showlist", 8, showList},
        {"clean", 5, cleanList},
        {"show,", 5, showN},
        {"", 0, NULL}
    };

    cmd = cmdspc;
    first = (Node **)malloc(sizeof(Node *));
    *first = (Node *)malloc(sizeof(Node));
    (*first)->next = NULL;

    printf("Welcome!\n");
    printf("\ncommands : <add>, <del>, <save>, <load>, <showlist>, <cleanlist>, <show,,>\n");
    printf("ex : [add,(id,)data], [save,filename], [showlist], [show,filename,number]\n");


    while (1) {
        printf(">> ");
        fgets(cmd, 105, stdin);

        int i = 0;
        while (cmds[i].cmdlen) {
            if (!strncmp(cmds[i].cmd, cmd, cmds[i].cmdlen)) {
                if (cmds[i].cmdfp(first, (cmd+cmds[i].cmdlen)) == 0) {
                    printf("command done!\n");
                } else {
                    printf("command fail...\n");
                }
            } 
            i++;
        }
        if (!strcmp(cmd, "exit\n")) {
            printf("exiting...\n");
            break;
        }
    }

    cleanList(first, "");
    free(*first);
    free(first);
    return 0;
}

int add (Node **node, char *input) {
    Node *new_node, *cur;
    static int id = 0;

    id++;
    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->data, "");
    if (atoi(input) == 0 || strstr(input, ",") == NULL) {
        sprintf(new_node->data, "%d,", id);
    }
    strcat(new_node->data, input);
    new_node->next = NULL;

    cur->next = new_node;
    return 0;
}

int del (Node **node, char *input) {
    Node *cur, *tmp;
    char *data;
    int id;

    data = (char *)malloc(strlen(input));
    cur = *node;
    while (cur->next != NULL) {
        strcpy(data, strstr(cur->next->data, ",")+1);
        id = atoi(cur->next->data);
        if (!strcmp(cur->next->data, input) || id == atoi(input) || !strcmp(input, data)) {
            tmp = cur->next->next;
            free(cur->next); 
            cur->next = tmp;
            return 0;
        }
        cur = cur->next;
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

int showList (Node **node, char *none) {
    Node *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        printf("%s\n", cur->data);
        cur = cur->next;
    }
    printf("----------------------------end----------------------------\n");
    return 0;
}

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
