#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include <QMap>
#include <QPoint>


class Board
{
public:
    Board();

    bool setChess(int x , int y ,int value); // 放置棋子，不会加入到备份中
    bool moveChess(int startx,int starty,int endx,int endy); // 移动棋子，会加入到备份当中
    bool undo();    // 撤销
    bool cancelUndo();   // 取消撤销
    void backup();
    bool checkBoardSet(); // 棋局的设置是否合理
    int getChess(int x , int y);
    bool canUndo();

    QString getNowPlayerStr();
    int getNowPlayer();
    int getOurColor();
    bool setDice(int dice);

    void setOurColor(int color);
    void setNowPlayer(int color);

    const QVector<QVector<int>>& getBoard();
    int getSente();
    void setSente(int color);
    int getDice();



    /**
     * @brief checkWin  查看谁赢了
     * @return  -1 红方赢，1蓝方赢，0游戏还需要继续
     */
    int checkWin();

private:
    /**
     * @brief addToChessMap 将棋子的坐标更新到 ChessMap中
     * @param value 棋子的标号
     * @param x
     * @param y
     */
    void addToChessMap(int value ,int x , int y);
    void initDefaultChessSet();
    void removeFromChessMap(int value);


private:
    // 当前棋局
    QVector<QVector<int>> board;

    int dice; //骰子数

    // 保存我红方的蓝方的棋子，还有棋子的位置，0位置为x，1位置为y
    QMap<int,QPoint> redChessMap;
    QMap<int,QPoint> blueChessMap;

    // 用于棋局备份
    QVector<QVector<QVector<int>>> backupBoards;
    int backupIndex = -1; // 指向棋局备份的下标，用于撤销，和取消撤销

    // 队伍，选手信息
    int ourColor = 1;  // 我们队伍的颜色，1 为蓝色， -1为红色
    int nowPlayer = 1; // 当前轮到的选手

    // 先手是谁
    int sente = 1;
};

#endif // BOARD_H
