#include "gamecontroller.h"

gameController::gameController()
{
}

//初始化游戏
void gameController::initGame()
{
    chessMap.clear();//先清空棋盘
    //初始化棋盘
    for(int i=0;i<15;i++)
    {
        QVector<int> v;
        for(int j=0;j<15;j++)
        {
            v.push_back(0);
        }
        chessMap.push_back(v);
    }
    playerFlag=true;//确保玩家是黑棋
}

//更新棋盘 记录下过的位置
void gameController::updateBoard(int r,int c)
{
    if(playerFlag)
    {
        chessMap[r][c]=1;//黑棋
    }else
    {
        chessMap[r][c]=-1;
    }
    playerFlag=!playerFlag;
}

//评分计算各个点的价值
void gameController::calculateScore()
{
    int personNum = 0; //玩家方（黑棋）多少个连续的棋子
    int aiNum = 0; //系统方（白棋）连续有多少个连续的棋子
    int emptyNum = 0; // 该方向上空白位的个数

    // 评分向量数组清零
    scoreMap.clear();
    for(int i=0;i<15;i++)
    {
        QVector<int> v;
        for(int j=0;j<15;j++)
        {
            v.push_back(0);
        }
        scoreMap.push_back(v);
    }

    for (int row = 0; row < 15; row++) {
        for (int col = 0; col < 15; col++) {
            //对每个点进行计算
            if (chessMap[row][col]) {continue;} //避开已经下过的点 -1、1都是true

            //控制方向 一共4个正方向：y不变 x+、y- x-、y- x不变、y- x+
            for (int y = -1; y <= 0; y++) {        //Y的范围是-1,0
                for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
                    if (y == 0 && x == 0) {continue;}
                    if (y == 0 && x != 1) {continue;} //当y=0时，仅允许x=1

                    personNum = 0;
                    aiNum = 0;
                    emptyNum = 0;

                    // 假设黑棋在该位置落子，会构成什么棋型
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;

                        if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == 1) {
                            personNum++;
                        }
                        else if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }

                    // 反向继续计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;

                        if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == 1) {
                            personNum++;
                        }
                        else if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }

                    if (personNum == 1) { //连2
                        scoreMap[row][col] += 10;
                    }
                    else if (personNum == 2) {
                        if (emptyNum == 1) { //死3
                            scoreMap[row][col] += 30;
                        }
                        else if (emptyNum == 2) { //活3
                            scoreMap[row][col] += 40;
                        }
                    }
                    else if (personNum == 3) {
                        if (emptyNum == 1) { //死4
                            scoreMap[row][col] = 60;
                        }
                        else if (emptyNum == 2) { //活4
                            scoreMap[row][col] = 5000;
                        }
                    }
                    else if (personNum == 4) { //连5
                        scoreMap[row][col] = 20000;
                    }

                    // 假设白棋在该位置落子，会构成什么棋型
                    emptyNum = 0;

                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;

                        if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == -1) {
                            aiNum++;
                        }
                        else if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }

                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;

                        if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == -1) {
                            aiNum++;
                        }
                        else if (curRow >= 0 && curRow < 15 &&
                            curCol >= 0 && curCol < 15 &&
                            chessMap[curRow][curCol] == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }

                    if (aiNum == 0) {
                        scoreMap[row][col] += 5;
                    }
                    else if (aiNum == 1) { //连2
                        scoreMap[row][col] += 10;
                    }
                    else if (aiNum == 2) {
                        if (emptyNum == 1) { //死3
                            scoreMap[row][col] += 25;
                        }
                        else if (emptyNum == 2) { //活3
                            scoreMap[row][col] += 50;
                        }
                    }
                    else if (aiNum == 3) {
                        if (emptyNum == 1) { //死4
                            scoreMap[row][col] += 55;
                        }
                        else if (emptyNum == 2) { //活4
                            scoreMap[row][col] += 10000;
                        }
                    }
                    else if (aiNum >= 4) { //连5
                        scoreMap[row][col] += 30000;
                    }
                }
            }
        }
    }
}

//找到评分最高的点
QVector<int> gameController::getBest()
{
    // 计算评分
    calculateScore();

    // 从评分中找出最大分数的位置 如果有多个，就选找到的第一个
    int maxScore = 0;
    QVector<int> maxPoint;

    for (int row = 0; row < 15; row++) {
        for (int col = 0; col < 15; col++)
        {
            // 前提是这个坐标是空的
            if (chessMap[row][col] == 0) {
                if (scoreMap[row][col] > maxScore)
                {
                    maxScore = scoreMap[row][col];
                    maxPoint.clear();
                    maxPoint.push_back(row);
                    maxPoint.push_back(col);
                }
            }
        }
    }
    return maxPoint;
}

//看是否五子连珠
bool gameController::isWin(int r,int c,int color)
{
    //水平方向
    int left=1;
    int right=1;
    while(r-left>=0 && chessMap[r-left][c]==color)
    {
        left++;
    }
    while(r+right<15 && chessMap[r+right][c]==color)
    {
        right++;
    }
    if(chessMap[r-left][c]!=color){
        left--;
    }
    if(chessMap[r+right][c]!=color){
        right--;
    }
    if(left+right==4){
        return true;
    }

    //竖直方向
    int up=1;
    int down=1;
    while(c-up>=0 && chessMap[r][c-up]==color)
    {
        up++;
    }
    while(c+down<15 && chessMap[r][c+down]==color)
    {
        down++;
    }
    if(chessMap[r][c-up]!=color){
        up--;
    }
    if(chessMap[r][c+down]!=color){
        down--;
    }
    if(up+down==4){
        return true;
    }

    //"\"方向
    int leftup=1;
    int rightdown=1;
    while(r-leftup>=0 && chessMap[r-leftup][c-leftup]==color)
    {
        leftup++;
    }
    while(r+rightdown<15 && chessMap[r+rightdown][c+rightdown]==color)
    {
        rightdown++;
    }
    if(chessMap[r-leftup][c-leftup]!=color){
        leftup--;
    }
    if(chessMap[r+rightdown][c+rightdown]!=color){
        rightdown--;
    }
    if(leftup+rightdown==4){
        return true;
    }

    //"/"方向
    int leftdown=1;
    int rightup=1;
    while(r-leftdown>=0 && chessMap[r-leftdown][c+leftdown]==color)
    {
        leftdown++;
    }
    while(r+rightup<15 && chessMap[r+rightup][c-rightup]==color)
    {
        rightup++;
    }
    if(chessMap[r-leftdown][c+leftdown]!=color){
        leftdown--;
    }
    if(chessMap[r+rightup][c-rightup]!=color){
        rightup--;
    }
    if(leftdown+rightup==4){
        return true;
    }

    return false;
}

























