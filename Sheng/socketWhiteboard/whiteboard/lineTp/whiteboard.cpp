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

#include "linkedlist.h"

using namespace cv;

void *recvData(void*);
void onMouse (int, int, int, int, void*);

char windowName[] = "Display Image";
char imageName[] = "background.png";
Mat image;
Node_s **sendList;
Node_s **recvList;

int main() {
    pthread_t t;
    int fd;
    struct sockaddr_in me;
    struct hostent *host;

    sendList = (Node_s **)malloc(sizeof(Node_s *));
    *sendList = (Node_s *)malloc(sizeof(Node_s));
    (*sendList)->next = NULL;

    recvList = (Node_s **)malloc(sizeof(Node_s *));
    *recvList = (Node_s *)malloc(sizeof(Node_s));
    (*recvList)->next = NULL;

    host = gethostbyname("10.25.1.133");
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
            free(*recvList);
            free(recvList);
            printf("Exiting...\n");
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
    int id;
    Node_s space;
    Node_s *recv, *cur, *next;
    Point p1, p2;

    if ((id = socket_read(recvList, sockfd)) != -1) {
        cur = (*recvList)->next;
        while (cur->next != NULL) {
            printf("id %d\n", id);
            if (cur->point.id == id && cur->next->point.x != -1 && cur->next->point.y != 0) {
                p1.x = cur->point.x;
                p1.y = cur->point.y;
                next = cur->next;
                while (next->point.id != id && next != NULL) {
                    next = next->next;
                }
                if (next->point.x != -1 && next->point.y != 0) {
                    p2.x = next->point.x;
                    p2.y = next->point.y;
                    line(image, p1, p2, Scalar(55, 55, 0), 2); 
                    imshow(windowName, image);
                }
            }
            cur = cur->next;
        }
        IDdelete(recvList, id);
    }

    pthread_exit(0);
}

void onMouse (int event, int x, int y, int flags, void *userdata) {
    static Data_s tmp;
    static Point pre_pos;
    int fd = *(int *) userdata;


    tmp.id = 0;
    if (event == EVENT_LBUTTONDOWN) {
        pre_pos.x = x;
        pre_pos.y = y;
        printf("L click pos x: %d, y: %d\n", pre_pos.x, pre_pos.y);
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) {
        Point now_pos;

        now_pos.x = x;
        now_pos.y = y;

        line(image, pre_pos, now_pos, Scalar(0, 255, 0), 2);
        imshow(windowName, image);

        tmp.x = pre_pos.x;
        tmp.y = pre_pos.y;
        add(sendList, tmp);

        pre_pos.x = x;
        pre_pos.y = y;

        printf("moving pos x: %d, y: %d\n", x, y);
    } else if (event == EVENT_LBUTTONUP) {

        add(sendList, tmp);

        showList(sendList);
        tmp.x = -1;
        tmp.y = 0;
        add(sendList, tmp);
        socket_write(sendList, fd);
        
        cleanList(sendList);
        
    }
}

