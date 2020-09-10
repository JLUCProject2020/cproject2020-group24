#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include <chooselevelscene.h>
#include<helpscene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //画背景图
    void paintEvent(QPaintEvent *);
    //初始化窗口
    void initScene();
    //菜单栏
    void menu();
    chooselevelscene * chooseScene = NULL;
    helpscene * helpWin = NULL;

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
