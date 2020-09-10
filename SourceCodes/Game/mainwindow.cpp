#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <config.h>
#include <QPushButton>
#include <mypushbutton.h>
#include <QDebug>
#include <QTimer>
#include <QSound>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化窗口
    initScene();
    //播放背景音乐wav
    QSound * bgm = new QSound(SOUND_BACKGROUND);
    bgm->play();
    bgm->setLoops(-1);
    //菜单栏
    menu();

    //音效
    QSound * startSound = new QSound(SOUND_CLICK);
    //开始按钮
    MyPushButton * startBtn= new MyPushButton(START_BUTTON);
    startBtn->setParent(this);
    startBtn->move( this->width() *0.65 ,this->height() * 0.45);
    //开始按钮linker
    connect(startBtn,&MyPushButton::clicked, [=]()
    {
        //qDebug() <<"start";
        //音效
        startSound->play();

        //弹跳特效
        startBtn->zoom1();
        startBtn->zoom2();
        //延时进入->让特效充分展示
        QTimer::singleShot(500,this,[=]()
        {
            //进入到选关场景
            //自身隐藏
            this->hide();
            //显示选关场景
            chooseScene->show();
        });
        //监听返回按钮信号
        connect(chooseScene,&chooselevelscene::choosesceneback,this,[=](){
            chooseScene->hide();
            this->show();
        });
    });
    //音效
    QSound * helpSound = new QSound(SOUND_CLICK);
    //帮助按钮
    MyPushButton * helpBtn= new MyPushButton(HELP_BUTTON);
    helpBtn->setParent(this);
    helpBtn->move( this->width() *0.65 ,this->height() * 0.60);
    //帮助按钮linker
    connect(helpBtn,&MyPushButton::clicked, [=]()
    {
        //qDebug() <<"start";
        //音效
        helpSound->play();

        //弹跳特效
        helpBtn->zoom1();
        helpBtn->zoom2();
        //延时进入->让特效充分展示
        QTimer::singleShot(500,this,[=]()
        {
            //自身隐藏
            this->hide();
            //显示帮助场景
            helpWin->show();
        });
        //监听返回按钮信号
        connect(helpWin,&helpscene::helpsceneback,this,[=](){
            helpWin->hide();
            this->show();
        });
    });
    //退出游戏按钮
    MyPushButton * quitBtn= new MyPushButton(QUIT_BUTTON);
    quitBtn->setParent(this);
    quitBtn->move(this->width() *0.65,this->height() * 0.75);
    connect(quitBtn,&QPushButton::clicked,this,&MainWindow::close);

    //创建选关场景
    chooseScene = new chooselevelscene;
    //创建帮助场景
    helpWin = new helpscene;

}

/***************绘制开始背景图***************/
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(START_SCENE);
    painter.drawPixmap(0,0,pix);
}

/***************初始化窗口***************/
void MainWindow::initScene()
{
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);//设置窗口大小
    setWindowTitle(GAME_TITLE);//设置标题
    setWindowIcon(QPixmap(ANT_PIX));//设置图标
}

void MainWindow::menu()
{
    //菜单栏创建
    QMenuBar * bar = menuBar();
    //将菜单栏放入到窗口中
    setMenuBar(bar);
    //创建菜单
    QMenu * fileMenu = bar->addMenu("模式选择");
    //创建菜单项1
    fileMenu->addAction("键盘模式");
    //添加分割线
    fileMenu->addSeparator();
    // 创建菜单项2
    fileMenu->addAction("体感模式");
}

MainWindow::~MainWindow()
{
    delete ui;
}












