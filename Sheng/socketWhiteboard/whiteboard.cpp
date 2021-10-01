#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "linkedlist.h"

using namespace cv;

void *recvData(void*);
void onMouse (int, int, int, int, void*);

char windowName[] = "Display Image";
char imageName[] = "background.png";
Mat image;
Node_s **sendList;

int main() {
    pthread_t t;
    int len, fd;
    struct sockaddr_in me;
    struct hostent *host;

    sendList = (Node_s **)malloc(sizeof(Node_s *));
    *sendList = (Node_s *)malloc(sizeof(Node_s));
    (*sendList)->next = NULL;

    host = gethostbyname("192.168.0.236");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&me, 0, sizeof(struct sockaddr_in));
    me.sin_family = AF_INET;
    me.sin_port = htons(2244);
    memcpy(&me.sin_addr, host->h_addr_list[0], host->h_length);
    len = sizeof(struct sockaddr_in);
    if (connect(fd, (struct sockaddr *) &me, len) < 0) {
        printf("Error Connection!\n");
        exit(1);
    }

    image = imread(imageName, 1);
    namedWindow(windowName, WINDOW_NORMAL);
    /*  WINDOW_NORMAL: 0x00000000 can resize
        WINDOW_AUTOSIZE: 0x00000001 can't resize
    */

    imshow(windowName, image);
    setMouseCallback(windowName, onMouse, (void *) &fd);

    while (1) {
        if (waitKey(1000) == 27) break;
        pthread_create(&t, NULL, recvData, (void *) &fd);
    }

    Data_s quit = {-1, -1};
    write(fd, &quit, sizeof(int));
    close(fd);
    free(*sendList);
    free(sendList);
    printf("Exiting...\n");

    return 0;
}

void *recvData(void *fd) {
    int sockfd = *(int *) fd;
    Node_s **recv, *cur;
    Point p1, p2;

    recv = (Node_s **)malloc(sizeof(Node_s *));
    *recv = (Node_s *)malloc(sizeof(Node_s));
    (*recv)->next = NULL;

    if (!socket_read(recv, sockfd)) {
        showList(recv);
        cur = (*recv)->next;
        while (cur->next != NULL) {
            p1.x = cur->point.x;
            p1.y = cur->point.y;
            p2.x = cur->next->point.x;
            p2.y = cur->next->point.y;
            line(image, p1, p2, Scalar(0, 0, 0), 2); imshow(windowName, image);
            cur = cur->next;
        }
        cleanList(recv);
    } else {
        printf("get Nothing...\n");
    }

    free(*recv);
    free(recv);
    pthread_exit(NULL);
}

void onMouse (int event, int x, int y, int flags, void *userdata) {
    Data_s tmp;
    static Point pre_pos;
    int fd = *(int *) userdata;


    if (event == EVENT_LBUTTONDOWN) {
        pre_pos.x = x;
        pre_pos.y = y;
        printf("L click pos x: %d, y: %d\n", pre_pos.x, pre_pos.y);
    } else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON) {
        Point now_pos;

        now_pos.x = x;
        now_pos.y = y;

        line(image, pre_pos, now_pos, Scalar(0, 0, 0), 2);
        imshow(windowName, image);

        add(sendList, pre_pos.x, pre_pos.y);

        pre_pos.x = x;
        pre_pos.y = y;

        printf("moving pos x: %d, y: %d\n", x, y);
    } else if (event == EVENT_LBUTTONUP) {
        add(sendList, pre_pos.x, pre_pos.y);

        showList(sendList);
        add(sendList, -1, 0);
        socket_write(sendList, fd);
        
        cleanList(sendList);
    }
}

