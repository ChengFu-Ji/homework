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
#include <signal.h>
#include <time.h>

#include "linkedlist.h"

using namespace cv;

void *recvData(void*);
void onMouse (int, int, int, int, void*);
void showTime (char *);

char windowName[] = "Display Image";
char imageName[] = "background.png";
Mat image;
Node_s **sendList;

int main() {
    pthread_t t;
    int fd;
    struct sockaddr_in me;
    struct hostent *host;

    sendList = (Node_s **)malloc(sizeof(Node_s *));
    *sendList = (Node_s *)malloc(sizeof(Node_s));
    (*sendList)->next = NULL;

    host = gethostbyname("localhost");
    fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&me, 0, sizeof(struct sockaddr_in));
    me.sin_family = AF_INET;
    me.sin_port = htons(2244);
    memcpy(&me.sin_addr, host->h_addr_list[0], host->h_length);

    

    if (connect(fd, (struct sockaddr *) &me, sizeof(struct sockaddr_in)) < 0) {
        printf("Error Connection!\n");
        exit(1);
    }

    image = imread(imageName, 1);
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, image);

    setMouseCallback(windowName, onMouse, (void *) &fd);

    pthread_create(&t, NULL, recvData, (void *) &fd);

    while (1) {
        if (waitKey(100) == 27) {
            close(fd);
            free(*sendList);
            free(sendList);
            printf("exiting...\n");
            break;
        }
        if (pthread_kill(t, 0) == ESRCH) {
            pthread_create(&t, NULL, recvData, (void *) &fd);
        }
    }
    if (pthread_kill(t, 0) != ESRCH) {
        pthread_cancel(t);
    }
    return 0;
}

void *recvData(void *fd) {
    int sockfd = *(int *) fd;
    Node_s space;
    Node_s *recv, *cur;
    Point p1, p2;

    recv = &space;
    recv->next = NULL;

    if (!socket_read(&recv, sockfd)) {
        showList(&recv);
        cur = recv->next;
        while (cur->next != NULL) {
            p1.x = cur->point.x;
            p1.y = cur->point.y;
            p2.x = cur->next->point.x;
            p2.y = cur->next->point.y;
            line(image, p1, p2, Scalar(55, 55, 0), 2); 
            imshow(windowName, image);
            cur = cur->next;
        }
        cleanList(&recv);
    }

    pthread_exit(0);
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

        line(image, pre_pos, now_pos, Scalar(0, 255, 0), 2);
        imshow(windowName, image);

        add(sendList, pre_pos.x, pre_pos.y);

        pre_pos.x = x;
        pre_pos.y = y;

    } else if (event == EVENT_LBUTTONUP) {
        add(sendList, pre_pos.x, pre_pos.y);

        showList(sendList);
        add(sendList, -1, 0);
        socket_write(sendList, fd);
        showTime("end send");
        
        cleanList(sendList);
    }
}

void showTime (char *status) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    printf("%s, time[%ld]\n", status, ts.tv_nsec);
}
