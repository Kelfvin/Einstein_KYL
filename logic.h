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
    QVector<QVector<int>> redValueChart; //�췽��ֵ��
    QVector<QVector<int>> blueValueChart; //������ֵ��

    QVector<QVector<int>> virtueTable; //�洢һ��ǰ�˵����̣���ֹ����

    QVector<int> redthreaten;
    QVector<int> bluethreaten;
    QVector<int> redValue;           //�췽ÿ�����ӵļ�ֵ
    QVector<int> blueValue;           //����ÿ�����ӵļ�ֵ
    QVector<float> redProbability;      //�췽ÿ�����ӵĸ���
    QVector<float> blueProbability;     //����ÿ�����ӵĸ���
    QVector<QVector<bool>> blueprobabilityflag; //����ǰһ�������Ӧ���ֵ������Ƿ���ڣ��м��������ֶ�Ӧ���Ӹ����Ƿ������ˣ�������ֵ
    QVector<QVector<bool>> redprobabilityflag;  //�췽ǰһ�������Ӧ���ֵ������Ƿ���ڣ��м��������ֶ�Ӧ���Ӹ����Ƿ������ˣ�������ֵ


    int random; //��ȡǰ�˵�������
    int whoplay;
    int sente=1;
    int depth;

    const float k1=1.0; //��Ҫ��k1ϵ��
    const float k2=-2.3f; //��Ҫ��k2ϵ��````
    const float k3=-0.1f; //��Ҫ��k3ϵ��
    const float k4=0.0; //��Ҫ��k4ϵ��
    const int infinity=128;
    const int SIZE=6;
    const int LINE=5;

private:
    bool isThereBlue();
    bool isThereRed();
    bool specialDeal(int& x,int& y);//�оִ���

    //���ĺ���
    float blueMin(int, int, int, float, float);
    float redMax(int, int, int, float, float);
    float value();
    void blueReady();
    void redReady();
    int judgeResult();
    QPoint blueWhereToGo(int x, int y, int depth, float alpha, float beta);

public:
    //��ȡǰ�˵������
    void setBackNeed(int rand,int sente,int depth);
    QVector<QPoint> getPointToGo();
    void setvirtueTable(const QVector<QVector<int>>& board);
    

};

#endif // LOGIC_H
