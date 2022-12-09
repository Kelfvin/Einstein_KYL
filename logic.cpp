#include "logic.h"
#include <qmath.h>
#include <QDebug>


Logic::Logic()
{
    whoplay++;
    redValueChart={
        {0,2,2,2,2},
        {2,4,4,4,5},
        {2,4,8,8,10},
        {2,4,8,16,20},
        {2,5,10,20,32}
    };
    blueValueChart={
        {32,20,10,5,2},
        {20,12,12,12,2},
        {10,12,8,8,2},
        {5,12,8,4,2},
        {2,2,2,2,1},
    };

    redValue.resize(SIZE);
    blueValue.resize(SIZE);
    redProbability.resize(SIZE);
    blueProbability.resize(SIZE);
    redthreaten.resize(SIZE);
    bluethreaten.resize(SIZE);

    blueprobabilityflag.resize(SIZE);
    for (int i=0;i<SIZE;i++) {
        blueprobabilityflag[i].resize(2);
    }

    redprobabilityflag.resize(SIZE);
    for (int i=0;i<SIZE;i++) {
        redprobabilityflag[i].resize(2);
    }

    virtueTable.resize(LINE);
    for (int i=0;i<LINE;i++) {
        redprobabilityflag[i].resize(LINE);
    }

}


bool Logic::isThereBlue()
{
    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            if(virtueTable[i][j]>0){
                return true;
            }
        }
    }
    return false;
}


bool Logic::isThereRed()
{
    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            if(virtueTable[i][j]<0){
                return true;
            }
        }
    }
    return false;
}

int Logic::judgeResult()
{
    if (virtueTable[0][0] > 0 || !isThereRed())
    {
        //ui->back->setEnabled(true);
        return 1;
    }
    if (virtueTable[4][4] < 0 || !isThereBlue())
    { //ui->back->setEnabled(true);
        return 2;
    }
    return 0;
}

QPoint Logic::blueWhereToGo(int x, int y, int depth, float alpha, float beta)
{

    int a1 = 0; //a���ڱ��������������ֵ
    float val = 0;
    float temp = 0;
    int flag = 0;
    int num = 0;
    int bestmoveX;
    int bestmoveY;
    if (x > 0 && y > 0){ //�����Ϸ�
        if (specialDeal(x,y)) {
            return QPoint(x,y);
        }
        a1 = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�������ϵ��ֵ

        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }

        qDebug() << val;
        //�����岽��ɱ�öԷ�һ��Ҳû����
        bestmoveX = x - 1;
        bestmoveY = y - 1;
        if (flag == 0)
        {
            bestmoveX = x - 1;
            bestmoveY = y - 1;
            x = bestmoveX;
            y = bestmoveY;
            return QPoint(x,y);
        }
        flag = 0;
        //�ָ�����
        virtueTable[x][y] = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = a1;
        //����������--------------------------------------------------------


        //���������--------------------------------------------------------
        a1 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //������߼�ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }
        qDebug() << temp;
        //�����岽
        if (flag == 0)
        {
            bestmoveX = x - 1;
            bestmoveY = y;
            x = bestmoveX;
            y = bestmoveY;
            return QPoint(x,y);
        }
        flag = 0;
        if (temp < val)
        {
            val = temp;
            bestmoveX = x - 1;
            bestmoveY = y;
        }
        temp = 0;
        //�ָ�����
        virtueTable[x][y] = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = a1;
        //���������--------------------------------------------------------


        //�������Ϸ�--------------------------------------------------------
        a1 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�����Ϸ���ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }
        qDebug() << temp;
        if (flag == 0)
        {
            bestmoveX = x;
            bestmoveY = y - 1;
            x = bestmoveX;
            y = bestmoveY;
            return QPoint(x,y);
        }
        flag = 0;
        //�����岽
        if (temp < val)
        {
            val = temp;
            bestmoveX = x;
            bestmoveY = y - 1;
        }
        //�������Ϸ�--------------------------------------------------------


        //�ָ�����
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a1;
        //���������岽
        //qDebug()<<num1;
        /********�����ƶ�*********/
        x = bestmoveX;
        y = bestmoveY;
        return QPoint(x,y);
        /********�����ƶ�*********/
    }
    else if (x == 0) //���Ϊǽ���ò��Ź�ֵ����Ϊֻ���߰�
    {
        //qDebug()<<"BlueWhereToGo "<<depth<<":1";
        y = y - 1;
        return QPoint(x,y);
    }
    else if (y == 0) //�Ϸ�Ϊǽ���ò��Ź�ֵ����Ϊֻ���߰�
    {
        //qDebug()<<"BlueWhereToGo "<<depth<<":1";
        x = x - 1;
        return QPoint(x,y);
    }



}



bool Logic::specialDeal(int& x,int& y)
{
    //0731 ���Խ���ȡ����
    int num1 = 0;
    int bestmoveX;
    int bestmoveY;
    if (x > 0 && y > 0) //�����Ϸ�
    {

        int n = LINE, j = 0, m = 0;
        while (n--)
        {
            for (j = m; j < 5; j++)
            {
                if (virtueTable[j][n] < 0)
                    num1++;
            }
            m++;
        }

        if (whoplay >= 18)
        {

            int chess_number = 0;
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    if (virtueTable[i][j] != 0)
                        chess_number++;
                }
            if (chess_number == 2)
            {
                int x1 = (x - 1);
                int y1 = (y - 1);
                if (virtueTable[x1 - 1][y1 - 1] < 0)
                {

                    /*
                    bestmoveX=x-1;
                    bestmoveY=y;
                    x=bestmoveX;
                    y=bestmoveY;*/
                    if (x == 4)
                    {
                        bestmoveX = x - 1;
                        bestmoveY = y;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                    if (y == 4)
                    {
                        bestmoveX = x;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                    else
                    {
                        bestmoveX = x;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                }
            }
        }

        //0801��ʽ����
        if (whoplay >= 10)
        {
            int x1 = (x - 1);
            int y1 = (y - 1);
            if (x == 2 && y == 1)
            {
                if (virtueTable[0][0] == 0 && virtueTable[1][0] < 0 && virtueTable[1][1] == 0 && virtueTable[0][1] == 0)
                {
                    bestmoveX = x - 1;
                    bestmoveY = y - 1;
                    x = bestmoveX;
                    y = bestmoveY;
                    return true;
                }
            }
            if (x == 2 && y == 3)
            {
                if (virtueTable[x - 1][y - 1] < 0)
                {

                    if (virtueTable[x1 - 1][y1 - 1] == 0 && virtueTable[x1][y1 - 1] == 0 && virtueTable[x1 - 1][y1] == 0)
                    {
                        bestmoveX = x - 1;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                }
            }
            if (x == 1 && y == 3)
            {
                if (virtueTable[x - 1][y - 1] < 0)
                {
                    int x1 = (x - 1);
                    int y1 = (y - 1);
                    if (virtueTable[x1][y1 - 1] == 0)
                    {
                        bestmoveX = x - 1;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                }
            }
            if (x == 1 && y == 2)
            {
                if (virtueTable[x - 1][y - 1] < 0)
                {
                    int x1 = (x - 1);
                    int y1 = (y - 1);
                    if (virtueTable[x1][y1 - 1] == 0)
                    {
                        bestmoveX = x - 1;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                }
            }

            //0801��ʽ����
            if (x == 3 && y == 2)
            {
                if (virtueTable[x - 1][y - 1] < 0)
                {

                    if (virtueTable[x1 - 1][y1 - 1] == 0 && virtueTable[x1][y1 - 1] == 0 && virtueTable[x1 - 1][y1] == 0)
                    {
                        bestmoveX = x - 1;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                }
            }
            if (x == 3 && y == 1)
            {
                if (virtueTable[x - 1][y - 1] < 0)
                {
                    int x1 = (x - 1);
                    int y1 = (y - 1);
                    if (virtueTable[x1 - 1][y1] == 0)
                    {
                        bestmoveX = x - 1;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                }
            }

            if (x == 2 && y == 1)
            {
                if (virtueTable[x - 1][y - 1] < 0)
                {
                    int x1 = (x - 1);
                    int y1 = (y - 1);
                    if (virtueTable[x1 - 1][y1] == 0)
                    {
                        bestmoveX = x - 1;
                        bestmoveY = y - 1;
                        x = bestmoveX;
                        y = bestmoveY;
                        return true;
                    }
                }
            }
        }

        //0731 ���Խ���ȡ������ʽ����
        if (whoplay <= 10 && num1 == 1 && ((x == 2 && y == 3) || (x == 2 && y == 4) || (x == 3 && y == 2) || (x == 4 && y == 2) || (x == 3 && y == 3)))
        {
            if (virtueTable[x - 1][y] < 0)
            {
                bestmoveX = x - 1;
                bestmoveY = y;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (virtueTable[x - 1][y - 1] < 0)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (virtueTable[x][y - 1] < 0)
            {
                bestmoveX = x;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
        }
        //0801

        //0801
        if (whoplay >= 8 && ((x == 2 && y == 1) || (x == 1 && y == 2)) && ((virtueTable[0][0] < 0 && virtueTable[1][0] < 0) || (virtueTable[0][0] < 0 && virtueTable[0][1] < 0)) && (!(virtueTable[4][3] < 0 || virtueTable[3][3] < 0 || virtueTable[3][4] < 0)))
        { //qDebug()<<123456;
            bestmoveX = x - 1;
            bestmoveY = y - 1;
            x = bestmoveX;
            y = bestmoveY;
            return true;
        }
        /*if(whoplay>=10)
      {   int num_red=0;
          for(int i=0;i<5;++i)
          {
              for(int j=0;j<5;++j)
              {

              }
          }
      }*/
        //0801 ��ʽ����
        if (num1 == 0 && ((x == 2 && y == 4) || (x == 4 && y == 2)))
        {
            bestmoveX = x - 1;
            bestmoveY = y - 1;
            x = bestmoveX;
            y = bestmoveY;
            return true;
        }
        if (x == 1 && y == 1)
        {
            bestmoveX = x - 1;
            bestmoveY = y - 1;
            x = bestmoveX;
            y = bestmoveY;
            return true;
        }

        if (sente==1 && whoplay == 1)
        {
            if (x == 4 && y == 3)
            {
                bestmoveX = x;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 3 && y == 4)
            {
                bestmoveX = x - 1;
                bestmoveY = y;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 4 && y == 4)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 3 && y == 3)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 2 && y == 4)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 4 && y == 2)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
        }
        if (sente==1 && whoplay == 0)
        {
            if (x == 4 && y == 3)
            {
                bestmoveX = x;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 3 && y == 4)
            {
                bestmoveX = x - 1;
                bestmoveY = y;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 4 && y == 4)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 3 && y == 3)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 2 && y == 4)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
            if (x == 4 && y == 2)
            {
                bestmoveX = x - 1;
                bestmoveY = y - 1;
                x = bestmoveX;
                y = bestmoveY;
                return true;
            }
        }
    }
    return false;
}

void Logic::setBackNeed(int rand,int sente,int depth)
{
    this->random=rand;
    this->whoplay;
    this->sente=sente;
    this->depth=depth;
}

float Logic::blueMin(int x, int y, int depth, float alpha, float beta)
{

    //ʤ���Ѷ�
    //�ж���Ӯ��ӡ��Ϣ
    if (judgeResult() == 1)
    {
        return -infinity;
    }
    if (judgeResult() == 2)
    {
        return infinity;
    }

    int a2 = 0;
    float val = 0, temp = 0;

    if (depth == 0)
    {
        if (x > 0 && y > 0) //�����Ϸ�
        {
            //������������Ѱ��value()���ֵ
            //qDebug()<<"BlueMin "<<depth<<":3";
            //����
            a2 = virtueTable[x - 1][y - 1];
            virtueTable[x - 1][y - 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x - 1][y - 1];
            virtueTable[x - 1][y - 1] = a2;
            //Alpha��֦
            beta = qMin(beta, val);
            if (beta <= alpha)
                return beta;

            //���
            a2 = virtueTable[x - 1][y];
            virtueTable[x - 1][y] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            temp = value();
            virtueTable[x][y] = virtueTable[x - 1][y];
            if (temp < val)
            {
                val = temp;
            }
            virtueTable[x - 1][y] = a2;
            //Alpha��֦
            beta = qMin(beta, val);
            if (beta <= alpha)
                return beta;

            //�Ϸ�
            a2 = virtueTable[x][y - 1];
            virtueTable[x][y - 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            temp = value();
            virtueTable[x][y] = virtueTable[x][y - 1];
            if (temp < val)
            {
                val = temp;
            }
            virtueTable[x][y - 1] = a2;
            //Alpha��֦
            beta = qMin(beta, val);
            if (beta <= alpha)
                return beta;
        }
        else if (x == 0)
        {
            //qDebug()<<"BlueqMin "<<depth<<":1";
            a2 = virtueTable[x][y - 1];
            virtueTable[x][y - 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x][y - 1];
            virtueTable[x][y - 1] = a2;
        }
        else if (y == 0)
        {
            //qDebug()<<"BlueqMin "<<depth<<":1";
            a2 = virtueTable[x - 1][y];
            virtueTable[x - 1][y] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x - 1][y];
            virtueTable[x - 1][y] = a2;
        }
        return val;
    }

    if (x > 0 && y > 0) //�����Ϸ�
    {
        //������������Ѱ��value()���ֵ
        //qDebug()<<"BlueqMin "<<depth<<":3";
        //����������--------------------------------------------------------
        a2 = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�������ϵ��ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�ָ�����
        virtueTable[x][y] = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = a2;
        //Alpha��֦
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;

        //���������--------------------------------------------------------
        a2 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //������߼�ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�����岽
        if (temp < val)
        {
            val = temp;
        }
        temp = 0;
        //�ָ�����
        virtueTable[x][y] = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = a2;
        //Alpha��֦
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;

        //�������Ϸ�--------------------------------------------------------
        a2 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�����Ϸ���ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�����岽
        if (temp < val)
        {
            val = temp;
        }
        //�ָ�����
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a2;
        //Alpha��֦
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;
    }
    else if (x == 0)
    {
        //qDebug()<<"BlueqMin "<<depth<<":1";
        //�������Ϸ�--------------------------------------------------------
        a2 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�����Ϸ���ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�ָ�����
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a2;
    }
    else if (y == 0)
    {
        //qDebug()<<"BlueqMin "<<depth<<":1";
        //���������--------------------------------------------------------
        a2 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //������߼�ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�ָ�����
        virtueTable[x][y] = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = a2;
    }
    return val;
}

float Logic::redMax(int x, int y, int depth, float alpha, float beta)
{
    if (judgeResult() == 1)
    {
        return -infinity;
    }
    if (judgeResult() == 2)
    {
        return infinity;
    }

    //ʤ���Ѷ�
    //�ж���Ӯ��ӡ��Ϣ
    if (judgeResult() == 1)
    {
        return -infinity;
    }
    if (judgeResult() == 2)
    {
        return infinity;
    }

    int a;
    float val = 0, temp = 0;

    if (depth == 0) //�Ѿ��ﵽ�����
    {
        if (x < 4 && y < 4) //�����·�,����
        {
            //������������Ѱ��value()���ֵ
            //qDebug()<<"RedMax "<<depth<<":3";


            /*****************����**************/

            /*********��ģ����һ��*******/
            a = virtueTable[x + 1][y + 1];
            virtueTable[x + 1][y + 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();  //���㵱ǰ�Ĺ�ֵ����
            /*********��ģ����һ��*******/

            /******����ģ��״̬*******/
            virtueTable[x][y] = virtueTable[x + 1][y + 1];
            virtueTable[x + 1][y + 1] = a;
            /******����ģ��״̬*******/
            //Beta��֦
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;

            /*****************����**************/


            /*****************��***************/
            a = virtueTable[x + 1][y];
            virtueTable[x + 1][y] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            temp = value();
            virtueTable[x][y] = virtueTable[x + 1][y];
            if (temp > val)
            {
                val = temp;
            }
            virtueTable[x + 1][y] = a;
            //Beta��֦
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;
            /*****************��***************/


            /****************��***************/
            a = virtueTable[x][y + 1];
            virtueTable[x][y + 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            temp = value();
            virtueTable[x][y] = virtueTable[x][y + 1];
            if (temp > val)
            {
                val = temp;
            }
            virtueTable[x][y + 1] = a;
            //Beta��֦
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;
            /****************��***************/
        }
        else if (x == 4)
        {
            /****************��***************/
            //qDebug()<<"RedMax "<<depth<<":1";
            a = virtueTable[x][y + 1];
            virtueTable[x][y + 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x][y + 1];
            virtueTable[x][y + 1] = a;
            /****************��***************/
        }
        else if (y == 4)
        {
            /***********��*****************/
            //qDebug()<<"RedMax "<<depth<<":1";
            a = virtueTable[x + 1][y];
            virtueTable[x + 1][y] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x + 1][y];
            virtueTable[x + 1][y] = a;
            /***********��*****************/
        }
        return val; //��Ϊ���߽���ֻ��һ��·���ߣ�����˵ֱ�ӷ���val
    }



    //��û�дﵽ�����

    //��û�дﵽ�����
    if (x < 4 && y < 4) //�����·�
    {
        //������������Ѱ��value()���ֵ
        //qDebug()<<"RedMax "<<depth<<":3";
        //����������--------------------------------------------------------
        a = virtueTable[x + 1][y + 1];
        virtueTable[x + 1][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�������µ��ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady(); //��ȡblueprobability,��ģ��ı������
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, depth - 1, alpha, beta);
                }


        //�ָ�����
        virtueTable[x][y] = virtueTable[x + 1][y + 1];
        virtueTable[x + 1][y + 1] = a;
        //Beta��֦
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;

        //�������ұ�--------------------------------------------------------
        a = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //������߼�ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    temp += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�����岽
        if (temp > val)
        {
            val = temp;
        }
        temp = 0;
        //�ָ�����
        virtueTable[x][y] = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = a;
        //Beta��֦
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;

        //�������·�--------------------------------------------------------
        a = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�����Ϸ���ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    temp += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�����岽
        if (temp > val)
        {
            val = temp;
        }
        //�ָ�����
        virtueTable[x][y] = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = a;
        //Beta��֦
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;
    }
    else if (x == 4)
    {
        //qDebug()<<"RedMax "<<depth<<":1";
        //�������·�--------------------------------------------------------
        a = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //�����Ϸ���ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�ָ�����
        virtueTable[x][y] = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = a;
    }
    else if (y == 4)
    {
        //qDebug()<<"RedMax "<<depth<<":3";
        //�������ұ�--------------------------------------------------------
        a = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //������߼�ֵ
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //�ָ�����
        virtueTable[x][y] = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = a;
    }

    return val;
}

float Logic::value()
{
    float bluedistance = 0;
    float reddistance = 0;
    float blueThreaten = 0;
    float redThreaten = 0;
    float val = 0;

    blueReady(); //�õ�������˫���ļ�ֵ�Լ������ĸ�����вֵ
    for (int i = 0; i < SIZE; i++){
        if (qAbs(blueProbability[i]-0.00) > 0.005) //���Ӵ���
        {
            bluedistance += blueProbability[i] * blueValue[i]; //�ҷ�����ֵ
            blueThreaten+= blueProbability[i] * bluethreaten[i];
        }
    }
    // ��������вֵ



    redReady();
    for (int i = 0; i < SIZE; i++){
        if (qAbs(redProbability[i]-0.00) > 0.005){ //���Ӵ���
            reddistance += redProbability[i] * redValue[i]; //�췽����ֵ
            redThreaten += redProbability[i] * redthreaten[i];
        }
    }
    //�췽����вֵ

    val = (k1 * reddistance + k2 * bluedistance + k3 * blueThreaten + k4 * redThreaten); /**/


    return val;
}

\

void Logic::blueReady()
{
    QVector<int> bluedistancerate(SIZE,0);

    for (int i = 0; i < LINE; i++){
        blueProbability[i] = 0;
    }
    for (int i = 0; i < LINE; i++){
        for (int j = 0; j < 2; j++){
            blueprobabilityflag[i][j] = false;
        }
    }
    for (int i = 0; i < LINE; i++){
        redValue[i] = 0;
    }
    for (int i = 0; i < LINE; i++){
        blueValue[i] = 0;
    }
    for (int i = 0; i < LINE; i++){
        bluethreaten[i] = 0;
    }


    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            if(virtueTable[i][j]>0){
                blueValue[virtueTable[i][j]-1]=blueValueChart[i][j];
            }
            else if(virtueTable[i][j]<0){
                redValue[-virtueTable[i][j]-1]=redValueChart[i][j];
            }
        }
    }


    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            if(virtueTable[i][j]>0){
                if(i!=0 && j!=0){
                    blueprobabilityflag[virtueTable[i][j] - 1][0] = true;
                    int a = 0, b = 0, c = 0, temp;

                    if (virtueTable[i][j - 1] < 0){
                        a = redValue[-virtueTable[i][j-1] - 1];
                    }
                    if (virtueTable[i - 1][j - 1] < 0){
                        b = redValue[-virtueTable[i-1][j-1] - 1];
                    }
                    if (virtueTable[i - 1][j] < 0){
                        c = redValue[-virtueTable[i-1][j] - 1];
                    }

                    temp = qMax(a,b);
                    temp = qMax(temp, c);
                    bluethreaten[virtueTable[i][j] - 1] = temp;
                }

                else if (i==0&&j!=0) {
                    blueprobabilityflag[virtueTable[i][j] - 1][0] = true;
                    int temp=0;
                    if(virtueTable[i][j-1]<0){
                        temp=redValue[-virtueTable[i][j-1]-1];
                    }
                    bluethreaten[virtueTable[i][j] - 1]=temp;
                }

                else if(i!=0&&j==0){
                    blueprobabilityflag[virtueTable[i][j] - 1][0] = true;
                    int temp=0;
                    if(virtueTable[i-1][j]<0){
                        temp=blueValue[-virtueTable[i-1][j]-1];
                    }
                    bluethreaten[virtueTable[i][j] - 1]=temp;
                }

            }
        }
    }

    int distancerate = 0; //���ȼ�
    for (int i = 0; i < 5; i++)
    {
        for (int k = 0; k <= i; k++)
            if (virtueTable[i][k] > 0)
                bluedistancerate[distancerate++] = virtueTable[i][k];
        for (int k = i; k > 0; k--)
            if (virtueTable[k - 1][i] > 0)
                bluedistancerate[distancerate++] = virtueTable[k - 1][i];
    }


    int num=0, sum = 0;
    for (int i = 0; i < 6; i++)
    {
        if (bluedistancerate[i] > 0)
        {
            num = bluedistancerate[i] - 1;
            while (num > 0 && blueprobabilityflag[--num][0] == false && blueprobabilityflag[num][1] == false)
            {
                sum++;
                blueprobabilityflag[num][1] = true; //���ʱ�־λ��1��ʾ�Ѿ������������
            }
            num = bluedistancerate[i] - 1;
            while (num < 5 && blueprobabilityflag[++num][0] == false && blueprobabilityflag[num][1] == false)
            {
                sum++;
                blueprobabilityflag[num][1] = true; //���ʱ�־λ��1��ʾ�Ѿ������������
            }
            num = bluedistancerate[i] - 1;
            sum++;                           //��������ĸ���
            blueprobabilityflag[num][1] = true; //�����־λ��1

            blueProbability[num] = sum / 6.0f; //�洢����
            sum = 0;
        }
    }
}



void Logic::redReady()
{
    QVector<int> reddistancerate(SIZE,0);

    for (int i = 0; i < LINE; i++){
        redProbability[i] = 0;
    }
    for (int i = 0; i < LINE; i++){
        for (int j = 0; j < 2; j++){
            redprobabilityflag[i][j] = false;
        }
    }
    for (int i = 0; i < LINE; i++){
        redValue[i] = 0;
    }
    for (int i = 0; i < LINE; i++){
        blueValue[i] = 0;
    }
    for (int i = 0; i < LINE; i++){
        redthreaten[i] = 0;
    }


    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            if(virtueTable[i][j]>0){
                blueValue[virtueTable[i][j]-1]=blueValueChart[i][j];
            }
            else if(virtueTable[i][j]<0){
                redValue[-virtueTable[i][j]-1]=redValueChart[i][j];
            }
        }
    }


    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            if(virtueTable[i][j]<0){
                if(i!=4 && j!=4){
                    redprobabilityflag[-virtueTable[i][j] - 1][0] = true;
                    int a = 0, b = 0, c = 0, temp;

                    if (virtueTable[i][j + 1] > 0)
                        a = blueValue[virtueTable[i][j+1] - 1];
                    if (virtueTable[i + 1][j + 1] > 0)
                        b = blueValue[virtueTable[i+1][j+1] - 1];
                    if (virtueTable[i + 1][j] > 0)
                        c = blueValue[virtueTable[i+1][j] - 1];

                    temp = qMax(a,b);
                    temp = qMax(temp, c);
                    redthreaten[-virtueTable[i][j] - 1] = temp;
                }

                else if (i==4&&j!=4) {
                    redprobabilityflag[-virtueTable[i][j] - 1][0] = true;
                    int temp=0;
                    if(virtueTable[i][j+1]>0){
                        temp=blueValue[virtueTable[i][j+1]-1];
                    }
                    redthreaten[-virtueTable[i][j] - 1]=temp;
                }

                else if(i!=4&&j==4){
                    redprobabilityflag[-virtueTable[i][j] - 1][0] = true;
                    int temp=0;
                    if(virtueTable[i+1][j]>0){
                        temp=blueValue[virtueTable[i+1][j]-1];
                    }
                    redthreaten[-virtueTable[i][j] - 1]=temp;
                }

            }
        }
    }

    int distancerate = 0; //���ȼ�
    for (int i = LINE-1; i >= 0; i--){
        for (int k = LINE-1; k >= i; k--){
            if (virtueTable[i][k] < 0){
                reddistancerate[distancerate++] = virtueTable[i][k];
            }
        }
        for (int k = i; k < LINE-1; k++){
            if (virtueTable[k + 1][i] < 0){
                reddistancerate[distancerate++] = virtueTable[k + 1][i];
            }
        }
    }


    int num=0, sum = 0;
    for (int i = 0; i < SIZE; i++) {//6�������������ȼ�ռ�����
        if (reddistancerate[i] < 0)
        {
            num = -reddistancerate[i] - 1;
            while (num > 0 && redprobabilityflag[--num][0] == false && redprobabilityflag[num][1] == false)
            {
                sum++;
                redprobabilityflag[num][1] = true; //���ʱ�־λ��1��ʾ�Ѿ������������
            }
            num = -reddistancerate[i] - 1;
            while (num < LINE && redprobabilityflag[++num][0] == false && redprobabilityflag[num][1] == false)
            {
                sum++;
                redprobabilityflag[num][1] = true; //���ʱ�־λ��1��ʾ�Ѿ������������
            }
            num = -reddistancerate[i] - 1;
            sum++;                          //��������ĸ���
            redprobabilityflag[num][1] = true; //�����־λ��1

            redProbability[num] = sum / 6.0f; //�洢����
            sum = 0;
        }
    }


}



QPoint Logic::getPointToGo()
{
    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            if(virtueTable[i][j]==random){
                return blueWhereToGo(i,j,depth,-infinity,infinity);
            }
        }
    }
    int temp1 = 0; //���ڱ���С��randnum����ӽ���
    int temp2 = 7; //���ڱ������randnum����ӽ���
    int k1, l1;
    int k2, l2;

    for (int k = 0; k < 5; ++k){
        for (int l = 0; l < 5; ++l)
        {
            if (virtueTable[k][l] > temp1 && virtueTable[k][l] < random) //С��randnum����ӽ���
            {
                temp1 = virtueTable[k][l];
                k1 = k;
                l1 = l;
            }
            else if (virtueTable[k][l] > random && virtueTable[k][l] < temp2)
            {
                temp2 = virtueTable[k][l];
                k2 = k;
                l2 = l;
            }
        }
    }

    if (temp1 != 0 && temp2 == 7)
    {
        //�������̣����ڻ���
        int x = k1, y = l1;
        return blueWhereToGo(x, y, depth, -infinity, infinity);

    }
    else if (temp1 == 0 && temp2 != 7)
    {
        //�������̣����ڻ���

        int x = k2, y = l2;
        return blueWhereToGo(x, y, depth, -infinity, infinity);

    }
    else
    {
        float value1 = blueMin(k1, l1, depth, -infinity, infinity);
        float value2 = blueMin(k2, l2, depth, -infinity, infinity);
        if (value1 > value2) //����k2,l2��Ӧ������
        {

            int x = k2, y = l2;
            blueWhereToGo(x, y, depth, -infinity, infinity);

        }
        else
        {

            //����
            int x = k1, y = l1;
            return blueWhereToGo(x, y, depth, -infinity, infinity);

        }
    }
}



void Logic::setvirtueTable(const QVector<QVector<int> > &board)
{
    for (int i=0;i<LINE;i++) {
        for (int j=0;j<LINE;j++) {
            virtueTable[i][j]=board[i][j];
        }
    }
}

