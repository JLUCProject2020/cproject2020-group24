#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include "gamewindow.h"

class chooselevelscene : public QMainWindow
{
    Q_OBJECT

public:
    explicit chooselevelscene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    gamewindow * gameWin = NULL;

signals:
    void choosesceneback();

public slots:

};

#endif // CHOOSELEVELSCENE_H
