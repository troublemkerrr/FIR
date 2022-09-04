#include "mainwindow.h"
#include "ui_mainwindow.h"


gameController * gc=new gameController();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //设置固定窗口大小
     setFixedSize(930,900);
     //设置窗口标题
     setWindowTitle("Five In A Row");

     //设置鼠标追踪功能
     ui->centralwidget->setMouseTracking(true);
     this->setMouseTracking(true);

     gc->initGame();
}

//绘图事件
void MainWindow::paintEvent(QPaintEvent *)
{
    //实例化画家对象 this指的是绘图设备
    QPainter painter(this);
    //设置画笔
    QPen pen(QColor(190,190,190));
    pen.setWidth(3);//笔的粗细
    //让画家使用这个笔
    painter.setPen(pen);
    //设置抗锯齿能力
    painter.setRenderHint(QPainter::Antialiasing);
    //画出棋盘格
    for(int i=0;i<15;i++)
    {
        painter.drawLine(QPoint(LEFT_BORDER,UP_BORDER+i*SIZE),QPoint(LEFT_BORDER+14*SIZE,UP_BORDER+i*SIZE));//横线
        painter.drawLine(QPoint(LEFT_BORDER+i*SIZE,UP_BORDER),QPoint(LEFT_BORDER+i*SIZE,UP_BORDER+14*SIZE));//竖线
    }

    //设置画刷
    QBrush brush(QColor(160,160,160));
    //让画家使用这个画刷
    painter.setBrush(brush);
    //画出棋盘上的五个黑点
    painter.drawEllipse(QPoint(3*SIZE+LEFT_BORDER,3*SIZE+UP_BORDER),5,5);
    painter.drawEllipse(QPoint(3*SIZE+LEFT_BORDER,11*SIZE+UP_BORDER),5,5);
    painter.drawEllipse(QPoint(11*SIZE+LEFT_BORDER,3*SIZE+UP_BORDER),5,5);
    painter.drawEllipse(QPoint(11*SIZE+LEFT_BORDER,11*SIZE+UP_BORDER),5,5);
    painter.drawEllipse(QPoint(7*SIZE+LEFT_BORDER,7*SIZE+UP_BORDER),5,5);
    //画出离鼠标最近的点
    if(row!=-1 && col!=-1 && gc->chessMap[row][col] == 0)
    {
        brush.setColor(QColor(0,255,0,150));
        painter.drawEllipse(QPoint(col*SIZE+LEFT_BORDER,row*SIZE+UP_BORDER),10,10);
    }


    //绘制棋子
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            if(gc->chessMap[i][j]==-1)
            {
                QBrush brush2(QColor(255,255,255));
                painter.setBrush(brush2);
                painter.drawEllipse(QPoint(j*SIZE+LEFT_BORDER,i*SIZE+UP_BORDER),22,22);
            }
            else if(gc->chessMap[i][j]==1)
            {
                QBrush brush3(QColor(0,0,0));
                painter.setBrush(brush3);
                painter.drawEllipse(QPoint(j*SIZE+LEFT_BORDER,i*SIZE+UP_BORDER),22,22);
            }
        }
    }
}

//鼠标移动事件
void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    int x=event->x();
    int y=event->y();
    //只考虑棋盘内部位置
    if(x>=LEFT_BORDER && x<=LEFT_BORDER+14*SIZE && y>=UP_BORDER && y<=UP_BORDER+14*SIZE)
    {
        //看左上角最近交叉点在几行几列
        int r=(y-UP_BORDER)/SIZE;
        int c=(x-LEFT_BORDER)/SIZE;

        //得到左上角最近交叉点的具体坐标位置
        int leftTopPosX = LEFT_BORDER + SIZE * c;
        int leftTopPosY = UP_BORDER + SIZE * r;
        int offset = SIZE * 0.4; //鼠标点击的模糊距离上限

        int len;
        selectPos = false;

        // 距离左上角的距离
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < offset) {
            row = r;
            col = c;
            if (gc->chessMap[row][col] == 0) {
                selectPos = true;
            }
        }

        // 距离右上角的距离
        len = sqrt((x - leftTopPosX - SIZE) * (x - leftTopPosX - SIZE) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < offset) {
            row = r;
            col = c + 1;
            if (gc->chessMap[row][col] == 0) {
                selectPos = true;
            }
        }

        // 距离左下角的距离
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - SIZE) * (y - leftTopPosY - SIZE));
        if (len < offset) {
            row = r + 1;
            col = c;
            if (gc->chessMap[row][col] == 0) {
                selectPos = true;
            }
        }

        // 距离右下角的距离
        len = sqrt((x - leftTopPosX - SIZE) * (x - leftTopPosX - SIZE) + (y - leftTopPosY - SIZE) * (y - leftTopPosY - SIZE));
        if (len < offset) {
            row = r + 1;
            col = c + 1;
            if (gc->chessMap[row][col] == 0) {
                selectPos = true;
            }
        }

        update();
    }
}

//鼠标点击事件
void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    label:
    if(selectPos==false)//没有选中有效点
    {
        return;
    }else{
        selectPos=false;
    }
    //玩家下棋
    if(row!=-1 && col!=-1 && gc->chessMap[row][col] == 0)
    {
        int preRow=row;
        int preCol=col;
        gc->updateBoard(row,col);
        update();
        if(gc->isWin(preRow,preCol,1)){
            QMessageBox::information(this,"result","win!");//信息对话框
            gc->initGame();
            goto label;
        }
    }
    //系统下棋
    QVector<int> v=gc->getBest();
    gc->updateBoard(v[0],v[1]);
    update();
    if(gc->isWin(v[0],v[1],-1)){
        QMessageBox::information(this,"result","lost!");//信息对话框
        gc->initGame();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

