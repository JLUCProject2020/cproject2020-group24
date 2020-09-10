#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <cstdio>
#include <cstring>
#include <algorithm>//算法库
#include <config.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <queue>
#include <utility>
#include <QTimer>
#include <mypushbutton.h>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QSound>
#include <QLabel>
#include <QPixmap>
#include <QRect>
#include <QPropertyAnimation>
#include <QEasingCurve>

using namespace std;

gamewindow::gamewindow(int level)
    :ui(new Ui::gamewindow)
{
     ui->setupUi(this);
    //创建地图
    makemap(level);

    //配置关卡场景
    iniscene();


}

//键盘按下事件
void gamewindow::keyPressEvent(QKeyEvent * event)
{
    //禁用键盘
    if(isWin||isLose) return;
    switch(event->key())
    {
        case Qt::Key_Up:
        case Qt::Key_W:
        if(px>1)
        {
            mp[px][py]=0;
            px--;
            move(px,py);
            //qDebug()<<"up";
        }
        break;
        case Qt::Key_Down:
        case Qt::Key_S:
        if(px<n)
        {
            if(px+1==n&&py==n&&fd>0) break;
            mp[px][py]=0;
            px++;
            move(px,py);
            //qDebug()<<"down";
        }
        break;
        case Qt::Key_Left:
        case Qt::Key_A:
        if(py>1)
        {
            mp[px][py]=0;
            py--;
            move(px,py);
            //qDebug()<<"left";
        }
        break;
        case Qt::Key_Right:
        case Qt::Key_D:
        if(py<n)
        {
            if(px==n&&py+1==n&&fd>0) break;
            mp[px][py]=0;
            py++;
            move(px,py);
            //qDebug()<<"right";
        }
        break;
    }
    update();//每次移动后重新绘制地图

    //利用队列实现bfs查找最近的蜘蛛和食物
    int nfd=0,nsp=0;
    int inf[25][25]={0},dir[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
    queue<data> q;
    data begp;
    begp.xi=px, begp.yi=py, begp.dis=0;
    inf[px][py]=1;
    q.push(begp);
    //从队列头拿出一个点，遍历四个方向，更新最短距离，打上标签，把它放到队列里。把头删掉
    while(!q.empty())
    {
        data frop=q.front();
        for(int i=0;i<4;i++)
        {
            data recp;
            int nx=frop.xi+dir[i][0], ny=frop.yi+dir[i][1], dis=frop.dis+1;
            if(inf[nx][ny]) continue;
            if(nx<=0||nx>n||ny<=0||ny>n) continue;
            recp.xi=nx, recp.yi=ny, recp.dis=dis;
            inf[px][py]=1;
            if(mp[nx][ny]==1&&(!nfd)) nfd=dis;
            if(mp[nx][ny]==2&&(!nsp)) nsp=dis;
            q.push(recp);
        }
        q.pop();
        if(nfd&&nsp) break;
        if(nfd&&(!sp)) break;
        if(nsp&&(!fd)) break;
        if((!sp)&&(!fd)) break;
    }
    qDebug("position: %d %d",px,py);
//    //显示位置
//    status.append("position:");
//    status.append(QString::number(px));
//    status.append(QString::number(py));
//    status.append("\n");
//    //显示生命值
//    status.append("life:");
//    status.append(QString::number(lf));
//    status.append("\n");
//    //显示剩余食物
//    status.append("food left:");
//    status.append(QString::number(fd));
//    status.append("\n");
//    //显示剩余蜘蛛
//    status.append("spider left:");
//    status.append(QString::number(sp));
//    status.append("\n");
//    //最近的食物的距离
//    status.append("nearest food left:");
//    status.append(QString::number(nfd));
//    status.append("\n");
//    //最近的蜘蛛距离
//    status.append("nearest spider left:");
//    status.append(QString::number(nsp));
    qDebug("life: %d",lf);
    qDebug("food left: %d",fd);
    qDebug("spider left: %d",sp);
    qDebug("nearest food left: %d",nfd);
    x=nfd;
    qDebug("nearest spider left: %d",nsp);
    y=nsp;
    if(isWin==true) qDebug("You have won!");
    if(isLose==true) qDebug("You have lost!");
    gamewindow::ui->labelStatus->setText(status);
}

//绘制当前地图
void gamewindow::paintEvent(QPaintEvent *)
{

    //根据数组mp和mpx，画出当前的地图
    QPainter painter(this);

    //绘制背景图
    QPixmap pix;
    pix.load(GAME_SCENE);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //绘制标准网格线，制图时作为参考
//    for(int i=1;i<=10;i++) painter.drawLine(QPointF(100*i,0), QPointF(100*i,1000));
//    for(int i=1;i<=10;i++) painter.drawLine(QPointF(0,100*i), QPointF(1000,100*i));
    painter.drawPixmap(5000,5000,QPixmap(":/res/bbb/life.JPG"));
    int lef,rt;
    lef=rt=(25-2*n)*20; //计算整张图最左上角的坐标
    int edge=80; //定义小方格图片大小
    if(!f) //画完全显示模式
    {
        for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
        {
            int xa=lef+(j-1)*edge,ya=rt+(i-1)*edge; //计算该小方格左上角的坐标
            //按照mp地图的值画出地图
            if(mp[i][j]==0) painter.drawPixmap(xa,ya,QPixmap(":/res/aaa/SOIL.png"));
            if(mp[i][j]==1) painter.drawPixmap(xa,ya,QPixmap(":/res/bbb/thefood.png"));
            if(mp[i][j]==2) painter.drawPixmap(xa,ya,QPixmap(":/res/bbb/thespider.png"));
            if(mp[i][j]==3) painter.drawPixmap(xa,ya,QPixmap(":/res/bbb/theant.png"));
            if(mp[i][j]==4) painter.drawPixmap(xa,ya,QPixmap(":/res/bbb/home.JPG"));
            //painter.drawRect(xa,ya,edge,edge);
        }
    }
    else //画带迷雾的地图
    {
        for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
        {
            int xa=lef+(j-1)*edge,ya=rt+(i-1)*edge;
            if(mp[i][j]==3) painter.drawPixmap(xa,ya,QPixmap(":/res/bbb/theant.png"));
            if(mp[i][j]==0) painter.drawPixmap(xa,ya,QPixmap(":/res/aaa/SOIL.png"));
            if(mp[i][j]==4) painter.drawPixmap(xa,ya,QPixmap(":/res/bbb/home.JPG"));
            if(!mpx[i][j]) //绘制迷雾
            {
                painter.setBrush(QBrush(Qt::white));
                painter.drawRect(xa,ya,edge,edge);
                continue;
            }            
        }
    }
    qDebug("nearest food left: %d",x);
    painter.drawPixmap(100,25,QPixmap(":/res/bbb/life.PNG"));
    painter.drawPixmap(925,175,QPixmap(":/res/bbb/fd.PNG"));
    painter.drawPixmap(925,325,QPixmap(":/res/bbb/fdd.PNG"));
    painter.drawPixmap(925,475,QPixmap(":/res/bbb/sp.PNG"));
    painter.drawPixmap(925,625,QPixmap(":/res/bbb/spd.PNG"));
    if(lf==3)painter.drawPixmap(400,25,QPixmap(lf3));
    if(lf==2)painter.drawPixmap(400,25,QPixmap(lf2));
    if(lf==1)painter.drawPixmap(400,25,QPixmap(lf1));
    if(lf==0)painter.drawPixmap(400,25,QPixmap(lf0));
    if(fd==0)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/00.png"));
    if(fd==1)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/01.png"));
    if(fd==2)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/02.png"));
    if(fd==3)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/03.png"));
    if(fd==4)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/04.png"));
    if(fd==5)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/05.png"));
    if(fd==6)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/06.png"));
    if(fd==7)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/07.png"));
    if(fd==8)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/08.png"));
    if(fd==9)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/09.png"));
    if(fd==10)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/10.png"));
    if(fd==11)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/11.png"));
    if(fd==12)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/12.png"));
    if(fd==13)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/13.png"));
    if(fd==14)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/14.png"));
    if(fd==15)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/15.png"));
    if(fd==16)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/16.png"));
    if(fd==17)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/17.png"));
    if(fd==18)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/18.png"));
    if(fd==19)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/19.png"));
    if(fd==20)painter.drawPixmap(1400,175,QPixmap(":/res/numbers/20.png"));
    if(sp==0)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/00.png"));
    if(sp==1)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/01.png"));
    if(sp==2)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/02.png"));
    if(sp==3)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/03.png"));
    if(sp==4)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/04.png"));
    if(sp==5)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/05.png"));
    if(sp==6)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/06.png"));
    if(sp==7)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/07.png"));
    if(sp==8)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/08.png"));
    if(sp==9)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/09.png"));
    if(sp==10)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/10.png"));
    if(sp==11)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/11.png"));
    if(sp==12)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/12.png"));
    if(sp==13)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/13.png"));
    if(sp==14)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/14.png"));
    if(sp==15)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/15.png"));
    if(sp==16)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/16.png"));
    if(sp==17)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/17.png"));
    if(sp==18)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/18.png"));
    if(sp==19)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/19.png"));
    if(sp==20)painter.drawPixmap(1400,475,QPixmap(":/res/numbers/20.png"));
    if(y==0)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/00.png"));
    if(y==1)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/01.png"));
    if(y==2)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/02.png"));
    if(y==3)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/03.png"));
    if(y==4)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/04.png"));
    if(y==5)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/05.png"));
    if(y==6)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/06.png"));
    if(y==7)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/07.png"));
    if(y==8)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/08.png"));
    if(y==9)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/09.png"));
    if(y==10)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/10.png"));
    if(y==11)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/11.png"));
    if(y==12)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/12.png"));
    if(y==13)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/13.png"));
    if(y==14)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/14.png"));
    if(y==15)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/15.png"));
    if(y==16)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/16.png"));
    if(y==17)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/17.png"));
    if(y==18)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/18.png"));
    if(y==19)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/19.png"));
    if(y==20)painter.drawPixmap(1400,625,QPixmap(":/res/numbers/20.png"));
    if(x==0)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/00.png"));
    if(x==1)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/01.png"));
    if(x==2)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/02.png"));
    if(x==3)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/03.png"));
    if(x==4)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/04.png"));
    if(x==5)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/05.png"));
    if(x==6)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/06.png"));
    if(x==7)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/07.png"));
    if(x==8)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/08.png"));
    if(x==9)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/09.png"));
    if(x==10)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/10.png"));
    if(x==11)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/11.png"));
    if(x==12)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/12.png"));
    if(x==13)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/13.png"));
    if(x==14)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/14.png"));
    if(x==15)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/15.png"));
    if(x==16)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/16.png"));
    if(x==17)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/17.png"));
    if(x==18)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/18.png"));
    if(x==19)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/19.png"));
    if(x==20)painter.drawPixmap(1400,325,QPixmap(":/res/numbers/20.png"));
}

//生成随机地图
void gamewindow::makemap(int level)
{
    //根据难度等级划定地图大小
    switch(level)
    {
        case 1: n=5;break;
        case 2: n=7;break;
        case 3: n=10;break;
        default: break;
    }
    //全图中有1/5的蜘蛛和1/5的食物
    fd=sp=(n*n+1)/5;

    //绘制地图mp和迷雾地图mpx
    mp[1][1]=3;//此处为蚂蚁
    mp[n][n]=4;//此处为终点
    mpx[1][1]=1;//初始位置不加迷雾
    mpx[n][n]=1;//终点位置不加迷雾
    //设置随机种子，调用系统时间，进行迭代生成随机数
    srand((unsigned)time(0));
    for(int i=1;i<=fd;i++)
    {
        int xi=rand()%n+1,yi=rand()%n+1;
        while(mp[xi][yi])
            xi=rand()%n+1,yi=rand()%n+1;
        mp[xi][yi]=1;
    }

    while(1)
    {
        memset(pdedge,0,sizeof(pdedge));
        memset(outd,0,sizeof(outd));
        for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            if(mp[i][j]==2) mp[i][j]=0;
        for(int i=1;i<=sp;i++)
        {
            int xi=rand()%n+1,yi=rand()%n+1;
            while(mp[xi][yi])
                xi=rand()%n+1,yi=rand()%n+1;
            mp[xi][yi]=2;
            psp[i].x=xi,psp[i].y=yi;
            if(xi==1||xi==n||yi==1||yi==n) pdedge[i]=1;
        }
        if(!pdbw()) break;
    }
}

//并查集去掉包围圈
int gamewindow::pdbw()
{
    memset(pdlt,0,sizeof(pdlt));
    for(int i=1;i<=sp;i++) fa[i]=i;

    int dirp[8][2]={{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};
    for(int i=1;i<=sp;i++)
    for(int j=i+1;j<=sp;j++)
    {
        for(int k=0;k<8;k++)
            if(psp[i].x+dirp[k][0]==psp[j].x&&psp[i].y+dirp[k][1]==psp[j].y)
            {
                pdlt[i][j]=pdlt[j][i]=1;
                outd[i]++;
                outd[j]++;
                break;
            }
        if(pdlt[i][j])
        {
            int fx=findfa(i), fy=findfa(j);
            if(fx>fy) swap(fx,fy);
            fa[fy]=fx;
        }
    }
    /*qDebug("tests try:");
    for(int i=1;i<=sp;i++)
        qDebug("%d edge:%d fa:%d outd:%d po:(%d,%d)",i,pdedge[i],fa[i],outd[i],psp[i].x,psp[i].y);
    for(int i=1;i<=n;i++)
        qDebug("%d %d %d %d %d",pdlt[i][1],pdlt[i][2],pdlt[i][3],pdlt[i][4],pdlt[i][5]);*/

    //如果两个边界值相连，意味着被堵在了墙角，存在包围圈
    for(int i=1;i<=sp;i++)
    for(int j=i+1;j<=sp;j++)
        if(pdedge[i]&&pdedge[j]&&fa[i]==fa[j]) return 1;

    //如果存在闭环，意味着存在包围圈
    queue<int> q1;
    int del[25]={0};
    for(int i=1;i<=sp;i++)
        if(outd[i]==0) del[i]=1;
    for(int i=1;i<=sp;i++)
        if(outd[i]==1) q1.push(i);
    while(!q1.empty())
    {
        int x=q1.front();
        del[x]=1;
        for(int i=1;i<=n;i++)
            if(pdlt[x][i])
            {
                pdlt[x][i]=pdlt[i][x]=0;
                outd[i]--;
                if(outd[i]==0) del[i]=1;
                if(outd[i]==1) q1.push(i);
            }
        q1.pop();
    }
    for(int i=1;i<=sp;i++)
        if(!del[i]) return 1;

    return 0;
}

int gamewindow::findfa(int x)
{
    if(fa[x]==x) return fa[x];
    fa[x]=findfa(fa[x]);
    return fa[x];
}


//移动函数，处理蚂蚁的移动带来的变化
void gamewindow::move(int px,int py)
{

    switch(mp[px][py])
    {
        case 1: fd--;break;
        case 2: sp--,lf--;break;
        case 4:
        {
            isWin=true;
            //游戏胜利弹出界面
            if(isWin==true)
            {
                //将胜利图片弹下来
                QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
                //设置时间间隔
                animation->setDuration(1000);
                //设置开始位置
                animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                //设置结束位置
                animation->setEndValue(QRect(winLabel->x(),winLabel->y()+400,winLabel->width(),winLabel->height()));
                //设置缓和曲线
                animation->setEasingCurve(QEasingCurve::OutBounce);
                //执行动画
                animation->start();
            }
        }
        break;
        default: break;
    }
    //游戏失败弹出界面
    if(!lf) isLose=true;
    if(isLose==true)
    {
        //将失败图片弹下来
        QPropertyAnimation * animation = new QPropertyAnimation(loseLabel,"geometry");
        //设置时间间隔
        animation->setDuration(1000);
        //设置开始位置
        animation->setStartValue(QRect(loseLabel->x(),loseLabel->y(),loseLabel->width(),loseLabel->height()));
        //设置结束位置
        animation->setEndValue(QRect(loseLabel->x(),loseLabel->y()+400,loseLabel->width(),loseLabel->height()));
        //设置缓和曲线
        animation->setEasingCurve(QEasingCurve::OutBounce);
        //执行动画
        animation->start();
    }
    mp[px][py]=3;
    mpx[px][py]=1;
    //迷雾变量
    f=1;
}

//配置场景
void gamewindow::iniscene()
{

    this->setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    //设置图标
    this->setWindowIcon(QPixmap(ANT_PIX));
    //设置标题
    this->setWindowTitle(GAME_TITLE);
    //音效
    QSound * chooseSound = new QSound(SOUND_CLICK);
    //返回按钮（返回同时清空地图）
    MyPushButton * backBtn = new MyPushButton(BACK_BUTTON);
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width() , this->height()-backBtn->height());
    connect(backBtn,&MyPushButton::clicked,[=]()
    {
        //清空地图
        memset(mp,0,sizeof(mp));
        memset(mpx,0,sizeof(mpx));
        //音效
        chooseSound->play();
        emit this->gamewindowback();
     });
    //胜利图片显示
    winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/bbb/WINpage.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());

    //失败图片显示
    loseLabel = new QLabel;
    QPixmap tmpPix2;
    tmpPix2.load(":/res/bbb/GAMEOVERpage.png");
    loseLabel->setGeometry(0,0,tmpPix2.width(),tmpPix2.height());
    loseLabel->setPixmap(tmpPix2);
    loseLabel->setParent(this);
    loseLabel->move((this->width()-tmpPix2.width())*0.5,-tmpPix2.height());
}

gamewindow::~gamewindow()
{
    delete ui;
}



