#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QDialog>
#include <QLayout>
#include <QLineEdit>

#include "linkedlist.h"
#include "whiteboard.h"

using namespace cv;

QDialog *loginRegDia, *wrong;
QLineEdit *pinput, *input, *confirm;
QString account, password;
Mat image;
Node_s **recvList, **sendList;
int is_login = 0, is_register = 0, is_exec = 0;
char windowName[] = "whiteboard";

void onTest(int status, void *userdata);
void regAccount();
void btnShow(QPushButton *btn);
void btnHide(QPushButton *btn);
int clientInit (int, char *);
Node_s **linkedlistInit ();
void linkedlistFree (Node_s **);
void *recvData (void *);
void MouseWork (int, int, int, int, void *);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->loginPBtn, &QAbstractButton::clicked, this, &MainWindow::loginBtn);
    //connect(ui->loginPBtn, &QAbstractButton::clicked, this, &MainWindow::quit);
    connect(ui->regPBtn, &QAbstractButton::clicked, this, &MainWindow::regBtn);
    //connect(ui->regPBtn_3, &QAbstractButton::clicked, this, &MainWindow::showMat);
    connect(ui->exitPBtn, &QAbstractButton::clicked, this, [&] () {this->close();});
    connect(ui->startPBtn, &QAbstractButton::clicked, this, &MainWindow::showMat);
    connect(ui->logoutPBtn, &QAbstractButton::clicked, this, &MainWindow::quit);

    btnHide(ui->logoutPBtn);
    btnHide(ui->startPBtn);
    ui->namelabel->hide();

    loginRegDia = NULL;
    password = NULL;
    account = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginBtn() {
    if (!is_login) {
        if (loginRegDia != NULL) {
            if (loginRegDia->isVisible()) {
                loginRegDia->done(0);
            }
        }
        loginRegDia = new QDialog();
        QVBoxLayout *vlayout = new QVBoxLayout;
        QHBoxLayout *layout = new QHBoxLayout;
        input = new QLineEdit();

        layout->addWidget(new QLabel(" account:   "));
        layout->addSpacing(15);
        layout->addWidget(input);

        vlayout->addLayout(layout);
        vlayout->addSpacing(10);

        layout = new QHBoxLayout;
        pinput = new QLineEdit();
        pinput->setEchoMode(pinput->Password);

        layout->addWidget(new QLabel("password: "));
        layout->addSpacing(15);
        layout->addWidget(pinput);

        vlayout->addLayout(layout);

        layout = new QHBoxLayout;

        QPushButton *btn = new QPushButton();
        btn->setText("submit");
        connect(btn, &QAbstractButton::clicked, this, &MainWindow::login);
        layout->addWidget(btn);

        layout->addSpacing(20);

        btn = new QPushButton();
        btn->setText("register");
        connect(btn, &QAbstractButton::clicked, this, &MainWindow::regBtn);
        layout->addWidget(btn);

        layout->setDirection(layout->RightToLeft);
        vlayout->addLayout(layout);

        loginRegDia->setLayout(vlayout);
        loginRegDia->setWindowModality(Qt::ApplicationModal);
        loginRegDia->show();
    } else {
        warnWin(QString("You are already Logged in !!"), 0);
    }
}

void MainWindow::login() {
    QString acc = input->text();
    QString pwd = pinput->text();

    if (!account.isNull() && !password.isNull()) {
        if (acc.compare(account) == 0 && pwd.compare(password) == 0) {
            is_login = 1;
            loginRegDia->done(0);

            warnWin(QString("success !!"), 0);

            btnHide(ui->loginPBtn);
            btnHide(ui->regPBtn);
            btnHide(ui->exitPBtn);
            btnShow(ui->startPBtn);
            btnShow(ui->logoutPBtn);
            ui->namelabel->show();
            ui->namelabel->setText(QString("Welcome~ "+account+" !~"));
        } else {
            warnWin(QString("Wrong password or Account !!"), 0);
        }
    } else {

        warnWin(QString("Not Register yet !!"), 0);
    }
}

void MainWindow::regBtn() {
    if (!is_login && !is_register) {
        if (loginRegDia != NULL) {
            if (loginRegDia->isVisible()) {
                loginRegDia->done(0);
            }
        }
        loginRegDia = new QDialog();
        QVBoxLayout *vlayout = new QVBoxLayout;
        QHBoxLayout *layout = new QHBoxLayout;
        QLabel *text = new QLabel(" account:   ");
        input = new QLineEdit();

        layout->addWidget(text);
        layout->addSpacing(15);
        layout->addWidget(input);

        vlayout->addLayout(layout);

        vlayout->addSpacing(10);

        layout = new QHBoxLayout;
        text = new QLabel("password: ");
        pinput = new QLineEdit();
        pinput->setEchoMode(pinput->Password);

        layout->addWidget(text);
        layout->addSpacing(15);
        layout->addWidget(pinput);

        vlayout->addLayout(layout);
        vlayout->addSpacing(10);

        layout = new QHBoxLayout;
        text = new QLabel("  confirm\npassword: ");
        confirm = new QLineEdit();
        confirm->setEchoMode(confirm->Password);

        layout->addWidget(text);
        layout->addSpacing(15);
        layout->addWidget(confirm);

        vlayout->addLayout(layout);

        layout = new QHBoxLayout;

        QPushButton *btn = new QPushButton();
        btn->setText("submit");
        connect(btn, &QAbstractButton::clicked, this, &MainWindow::regAccount);
        layout->addWidget(btn, Qt::AlignRight);

        //layout->addSpacing(20);

        vlayout->addLayout(layout);
        //layout->setGeometry(QRect(1000, 1000, 5000, 5000));
        //layout->setAlignment(Qt::AlignLeft);

        loginRegDia->setLayout(vlayout);
        loginRegDia->setWindowModality(Qt::ApplicationModal);
        loginRegDia->show();

        //loginRegDia->setFocus();
        //this->setFocus();
    } else {
        warnWin(QString("You are already registered!!"), 0);
    }
}

void MainWindow::regAccount() {
    QString acc = input->text();
    QString pwd = pinput->text();
    QString con = confirm->text();

    if (!acc.isEmpty() && !pwd.isEmpty() && !con.isEmpty()) {
        if (pwd == con) {
            password = pwd;
            account = acc;
            is_register = 1;
            loginRegDia->done(0);

            warnWin(QString("register success !!"), 1);
        } else {
            warnWin(QString("password not the same !!"), 0);
        }
    }
}


void btnShow(QPushButton *btn) {
    btn->setEnabled(true);
    btn->show();
}

void btnHide(QPushButton *btn) {
    btn->setEnabled(false);
    btn->hide();
}

void MainWindow::warnWin(QString showText, int execloginBtn) {
    is_exec = execloginBtn;

    wrong = new QDialog();
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addSpacing(20);
    vlayout->addWidget(new QLabel(showText));
    vlayout->addSpacing(20);
    QPushButton *btn = new QPushButton();
    btn->setText("confirm");
    connect(btn, &QAbstractButton::clicked, this, [&] () {
        wrong->done(0);
        if (is_exec) {
            MainWindow::loginBtn();
        }
    });
    vlayout->addWidget(btn);

    wrong->setLayout(vlayout);
    wrong->setWindowModality(Qt::ApplicationModal);
    wrong->show();
}

void MainWindow::quit() {
    is_login = 0;
    btnHide(ui->logoutPBtn);
    btnHide(ui->startPBtn);
    btnShow(ui->loginPBtn);
    btnShow(ui->regPBtn);
    btnShow(ui->exitPBtn);
    ui->namelabel->hide();
}

void MainWindow::showMat() {
    this->hide();
    pthread_t t;
    int fd;

    image = Mat(900, 1600, CV_8UC3, Scalar(255, 255, 255));

    int port = 2244;
    char address[] = "localhost";

    sendList = linkedlistInit();
    recvList = linkedlistInit();

    namedWindow(windowName, WINDOW_NORMAL | WINDOW_FREERATIO | WINDOW_GUI_NORMAL);
    imshow(windowName, image);

    setMouseCallback(windowName, MouseWork, (void *) &fd);

    if ((fd = clientInit(port, address)) < 0) {
        printf("ERROR:Can't not connect to %s:%d....\nTurn on offline mode\n", address, port);
    } else {
        pthread_create(&t, NULL, recvData, (void *) &fd);
    }

    while (1) {
        //printf(" %lf\n", getWindowProperty(windowName, WND_PROP_VISIBLE));
        if (waitKey(100) == 27 || getWindowProperty(windowName, WND_PROP_VISIBLE) <= 0) {
            ::close(fd);
            linkedlistFree(recvList);
            linkedlistFree(sendList);
            printf("Exiting...\n");
            break;
        }
        if (fd > 0) {
            if (pthread_kill(t, 0) == ESRCH) {
                pthread_create(&t, NULL, recvData, (void *) &fd);
                imshow(windowName, image);
            }
        }
    }
    if (fd > 0) {
        if (pthread_kill(t, 0) != ESRCH) {
            pthread_cancel(t);
        }
    }

    destroyWindow(windowName);
    this->show();
}

Node_s **linkedlistInit () {
    Node_s **list;
    list = (Node_s **)malloc(sizeof(Node_s *));
    *list = (Node_s *)malloc(sizeof(Node_s));
    (*list)->next = NULL;
    return list;
}

void linkedlistFree (Node_s **list) {
    free(*list);
    free(list);
}

int clientInit (int port, char *address) {
    int fd;
    struct sockaddr_in mAddr;
    struct hostent *host;

    if ((host = gethostbyname(address)) == NULL) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&mAddr, 0, sizeof(mAddr));
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(port);
    memcpy(&mAddr.sin_addr, host->h_addr_list[0], host->h_length);

    if (connect(fd, (struct sockaddr *) &mAddr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }

    return fd;
}

void *recvData(void *fd) {
    Point2d p[DOTS], plot[TIMES];
    Node_s *cur;
    int sockfd = *(int *) fd, i = 0;
    int size;

    if (read(sockfd, &size, sizeof(int)) < 0) {
        pthread_exit(NULL);
    }

    if (socket_read(recvList, sockfd, size)) {
        for (i = 0; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
        cur = (*recvList)->next;
        while (cur != NULL) {
            //printf("%d:cur %d %d\n", cur->point.id, cur->point.x, cur->point.y);
            if (cur->point.x != -1 && cur->point.y != 0) {
                for(i = 0; i < DOTS-1; i++) {
                    if (p[i].x == -1 && p[i].y == -1) {
                        p[i] = Point(cur->point.x, cur->point.y);
                        cur = cur->next;
                        break;
                    }
                }
                if (i != DOTS-1) {
                    continue;
                }
                p[DOTS-1] = Point(cur->point.x, cur->point.y);
                bezierCurve(plot, TIMES, p, DOTS);
                plotHandwriting(p, plot, 0);
            }
            cur = cur->next;
        }

        plotHandwriting(p, plot, 1);
        //IDdelete(recvList, id);
        cleanList(recvList);
    }
    pthread_exit(0);
}

void MouseWork (int event, int x, int y, int flags, void *userdata) {
    static Data_s tmp;
    static Point2d p[DOTS];
    static int dots = 0;
    Point2d plot[TIMES] ;
    int fd = *(int *) userdata, i = 0;

    //printf("p[0] %lf ,%lf \n", p[0].x, p[0].y);
    if (event == EVENT_LBUTTONDOWN) {
        p[0] = Point(x, y);

        if (fd > 0) {
            dots++;
            tmp.x = x;
            tmp.y = y;
            add(sendList, tmp);
        }
        for (i = 1; i < DOTS; i++) {
            p[i] = Point(-1, -1);
        }
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) && p[0].x != -1 && p[0].y != -1) {
        for (i = 0; i < DOTS-1; i++) {
            if (p[i].x == -1 && p[i].y == -1) {
                p[i] = Point(x, y);
                if (fd > 0) {
                    dots++;
                    tmp.x = x;
                    tmp.y = y;
                    add(sendList, tmp);
                }
                return;
            }
        }

        p[DOTS-1] = Point(x, y);
        if (fd > 0) {
            dots++;
            tmp.x = x;
            tmp.y = y;
            add(sendList, tmp);
        }

        bezierCurve(plot, TIMES, p, DOTS);
        //printf("draw\n");
        plotHandwriting(p, plot, 0);
        imshow(windowName, image);
    } else if (event == EVENT_LBUTTONUP) {
        //printf("wow\n");
        if (fd > 0) {
            dots++;
            tmp.x = -1;
            tmp.y = 0;
            add(sendList, tmp);
            socket_write(sendList, fd, dots);
            cleanList(sendList);
            dots = 0;
        }
        plotHandwriting(p, plot, 1);
        imshow(windowName, image);
    }
}
