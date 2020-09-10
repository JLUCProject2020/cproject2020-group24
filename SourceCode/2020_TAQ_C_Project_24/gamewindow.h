#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>

namespace Ui {
class gamewindow;
}

class gamewindow:public QMainWindow
{
    Q_OBJECT//QT中定义的一个宏

public:
    bool isWin=false,isLose=false;//胜利失败标签
    int n,fd,sp,mp[25][25]={0},mpx[25][25]={0};//记录地图基本信息
    int x=0,y=0;
    /*
     在mp地图数组中
        食物->1 蓝
        蜘蛛->2 红
        当前位置(蚂蚁)->3 绿
        终点->4
     在mpx迷雾数组中
        0->有迷雾
        1->无迷雾
    */
    int px=1,py=1;//表示当前位置

    //并查集求连通块
    struct data1{
        int x,y;
    }psp[25];
    int fa[25],pdlt[25][25]={0};
    int pdedge[25]={0};//pdedg判断第i个蜘蛛是否在边缘上
    int outd[25]={0};

    int lf=3;//生命值
    int f=0;//绘图使用的标记

    //bfs搜索最近蜘蛛和食物
    struct data{
        int xi,yi;
        int dis;
    };

    gamewindow(int level);

    void makemap(int level);
    int pdbw();
    int findfa(int x);

    void move(int px,int py);

    void iniscene();

    ~gamewindow();//管理内存的析构函数
    //一个标签的挂件，定义一个QLabel类型的指针叫。。。。初始值为空。
    QLabel *winLabel= NULL;
    QLabel *loseLabel=NULL;

signals:
    void gamewindowback();

private:
    Ui::gamewindow *ui;
    QString status;
//    void showWinLabel();
//    void showStatus();

protected:
    void keyPressEvent(QKeyEvent *event); //键盘按下事件
    void paintEvent(QPaintEvent *);//绘图事件
};

#endif //GAMEWINDOW_H
