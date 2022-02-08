#include "linkedlist/files.h"

int initFiles (fileNode ***filesPtr) {
    if ((*filesPtr = (fileNode **) malloc(sizeof(fileNode *))) == NULL) {
        return -1;
    }

    if ((**filesPtr = (fileNode *) malloc(sizeof(fileNode))) == NULL) {
        return -1;
    }
    (**filesPtr)->next = NULL;

    return 0;
}

int freeFiles (fileNode **files) {
    free(*files);
    free(files);
    return 0;
}

int addFile (fileNode **files, char *filename, char fileType) {
    fileNode *cur, *newNode;

    cur = (*files);
    while (cur->next != NULL) {
        cur = cur->next;
    }

    newNode = (fileNode *)malloc(sizeof(fileNode));
    newNode->fileType = fileType;
    newNode->fileName = (char *)malloc(strlen(filename)+1);
    strcpy(newNode->fileName, filename);
    newNode->next = NULL;
    
    cur->next = newNode;
    return 0;
}

int delFile (fileNode **files, char *filename) {
    fileNode *cur, *next;

    cur = (*files);
    while (cur->next != NULL) {
        if (!strcmp(cur->next->fileName, filename)) {
            next = cur->next->next;
            free(cur->next->fileName);
            free(cur->next);
            cur->next = next;
            break;
        }
        cur = cur->next;
    }
    return -1;
}

int deleteAllFiles(fileNode **files) {
    fileNode *cur, *next;

    cur = (*files);
    while (cur->next != NULL) {
        next = cur->next->next;
        free(cur->next->fileName);
        free(cur->next);

        cur->next = next;
    }
    return 0;
}

int showFilesList(fileNode **files) {
    fileNode *cur;

    cur = (*files);
    while (cur->next != NULL) {
        printf("filename %s type %d\n", cur->next->fileName, cur->next->fileType);
        cur = cur->next;
    }
    return 0;
}

int getFilesLen(fileNode **files) {
    fileNode *cur;
    int i = 0;
    cur = (*files);
    while (cur->next != NULL) {
        cur = cur->next;
        i++;
    }
    return i;
}
/*
int main () {
    fileNode **test;
    initFiles(&test);

    addFile(test, "1234", 3);
    addFile(test, "1233", 3);
    addFile(test, "1235", 3);
    addFile(test, "1232", 3);
    addFile(test, "1231", 3);
    showFilesList(test);
    deleteAllFiles(test);
    showFilesList(test);
    
    freeFiles(test);
    return 0;
}
*/
