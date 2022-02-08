#ifndef __WB_WINDOWS_H__
#define __WB_WINDOWS_H__

#include <stdio.h>
#include <stdlib.h>
/*memset*/
#include <string.h>
/* draw Icons */
#include <math.h>

/* find files */
#include <dirent.h>

/* man tcp */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* gethostbyname */
#include <netdb.h>

/* write read close functions */
#include <unistd.h>

#include <pthread.h>
#include <signal.h>

/* opencv */
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

/* my include */
extern "C" {
#include "bezier.h"
#include "linkedlist/pos.h"
#include "linkedlist/stroke.h"
#include "linkedlist/page.h"
#include "linkedlist/files.h"
}

#define TIMES 1000
#define DOTS 6

struct _sender {
    pageNode **pageHead;
    posNode **posHead;
    int *thk;
    int *is_eraser;
    int *id;
};

typedef struct _explorer_sender {
    fileNode **fileHead;
    cv::Mat *drawMat;
    cv::Rect *ListView;
    cv::Rect *scrollbar;
    cv::Rect *scrollbar_thumb;
    int listlen;
    int *head;
    int stepDist;
} Exp_s;

/*
typedef struct _cmd {
    int num;
    
} Cmd;
*/

int connectToServer (char *, int);
void *recvData (void *);
void plotHandwriting (cv::Mat, DPos *, DPos *, int, int, int);

void changeToPage (pageNode *, int *);
void drawPosList (cv::Mat image, posNode **pos, int thk, int is_eraser);
void drawStrokeList (cv::Mat image, strokeNode **strokes);

/* opencv */
void mouseOnWhiteboard (int, int, int, int, void *);

/*input box */
void getValue (int *value, int maxlength);
void getString (char *string, int maxlength);

/* button bar */
void setbuttonBar (struct _sender);
void buttonBarEvent (int, int, int, int, void *);

/* action bar */
void setActionBar ();


void storeImage (pageNode *page, char *fileName);

/* draw Icons */
void drawEraserIcon (cv::Mat image, int x, int y, double Size);
void drawSelectPageIcon (cv::Mat image, int x, int y, double Size);
void drawAddPageIcon (cv::Mat image, int x, int y, double Size);
void drawDeletePageIcon (cv::Mat image, int x, int y, double Size);
void drawSelectPageIcon (cv::Mat image, int x, int y, double Size);
void drawNextPageIcon (cv::Mat image, int x, int y, double Size);
void drawPrevPageIcon (cv::Mat image, int x, int y, double Size);

/* find files */
void setfileExplorer ();
void showPathFiles (cv::Mat image, fileNode **files, int head, cv::Rect *list, int listLen);
void ExplorerEvent (int event, int x, int y, int flags, void *sender);
void findPathFiles (fileNode **files, char *path);

/*
 color selector 
#ifdef __COLORFUL_WHITEBOARD__
void setColorSelector (struct _sender);
#endif
*/

#endif
