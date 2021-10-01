#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedlist.h"

/*
 *  新增一筆資料在 Linkedlist 中，
 *  如果使用者未輸入 id 則會自動補上。
 */
int add (Node_s **node, Data_s pt) {
    Node_s *new_node, *cur;

    cur = *node;
    while (cur->next != NULL)
        cur = cur->next;

    new_node = (Node_s *)malloc(sizeof(Node_s));
    new_node->point.x = pt.x;
    new_node->point.y = pt.y;
    new_node->next = NULL;

    cur->next = new_node;
    return 0;
}

/*
 *  刪除一筆和輸入資料相符的 Linkedlist 資料。
 *  (只需 id 或 data 其中一項與使用者輸入相符，便進行刪除)。
 
int del (Node **node, int x, int y) {
    Node *cur, *tmp;

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
*/

/*
 *  將目前的 Linkedlist 存入使用者輸入的檔案中。
 *  (會額外生成相同名稱的 .idx 索引檔)。
int save (Node **node, int none, int none2) {
    FILE *save;
    char *data;
    char fn[10] = ".draw_log";
    Node *cur;

    if ((save = fopen(fn, "a")) != NULL) {
        fwrite('\n', 1, 1, save);
        cur = (*node)->next;

        while (cur != NULL) {
            fwrite(&(cur->x), sizeof(int), 1, save);
            fwrite(&(cur->y), sizeof(int), 1, save);

            cur = cur->next;
        }
        fclose(save);
    }
    return 0;
}

*/
/*
 *  讀取使用者選定的檔案資料進 Linkedlist 。
 *  (不使用 .idx 檔)。
int load (Node **node, int none, int none2) {
    FILE *load;
    char *data, *output;
    char fn[10] = ".draw_log";
    int i, len, id;

    if ((load = fopen(fn, "r")) != NULL) {
        fseek(load, 0, SEEK_END);
        len = ftell(load);
        fseek(load, 0, SEEK_SET);
        
        while (len) {
            fread(&id, sizeof(int), 1, load);
        }
        free(data);
        fclose(load);
    }
    return 0;
}

*/

/*
 *  顯示目前 Linkedlist 所有的資料。
 */
int showList (Node_s **node) {
    Node_s *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        printf("x: %d, y: %d\n", cur->point.x, cur->point.y);
        cur = cur->next;
    }
    printf("----------------------------end----------------------------\n");
    return 0;
}

/*
 *  刪除目前 Linkedlist 所有的資料。
 */
int cleanList (Node_s **node) {
    Node_s *cur, *next;

    cur = (*node)->next;
    while (cur != NULL) {
        next = cur->next;
        
        free(cur);
        cur = next;
    }
    (*node)->next = NULL;
    return 0;
}

int socket_write (Node_s ** node, int fd) {
    Node_s *cur;

    cur = (*node)->next;
    while (cur != NULL) {
        write(fd, &(cur->point), sizeof(cur->point));

        cur = cur->next;
    }
    return 0;
}

int socket_read (Node_s ** node, int fd) {
    Data_s pt;

    while (1) {
        if (read(fd, &(pt), sizeof(pt)) > 0) {
            add(node, pt);
        } else {
            return 1;
        }
    }
    return 0;
}
/*
 *  讀取使用者選定檔案中，指定的該行資料並顯示。
 *  (使用 .idx 檔)。
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
 */
