#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <QVector>



class gameController
{

public:
    gameController();

    //存储当前游戏棋盘和棋子的情况,空白为0，黑子1，白子-1
    QVector<QVector<int>> chessMap;

    //初始化游戏
    void initGame();

    //更新棋盘 记录下过的位置
    void updateBoard(int row,int col);

    //评分计算各个点的价值
    void calculateScore();

    //找到评分最高的点
    QVector<int> getBest();

    //看是否五子连珠
    bool isWin(int r,int c,int color);

    //玩家true 系统false
    bool playerFlag=true;

private:
    //评分表
    QVector<QVector<int>> scoreMap;



};

#endif // GAMECONTROLLER_H
