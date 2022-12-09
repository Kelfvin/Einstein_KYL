#ifndef LOGIC_H
#define LOGIC_H

#include <QVector>
#include <QPoint>
#include "board.h"
class Logic
{
public:
    Logic();
private:
    QVector<QVector<int>> redValueChart; //红方价值表
    QVector<QVector<int>> blueValueChart; //蓝方价值表

    QVector<QVector<int>> virtueTable; //存储一个前端的棋盘，防止出错

    QVector<int> redthreaten;
    QVector<int> bluethreaten;
    QVector<int> redValue;           //红方每个棋子的价值
    QVector<int> blueValue;           //蓝方每个棋子的价值
    QVector<float> redProbability;      //红方每个棋子的概率
    QVector<float> blueProbability;     //蓝方每个棋子的概率
    QVector<QVector<bool>> blueprobabilityflag; //蓝方前一个代表对应数字的棋子是否存在，中间代表该数字对应棋子概率是否计算过了，后代表价值
    QVector<QVector<bool>> redprobabilityflag;  //红方前一个代表对应数字的棋子是否存在，中间代表该数字对应棋子概率是否计算过了，后代表价值


    int random; //获取前端的骰子数
    int whoplay;
    int sente=1;
    int depth;

    const float k1=1.0; //需要的k1系数
    const float k2=-2.3f; //需要的k2系数````
    const float k3=-0.1f; //需要的k3系数
    const float k4=0.0; //需要的k4系数
    const int infinity=128;
    const int SIZE=6;
    const int LINE=5;

private:
    bool isThereBlue();
    bool isThereRed();
    bool specialDeal(int& x,int& y);//残局处理

    //博弈函数
    float blueMin(int, int, int, float, float);
    float redMax(int, int, int, float, float);
    float value();
    void blueReady();
    void redReady();
    int judgeResult();
    QPoint blueWhereToGo(int x, int y, int depth, float alpha, float beta);

public:
    //获取前端的数据
    void setBackNeed(int rand,int sente,int depth);
    void setvirtueTable(const QVector<QVector<int>>& board);
    
    //0：要走的棋  1：要走到的位置
    QVector<QPoint> getPointToGo(); 
    
    

};

#endif // LOGIC_H
