#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

typedef struct cmd {
    char cmd[10];
    int cmdlen;
    int (*cmdfp) (Node **, char *);
} fcmd;

/*
 *  主程式
 */
int main() {
    Node **first;
    char *cmd, cmdspac[106];
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

    cmd = cmdspac;
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
                if (!cmds[i].cmdfp(first, (cmd+cmds[i].cmdlen))) {
                    printf("command done!\n");
                } else {
                    printf("command failed...\n");
                }
            }
            i++;
        }
        if (!strcmp(cmd, "exit\n")){
            printf("exiting...\n");
            break;
        }
    }
    free(*first);
    free(first);

    return 0;
}
