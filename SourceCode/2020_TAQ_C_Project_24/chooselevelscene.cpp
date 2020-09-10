#include "chooselevelscene.h"
#include "gamewindow.h"
#include <QPainter>
#include <config.h>
#include <QPushButton>
#include <mypushbutton.h>
#include <QSound>
#include <QTimer>

chooselevelscene::chooselevelscene(QWidget *parent)
    :QMainWindow(parent)
{
    //配置关卡场景
    this->setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    //设置图标
    this->setWindowIcon(QPixmap(ANT_PIX));
    //设置标题
    this->setWindowTitle(GAME_TITLE);

    //音效
    QSound * chooseSound = new QSound(SOUND_CLICK);
    //按钮1
    MyPushButton * chooseBtn1= new MyPushButton(CHOOSE_BUTTON1);
    chooseBtn1->setParent(this);
    chooseBtn1->move( this->width() *0.5-chooseBtn1->width() *0.5 ,this->height() * 0.2);
    connect(chooseBtn1,&MyPushButton::clicked, [=]()
    {
        gameWin = new gamewindow(1);
        //音效
        chooseSound->play();
        //弹跳特效
        chooseBtn1->zoom1();
        chooseBtn1->zoom2();
        //延时进入->让特效充分展示
        QTimer::singleShot(500,this,[=]()
        {
            //进入到选关场景
            //自身隐藏
            this->hide();
            //显示选关场景
            gameWin->show();
        });
        //监听返回按钮信号
        connect(gameWin,&gamewindow::gamewindowback,this,[=](){
            gameWin->hide();
            this->show();
        });
    });
    //按钮2
    MyPushButton * chooseBtn2= new MyPushButton(CHOOSE_BUTTON2);
    chooseBtn2->setParent(this);
    chooseBtn2->move( this->width() *0.5-chooseBtn2->width() *0.5 ,this->height() * 0.4);
    connect(chooseBtn2,&MyPushButton::clicked, [=]()
    {
        gameWin = new gamewindow(2);
        //音效
        chooseSound->play();
        //弹跳特效
        chooseBtn2->zoom1();
        chooseBtn2->zoom2();
        //延时进入->让特效充分展示
        QTimer::singleShot(500,this,[=]()
        {
            //进入到选关场景
            //自身隐藏
            this->hide();
            //显示选关场景
            gameWin->show();
        });
        //监听返回按钮信号
        connect(gameWin,&gamewindow::gamewindowback,this,[=](){
            gameWin->hide();
            this->show();
        });
    });
    //按钮3
    MyPushButton * chooseBtn3= new MyPushButton(CHOOSE_BUTTON3);
    chooseBtn3->setParent(this);
    chooseBtn3->move( this->width() *0.5-chooseBtn3->width() *0.5 ,this->height() * 0.6);
    connect(chooseBtn3,&MyPushButton::clicked, [=]()
    {
        gameWin = new gamewindow(3);
        //音效
        chooseSound->play();
        //弹跳特效
        chooseBtn3->zoom1();
        chooseBtn3->zoom2();
        //延时进入->让特效充分展示
        QTimer::singleShot(500,this,[=]()
        {
            //进入到选关场景
            //自身隐藏
            this->hide();
            //显示选关场景
            gameWin->show();
        });
        //监听返回按钮信号
        connect(gameWin,&gamewindow::gamewindowback,this,[=](){
            gameWin->hide();
            this->show();
        });
    });
    //返回按钮
    MyPushButton * backBtn = new MyPushButton(BACK_BUTTON);
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width() , this->height()-backBtn->height());
    connect(backBtn,&MyPushButton::clicked,[=]()
    {
        //音效
        chooseSound->play();
        emit this->choosesceneback();
     });
}

/***************绘制选关背景图***************/
void chooselevelscene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(CHOOSE_SCENE);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
