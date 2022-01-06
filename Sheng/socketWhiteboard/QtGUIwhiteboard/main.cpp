#include "mainwindow.h"

#include <QApplication>
#include <QLabel>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    /*
    Mat tmp = Mat(100, 100, CV_8UC3, Scalar(255, 255, 255));
    namedWindow("tt", WINDOW_FULLSCREEN);
    imshow("tt", tmp);
    QLabel *label = new QLabel("wowo");
    label->setWindowTitle("my Test");
    label->resize(200, 50);
    label->show();
    */
    w.show();


    /*
    if (waitKey(0) == 27) {
        destroyWindow("tt");
    }
    */

    return a.exec();
}
