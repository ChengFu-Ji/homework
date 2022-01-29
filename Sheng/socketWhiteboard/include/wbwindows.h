#ifndef __WB_WINDOWS_H__
#define __WB_WINDOWS_H__

#include <stdio.h>
#include <stdlib.h>
/*memset*/
#include <string.h>
//#include <math.h>

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

int connectToServer (char *, int);
void *recvData (void *);
void plotHandwriting (DPos *, DPos *, int, int, int);

/* opencv */
void mouseOnWhiteboard (int, int, int, int, void *);

void changeToPage (pageNode *, int *);
void drawPosList (posNode **, int, int);
void drawStrokeList (strokeNode **);

/*input box */
void getValue (int *, int);

/* button bar */
void setbuttonBar (struct _sender);
void buttonBarEvent (int, int, int, int, void *);

/* color selector */
#ifdef __COLORFUL_WHITEBOARD__
void setColorSelector (struct _sender);
#endif

#endif
