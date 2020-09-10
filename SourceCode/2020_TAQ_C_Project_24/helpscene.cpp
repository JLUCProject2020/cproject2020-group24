#include "helpscene.h"
#include "gamewindow.h"
#include <QPainter>
#include <config.h>
#include <QPushButton>
#include <mypushbutton.h>
#include<QSound>
#include<QTimer>
#include<mainwindow.h>

helpscene::helpscene(QWidget *parent) : QMainWindow(parent)
{
    //配置关卡场景
    this->setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    //设置图标
    this->setWindowIcon(QPixmap(ANT_PIX));
    //设置标题
    this->setWindowTitle(GAME_TITLE);

    //音效
    QSound * chooseSound = new QSound(SOUND_CLICK);
    //返回按钮
    MyPushButton * backBtn = new MyPushButton(BACK_BUTTON);
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width() , this->height()-backBtn->height());
    connect(backBtn,&MyPushButton::clicked,[=]()
    {
        chooseSound->play();
        emit this->helpsceneback();
    });
}

/***************绘制帮助图***************/
void helpscene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(HELP_SCENE);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}
