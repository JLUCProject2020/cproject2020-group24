#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MyPushButton(QWidget *parent = nullptr);
    MyPushButton(QString normalImg, QString pressImg="");
    QString normalImgPath;//参数1  正常显示图片路径
    QString pressImgPath;//参数2  按下后显示图片路径
    //弹跳特效
    void zoom1 ();
    void zoom2();

signals:

public slots:

};

#endif // MYPUSHBUTTON_H
