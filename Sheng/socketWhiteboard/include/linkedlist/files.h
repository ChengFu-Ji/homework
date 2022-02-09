#ifndef __FILES_LINKEDLIST_H__
#define __FILES_LINKEDLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _fileNode {
    char fileType;
    char *fileName;
    struct _fileNode *next;
} fileNode;

int initFiles(fileNode ***filesPtr);
int freeFiles(fileNode **files);

int addFile (fileNode **files, char *filename, char fileType);
int delFile (fileNode **files, char *filename);
int deleteAllFiles (fileNode **files);

/* debug use */
int showFilesList (fileNode **files);

int getFilesLen (fileNode **files);
fileNode *getFilesbyOrder (fileNode **files, int num);
#endif
