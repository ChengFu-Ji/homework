#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"


typedef struct cmd_p {
    char cmd[10];
    int cmdlen;
    int (*cmdfp) (Node **, char *);
} Cmd;

int main() {
    Node **head;
    char *cmd, cmdspac[105]; 

    Cmd cmds[] = {
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
    head = (Node **)malloc(sizeof(Node *));
    *head = (Node *)malloc(sizeof(Node));
    (*head)->data = NULL;
    (*head)->next = NULL;

    printf("Welcome!\n");
    printf("\ncommands : <add>, <del>, <save>, <load>, <showlist>, <cleanlist>, <show,,>\n");
    printf("ex : [add,(id,)data], [del,(id/data)], [save,filename], [showlist], [show,filename,number]\n");

    while (1) {
        printf(">> ");
        fgets(cmd, 105, stdin);
        if (!strcmp(cmd, "exit\n")) {
            break;
        }

        int i = 0;
        while (cmds[i].cmdlen) {
            if (!strncmp(cmds[i].cmd, cmd, cmds[i].cmdlen)) {
                if (cmds[i].cmdfp(head, (cmd+cmds[i].cmdlen)) == 0) {
                    printf("Command done!\n");
                } else {
                    printf("Command failed...\n");
                }
                break;
            }
            i++;
        }
    }

    free(*head);
    free(head);
    return 0;
}
