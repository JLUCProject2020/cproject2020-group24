#ifndef HELPSCENE_H
#define HELPSCENE_H

#include <QMainWindow>

class helpscene : public QMainWindow
{
    Q_OBJECT

public:
    explicit helpscene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

signals:
    //自定义信号
    void helpsceneback();

public slots:

};

#endif // HELPSCENE_H
