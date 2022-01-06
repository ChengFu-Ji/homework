#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void quit();
    void showMat();
    void loginBtn();
    void login();
    void regBtn();
    void regAccount();
    void warnWin(QString, int);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
