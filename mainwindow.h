#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include "gamecontroller.h"
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //绘图事件
    void paintEvent(QPaintEvent *);

    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent * event);

    //鼠标点击事件
    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    int SIZE=63;//每个小格子的大小
    int LEFT_BORDER=24;//棋盘左边界
    int UP_BORDER=9;//棋盘上边界

    //鼠标最靠近的落子位置
    int row=-1;
    int col=-1;

    bool selectPos=false;
};
#endif // MAINWINDOW_H
