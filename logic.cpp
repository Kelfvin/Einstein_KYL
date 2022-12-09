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

    int a1 = 0; //a用于保存三个方向的棋值
    float val = 0;
    float temp = 0;
    int flag = 0;
    int num = 0;
    int bestmoveX;
    int bestmoveY;
    if (x > 0 && y > 0){ //有左上方
        if (specialDeal(x,y)) {
            return QPoint(x,y);
        }
        a1 = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算左上点价值

        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }

        qDebug() << val;
        //最优棋步，杀得对方一个也没有了
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
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = a1;
        //蓝棋走左上--------------------------------------------------------


        //蓝棋走左边--------------------------------------------------------
        a1 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算左边价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }
        qDebug() << temp;
        //最优棋步
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
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = a1;
        //蓝棋走左边--------------------------------------------------------


        //蓝棋走上方--------------------------------------------------------
        a1 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算上方价值
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
        //最优棋步
        if (temp < val)
        {
            val = temp;
            bestmoveX = x;
            bestmoveY = y - 1;
        }
        //蓝棋走上方--------------------------------------------------------


        //恢复棋盘
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a1;
        //最终最优棋步
        //qDebug()<<num1;
        /********进行移动*********/
        x = bestmoveX;
        y = bestmoveY;
        return QPoint(x,y);
        /********进行移动*********/
    }
    else if (x == 0) //左边为墙，用不着估值，因为只能走啊
    {
        //qDebug()<<"BlueWhereToGo "<<depth<<":1";
        y = y - 1;
        return QPoint(x,y);
    }
    else if (y == 0) //上方为墙，用不着估值，因为只能走啊
    {
        //qDebug()<<"BlueWhereToGo "<<depth<<":1";
        x = x - 1;
        return QPoint(x,y);
    }



}



bool Logic::specialDeal(int& x,int& y)
{
    //0731 副对角线取消掉
    int num1 = 0;
    int bestmoveX;
    int bestmoveY;
    if (x > 0 && y > 0) //有左上方
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

        //0801定式处理
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

            //0801定式处理
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

        //0731 副对角线取消掉定式处理
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
        //0801 定式处理
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

    //胜负已定
    //判断输赢打印信息
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
        if (x > 0 && y > 0) //有左上方
        {
            //遍历三个方向寻求value()最大值
            //qDebug()<<"BlueMin "<<depth<<":3";
            //左上
            a2 = virtueTable[x - 1][y - 1];
            virtueTable[x - 1][y - 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x - 1][y - 1];
            virtueTable[x - 1][y - 1] = a2;
            //Alpha剪枝
            beta = qMin(beta, val);
            if (beta <= alpha)
                return beta;

            //左边
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
            //Alpha剪枝
            beta = qMin(beta, val);
            if (beta <= alpha)
                return beta;

            //上方
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
            //Alpha剪枝
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

    if (x > 0 && y > 0) //有左上方
    {
        //遍历三个方向寻求value()最大值
        //qDebug()<<"BlueqMin "<<depth<<":3";
        //蓝棋走左上--------------------------------------------------------
        a2 = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算左上点价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = a2;
        //Alpha剪枝
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;

        //蓝棋走左边--------------------------------------------------------
        a2 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算左边价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //最优棋步
        if (temp < val)
        {
            val = temp;
        }
        temp = 0;
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = a2;
        //Alpha剪枝
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;

        //蓝棋走上方--------------------------------------------------------
        a2 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算上方价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //最优棋步
        if (temp < val)
        {
            val = temp;
        }
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a2;
        //Alpha剪枝
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;
    }
    else if (x == 0)
    {
        //qDebug()<<"BlueqMin "<<depth<<":1";
        //蓝棋走上方--------------------------------------------------------
        a2 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算上方价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a2;
    }
    else if (y == 0)
    {
        //qDebug()<<"BlueqMin "<<depth<<":1";
        //蓝棋走左边--------------------------------------------------------
        a2 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算左边价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //恢复棋盘
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

    //胜负已定
    //判断输赢打印信息
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

    if (depth == 0) //已经达到最深度
    {
        if (x < 4 && y < 4) //有右下方,这样
        {
            //遍历三个方向寻求value()最大值
            //qDebug()<<"RedMax "<<depth<<":3";


            /*****************右下**************/

            /*********先模拟走一步*******/
            a = virtueTable[x + 1][y + 1];
            virtueTable[x + 1][y + 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();  //计算当前的估值函数
            /*********先模拟走一步*******/

            /******撤出模拟状态*******/
            virtueTable[x][y] = virtueTable[x + 1][y + 1];
            virtueTable[x + 1][y + 1] = a;
            /******撤出模拟状态*******/
            //Beta剪枝
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;

            /*****************右下**************/


            /*****************右***************/
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
            //Beta剪枝
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;
            /*****************右***************/


            /****************下***************/
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
            //Beta剪枝
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;
            /****************下***************/
        }
        else if (x == 4)
        {
            /****************下***************/
            //qDebug()<<"RedMax "<<depth<<":1";
            a = virtueTable[x][y + 1];
            virtueTable[x][y + 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x][y + 1];
            virtueTable[x][y + 1] = a;
            /****************下***************/
        }
        else if (y == 4)
        {
            /***********右*****************/
            //qDebug()<<"RedMax "<<depth<<":1";
            a = virtueTable[x + 1][y];
            virtueTable[x + 1][y] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x + 1][y];
            virtueTable[x + 1][y] = a;
            /***********右*****************/
        }
        return val; //因为到边界上只有一条路可走，所以说直接返回val
    }



    //还没有达到最深度

    //还没有达到最深度
    if (x < 4 && y < 4) //有右下方
    {
        //遍历三个方向寻求value()最大值
        //qDebug()<<"RedMax "<<depth<<":3";
        //红棋走右下--------------------------------------------------------
        a = virtueTable[x + 1][y + 1];
        virtueTable[x + 1][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算右下点价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady(); //获取blueprobability,并模拟改变的棋盘
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, depth - 1, alpha, beta);
                }


        //恢复棋盘
        virtueTable[x][y] = virtueTable[x + 1][y + 1];
        virtueTable[x + 1][y + 1] = a;
        //Beta剪枝
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;

        //红棋走右边--------------------------------------------------------
        a = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算左边价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    temp += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //最优棋步
        if (temp > val)
        {
            val = temp;
        }
        temp = 0;
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = a;
        //Beta剪枝
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;

        //红棋走下方--------------------------------------------------------
        a = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算上方价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    temp += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //最优棋步
        if (temp > val)
        {
            val = temp;
        }
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = a;
        //Beta剪枝
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;
    }
    else if (x == 4)
    {
        //qDebug()<<"RedMax "<<depth<<":1";
        //红棋走下方--------------------------------------------------------
        a = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算上方价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //恢复棋盘
        virtueTable[x][y] = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = a;
    }
    else if (y == 4)
    {
        //qDebug()<<"RedMax "<<depth<<":3";
        //红棋走右边--------------------------------------------------------
        a = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //计算左边价值
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //恢复棋盘
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

    blueReady(); //得到了蓝红双方的价值以及蓝方的个体威胁值
    for (int i = 0; i < SIZE; i++){
        if (qAbs(blueProbability[i]-0.00) > 0.005) //棋子存在
        {
            bluedistance += blueProbability[i] * blueValue[i]; //我方进攻值
            blueThreaten+= blueProbability[i] * bluethreaten[i];
        }
    }
    // 蓝方的威胁值



    redReady();
    for (int i = 0; i < SIZE; i++){
        if (qAbs(redProbability[i]-0.00) > 0.005){ //棋子存在
            reddistance += redProbability[i] * redValue[i]; //红方进攻值
            redThreaten += redProbability[i] * redthreaten[i];
        }
    }
    //红方的威胁值

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

    int distancerate = 0; //优先级
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
                blueprobabilityflag[num][1] = true; //概率标志位置1表示已经计算过概率了
            }
            num = bluedistancerate[i] - 1;
            while (num < 5 && blueprobabilityflag[++num][0] == false && blueprobabilityflag[num][1] == false)
            {
                sum++;
                blueprobabilityflag[num][1] = true; //概率标志位置1表示已经计算过概率了
            }
            num = bluedistancerate[i] - 1;
            sum++;                           //加上自身的概率
            blueprobabilityflag[num][1] = true; //自身标志位置1

            blueProbability[num] = sum / 6.0f; //存储概率
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

    int distancerate = 0; //优先级
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
    for (int i = 0; i < SIZE; i++) {//6个棋子依照优先级占领概率
        if (reddistancerate[i] < 0)
        {
            num = -reddistancerate[i] - 1;
            while (num > 0 && redprobabilityflag[--num][0] == false && redprobabilityflag[num][1] == false)
            {
                sum++;
                redprobabilityflag[num][1] = true; //概率标志位置1表示已经计算过概率了
            }
            num = -reddistancerate[i] - 1;
            while (num < LINE && redprobabilityflag[++num][0] == false && redprobabilityflag[num][1] == false)
            {
                sum++;
                redprobabilityflag[num][1] = true; //概率标志位置1表示已经计算过概率了
            }
            num = -reddistancerate[i] - 1;
            sum++;                          //加上自身的概率
            redprobabilityflag[num][1] = true; //自身标志位置1

            redProbability[num] = sum / 6.0f; //存储概率
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
    int temp1 = 0; //用于保存小于randnum且最接近的
    int temp2 = 7; //用于保存大于randnum且最接近的
    int k1, l1;
    int k2, l2;

    for (int k = 0; k < 5; ++k){
        for (int l = 0; l < 5; ++l)
        {
            if (virtueTable[k][l] > temp1 && virtueTable[k][l] < random) //小于randnum且最接近的
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
        //备份棋盘，用于悔棋
        int x = k1, y = l1;
        return blueWhereToGo(x, y, depth, -infinity, infinity);

    }
    else if (temp1 == 0 && temp2 != 7)
    {
        //备份棋盘，用于悔棋

        int x = k2, y = l2;
        return blueWhereToGo(x, y, depth, -infinity, infinity);

    }
    else
    {
        float value1 = blueMin(k1, l1, depth, -infinity, infinity);
        float value2 = blueMin(k2, l2, depth, -infinity, infinity);
        if (value1 > value2) //该走k2,l2对应的棋子
        {

            int x = k2, y = l2;
            blueWhereToGo(x, y, depth, -infinity, infinity);

        }
        else
        {

            //走棋
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

