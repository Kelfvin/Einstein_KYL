#include "logic.h"
#include <qmath.h>
#include <QDebug>


Logic::Logic()
{
    whoplay=0;
    redValueChart={
        {0,2,2,2,2},
        {2,4,4,4,5},
        {2,4,8,8,10},
        {2,4,8,16,20},
        {2,5,10,20,32}
    };
    blueValueChart={
        {32,20,10,5,2},
        {20,16,8,4,2},
        {10,8,8,4,2},
        {5,4,4,4,2},
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
        virtueTable[i].resize(LINE);
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

    int a1 = 0; //a鐢ㄤ簬淇濆瓨涓変釜鏂瑰悜鐨勬鍊
    float val = 0;
    float temp = 0;
    int flag = 0;
    int bestmoveX;
    int bestmoveY;

    if (x > 0 && y > 0 ){ //鏈夊乏涓婃柟
        if (specialDeal(x,y)) {
            return QPoint(x,y);
        }
        a1 = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻宸︿笂鐐逛环鍊

        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }


        //鏈€浼樻姝ワ紝鏉€寰楀鏂逛竴涓篃娌℃湁浜
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
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = a1;
        //钃濇璧板乏涓-------------------------------------------------------


        //钃濇璧板乏杈-------------------------------------------------------
        a1 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻宸﹁竟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }


        //鏈€浼樻姝
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
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = a1;
        //钃濇璧板乏杈-------------------------------------------------------


        //钃濇璧颁笂鏂-------------------------------------------------------
        a1 = virtueTable[x][y - 1];
        virtueTable[x][y -1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻涓婃柟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    redReady();

                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, depth, alpha, beta);
                    flag++;
                }


        if (flag == 0)
        {
            bestmoveX = x;
            bestmoveY = y - 1;
            x = bestmoveX;
            y = bestmoveY;
            return QPoint(x,y);
        }
        flag = 0;
        //鏈€浼樻姝
        if (temp < val)
        {
            val = temp;
            bestmoveX = x;
            bestmoveY = y - 1;
        }
        //钃濇璧颁笂鏂-------------------------------------------------------


        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a1;
        //鏈€缁堟渶浼樻姝
        //qDebug()<<num1;
        /********杩涜绉诲姩*********/
        x = bestmoveX;
        y = bestmoveY;
        return QPoint(x,y);
        /********杩涜绉诲姩*********/
    }
    else if (x == 0) //宸﹁竟涓哄锛岀敤涓嶇潃浼板€硷紝鍥犱负鍙兘璧板晩
    {
        //qDebug()<<"BlueWhereToGo "<<depth<<":1";
        y = y - 1;
        return QPoint(x,y);
    }
    else if (y == 0) //涓婃柟涓哄锛岀敤涓嶇潃浼板€硷紝鍥犱负鍙兘璧板晩
    {
        //qDebug()<<"BlueWhereToGo "<<depth<<":1";
        x = x - 1;
        return QPoint(x,y);
    }


}



bool Logic::specialDeal(int& x,int& y)
{
    //0731 鍓瑙掔嚎鍙栨秷鎺
    int num1 = 0;
    int bestmoveX;
    int bestmoveY;
    if (x > 0 && y > 0 && x < 4 && y < 4) //鏈夊乏涓婃柟
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

        //0801瀹氬紡澶勭悊
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

            //0801瀹氬紡澶勭悊
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

        //0731 鍓瑙掔嚎鍙栨秷鎺夊畾寮忓鐞
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
        //0801 瀹氬紡澶勭悊
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


float Logic::blueMin(int x, int y, int depth, float alpha, float beta)
{

    //鑳滆礋宸插畾
    //鍒ゆ柇杈撹耽鎵撳嵃淇℃伅
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
        if (x > 0 && y > 0) //鏈夊乏涓婃柟
        {
            //閬嶅巻涓変釜鏂瑰悜瀵绘眰value()鏈€澶у€
            //qDebug()<<"BlueMin "<<depth<<":3";
            //宸︿笂
            a2 = virtueTable[x - 1][y - 1];
            virtueTable[x - 1][y - 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x - 1][y - 1];
            virtueTable[x - 1][y - 1] = a2;
            //Alpha鍓灊
            beta = qMin(beta, val);
            if (beta <= alpha)
                return beta;

            //宸﹁竟
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
            //Alpha鍓灊
            beta = qMin(beta, val);
            if (beta <= alpha)
                return beta;

            //涓婃柟
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
            //Alpha鍓灊
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

    if (x > 0 && y > 0) //鏈夊乏涓婃柟
    {
        //閬嶅巻涓変釜鏂瑰悜瀵绘眰value()鏈€澶у€
        //qDebug()<<"BlueqMin "<<depth<<":3";
        //钃濇璧板乏涓-------------------------------------------------------
        a2 = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻宸︿笂鐐逛环鍊
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x - 1][y - 1];
        virtueTable[x - 1][y - 1] = a2;
        //Alpha鍓灊
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;

        //钃濇璧板乏杈-------------------------------------------------------
        a2 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻宸﹁竟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鏈€浼樻姝
        if (temp < val)
        {
            val = temp;
        }
        temp = 0;
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = a2;
        //Alpha鍓灊
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;

        //钃濇璧颁笂鏂-------------------------------------------------------
        a2 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻涓婃柟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    temp += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鏈€浼樻姝
        if (temp < val)
        {
            val = temp;
        }
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a2;
        //Alpha鍓灊
        beta = qMin(beta, val);
        if (beta <= alpha)
            return beta;
    }
    else if (x == 0)
    {
        //qDebug()<<"BlueqMin "<<depth<<":1";
        //钃濇璧颁笂鏂-------------------------------------------------------
        a2 = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻涓婃柟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x][y - 1];
        virtueTable[x][y - 1] = a2;
    }
    else if (y == 0)
    {
        //qDebug()<<"BlueqMin "<<depth<<":1";
        //钃濇璧板乏杈-------------------------------------------------------
        a2 = virtueTable[x - 1][y];
        virtueTable[x - 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻宸﹁竟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] < 0)
                {
                    //BlueProbability ();
                    redReady();
                    val += redProbability[-virtueTable[i][j] - 1] * redMax(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鎭㈠妫嬬洏
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

    //鑳滆礋宸插畾
    //鍒ゆ柇杈撹耽鎵撳嵃淇℃伅
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

    if (depth == 0) //宸茬粡杈惧埌鏈€娣卞害
    {
        if (x < 4 && y < 4) //鏈夊彸涓嬫柟,杩欐牱
        {
            //閬嶅巻涓変釜鏂瑰悜瀵绘眰value()鏈€澶у€
            //qDebug()<<"RedMax "<<depth<<":3";


            /*****************鍙充笅**************/

            /*********鍏堟ā鎷熻蛋涓€姝******/
            a = virtueTable[x + 1][y + 1];
            virtueTable[x + 1][y + 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();  //璁＄畻褰撳墠鐨勪及鍊煎嚱鏁
            /*********鍏堟ā鎷熻蛋涓€姝******/

            /******鎾ゅ嚭妯℃嫙鐘舵€******/
            virtueTable[x][y] = virtueTable[x + 1][y + 1];
            virtueTable[x + 1][y + 1] = a;
            /******鎾ゅ嚭妯℃嫙鐘舵€******/
            //Beta鍓灊
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;

            /*****************鍙充笅**************/


            /*****************鍙**************/
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
            //Beta鍓灊
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;
            /*****************鍙**************/


            /****************涓**************/
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
            //Beta鍓灊
            alpha = qMax(alpha, val);
            if (beta <= alpha)
                return alpha;
            /****************涓**************/
        }
        else if (x == 4)
        {
            /****************涓**************/
            //qDebug()<<"RedMax "<<depth<<":1";
            a = virtueTable[x][y + 1];
            virtueTable[x][y + 1] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x][y + 1];
            virtueTable[x][y + 1] = a;
            /****************涓**************/
        }
        else if (y == 4)
        {
            /***********鍙****************/
            //qDebug()<<"RedMax "<<depth<<":1";
            a = virtueTable[x + 1][y];
            virtueTable[x + 1][y] = virtueTable[x][y];
            virtueTable[x][y] = 0;
            val = value();
            virtueTable[x][y] = virtueTable[x + 1][y];
            virtueTable[x + 1][y] = a;
            /***********鍙****************/
        }
        return val; //鍥犱负鍒拌竟鐣屼笂鍙湁涓€鏉¤矾鍙蛋锛屾墍浠ヨ鐩存帴杩斿洖val
    }



    //杩樻病鏈夎揪鍒版渶娣卞害

    //杩樻病鏈夎揪鍒版渶娣卞害
    if (x < 4 && y < 4) //鏈夊彸涓嬫柟
    {
        //閬嶅巻涓変釜鏂瑰悜瀵绘眰value()鏈€澶у€
        //qDebug()<<"RedMax "<<depth<<":3";
        //绾㈡璧板彸涓-------------------------------------------------------
        a = virtueTable[x + 1][y + 1];
        virtueTable[x + 1][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻鍙充笅鐐逛环鍊
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady(); //鑾峰彇blueprobability,骞舵ā鎷熸敼鍙樼殑妫嬬洏
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, depth - 1, alpha, beta);
                }


        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x + 1][y + 1];
        virtueTable[x + 1][y + 1] = a;
        //Beta鍓灊
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;

        //绾㈡璧板彸杈-------------------------------------------------------
        a = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻宸﹁竟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    temp += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鏈€浼樻姝
        if (temp > val)
        {
            val = temp;
        }
        temp = 0;
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = a;
        //Beta鍓灊
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;

        //绾㈡璧颁笅鏂-------------------------------------------------------
        a = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻涓婃柟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    temp += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鏈€浼樻姝
        if (temp > val)
        {
            val = temp;
        }
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = a;
        //Beta鍓灊
        alpha = qMax(alpha, val);
        if (beta <= alpha)
            return alpha;
    }
    else if (x == 4)
    {
        //qDebug()<<"RedMax "<<depth<<":1";
        //绾㈡璧颁笅鏂-------------------------------------------------------
        a = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻涓婃柟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鎭㈠妫嬬洏
        virtueTable[x][y] = virtueTable[x][y + 1];
        virtueTable[x][y + 1] = a;
    }
    else if (y == 4)
    {
        //qDebug()<<"RedMax "<<depth<<":3";
        //绾㈡璧板彸杈-------------------------------------------------------
        a = virtueTable[x + 1][y];
        virtueTable[x + 1][y] = virtueTable[x][y];
        virtueTable[x][y] = 0;
        //璁＄畻宸﹁竟浠峰€
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (virtueTable[i][j] > 0)
                {
                    blueReady();
                    //RedProbability ();
                    val += blueProbability[virtueTable[i][j] - 1] * blueMin(i, j, /*num,*/ depth - 1, alpha, beta);
                }
        //鎭㈠妫嬬洏
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

    blueReady(); //寰楀埌浜嗚摑绾㈠弻鏂圭殑浠峰€间互鍙婅摑鏂圭殑涓綋濞佽儊鍊
    for (int i = 0; i < SIZE; i++){
        if (qAbs(blueProbability[i]-0.00f) > 0.005) //妫嬪瓙瀛樺湪
        {
            bluedistance += blueProbability[i] * blueValue[i]; //鎴戞柟杩涙敾鍊
            blueThreaten+= blueProbability[i] * bluethreaten[i];
        }
    }
    // 钃濇柟鐨勫▉鑳佸€



    redReady();
    for (int i = 0; i < SIZE; i++){
        if (qAbs(redProbability[i]-0.00f) > 0.005){ //妫嬪瓙瀛樺湪
            reddistance += redProbability[i] * redValue[i]; //绾㈡柟杩涙敾鍊
            redThreaten += redProbability[i] * redthreaten[i];
        }
    }
    //绾㈡柟鐨勫▉鑳佸€

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

    int distancerate = 0; //浼樺厛绾
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
                blueprobabilityflag[num][1] = true; //姒傜巼鏍囧織浣嶇疆1琛ㄧず宸茬粡璁＄畻杩囨鐜囦簡
            }
            num = bluedistancerate[i] - 1;
            while (num < 5 && blueprobabilityflag[++num][0] == false && blueprobabilityflag[num][1] == false)
            {
                sum++;
                blueprobabilityflag[num][1] = true; //姒傜巼鏍囧織浣嶇疆1琛ㄧず宸茬粡璁＄畻杩囨鐜囦簡
            }
            num = bluedistancerate[i] - 1;
            sum++;                           //鍔犱笂鑷韩鐨勬鐜
            blueprobabilityflag[num][1] = true; //鑷韩鏍囧織浣嶇疆1

            blueProbability[num] = sum / 6.0f; //瀛樺偍姒傜巼
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

    int distancerate = 0; //浼樺厛绾
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
    for (int i = 0; i < SIZE; i++) {//6涓瀛愪緷鐓т紭鍏堢骇鍗犻姒傜巼
        if (reddistancerate[i] < 0)
        {
            num = -reddistancerate[i] - 1;
            while (num > 0 && redprobabilityflag[--num][0] == false && redprobabilityflag[num][1] == false)
            {
                sum++;
                redprobabilityflag[num][1] = true; //姒傜巼鏍囧織浣嶇疆1琛ㄧず宸茬粡璁＄畻杩囨鐜囦簡
            }
            num = -reddistancerate[i] - 1;
            while (num < LINE && redprobabilityflag[++num][0] == false && redprobabilityflag[num][1] == false)
            {
                sum++;
                redprobabilityflag[num][1] = true; //姒傜巼鏍囧織浣嶇疆1琛ㄧず宸茬粡璁＄畻杩囨鐜囦簡
            }
            num = -reddistancerate[i] - 1;
            sum++;                          //鍔犱笂鑷韩鐨勬鐜
            redprobabilityflag[num][1] = true; //鑷韩鏍囧織浣嶇疆1

            redProbability[num] = sum / 6.0f; //瀛樺偍姒傜巼
            sum = 0;
        }
    }


}



QVector<QPoint> Logic::getPointToGo()
{
    if(direction==1){
        QVector<QPoint> returnData(2);
        for (int i=0;i<LINE;i++) {
            for (int j=0;j<LINE;j++) {
                if(virtueTable[i][j]==random){
                    returnData[0] = QPoint(i,j);
                    returnData[1] = blueWhereToGo(i,j,depth,-infinity,infinity);
                    return returnData;
                }
            }
        }
        int temp1 = 0; //鐢ㄤ簬淇濆瓨灏忎簬randnum涓旀渶鎺ヨ繎鐨
        int temp2 = 7; //鐢ㄤ簬淇濆瓨澶т簬randnum涓旀渶鎺ヨ繎鐨
        int k1, l1;
        int k2, l2;

        for (int k = 0; k < 5; ++k){
            for (int l = 0; l < 5; ++l)
            {
                if (virtueTable[k][l] > temp1 && virtueTable[k][l] < random) //灏忎簬randnum涓旀渶鎺ヨ繎鐨
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
            //澶囦唤妫嬬洏锛岀敤浜庢倲妫
            returnData[0] = QPoint(k1,l1);
            returnData[1] = blueWhereToGo(k1,l1,depth,-infinity,infinity);
            return returnData;
        }
        else if (temp1 == 0 && temp2 != 7)
        {
            //澶囦唤妫嬬洏锛岀敤浜庢倲妫
            returnData[0] = QPoint(k2,l2);
            returnData[1] = blueWhereToGo(k2,l2,depth,-infinity,infinity);
            return returnData;
        }
        else
        {
            float value1 = blueMin(k1, l1, depth, -infinity, infinity);
            float value2 = blueMin(k2, l2, depth, -infinity, infinity);
            if (value1 > value2) //璇ヨ蛋k2,l2瀵瑰簲鐨勬瀛
            {
                returnData[0] = QPoint(k2,l2);
                returnData[1] = blueWhereToGo(k2,l2,depth,-infinity,infinity);
                return returnData;
            }
            else
            {
                //璧版
                returnData[0] = QPoint(k1,l1);
                returnData[1] = blueWhereToGo(k1,l1,depth,-infinity,infinity);
                return returnData;
            }
        }
    }
    else if(direction==-1){
        QVector<QPoint> returnData(2);
        for (int i=0;i<LINE;i++) {
            for (int j=0;j<LINE;j++) {
                if(virtueTable[i][j]==random){
                    returnData[0] = QPoint(4-j,4-i);
                    returnData[1] = blueWhereToGo(i,j,depth,-infinity,infinity);
                    int temp=4-returnData[1].x();
                    returnData[1].setX(4-returnData[1].y());
                    returnData[1].setY(temp);
                    return returnData;
                }
            }
        }
        int temp1 = 0; //鐢ㄤ簬淇濆瓨灏忎簬randnum涓旀渶鎺ヨ繎鐨
        int temp2 = 7; //鐢ㄤ簬淇濆瓨澶т簬randnum涓旀渶鎺ヨ繎鐨
        int k1, l1;
        int k2, l2;

        for (int k = 0; k < 5; ++k){
            for (int l = 0; l < 5; ++l)
            {
                if (virtueTable[k][l] > temp1 && virtueTable[k][l] < random) //灏忎簬randnum涓旀渶鎺ヨ繎鐨
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
            //澶囦唤妫嬬洏锛岀敤浜庢倲妫
            returnData[0] = QPoint(4-l1,4-k1);
            returnData[1] = blueWhereToGo(k1,l1,depth,-infinity,infinity);
            int temp=4-returnData[1].x();
            returnData[1].setX(4-returnData[1].y());
            returnData[1].setY(temp);
            return returnData;
        }
        else if (temp1 == 0 && temp2 != 7)
        {
            //澶囦唤妫嬬洏锛岀敤浜庢倲妫
            returnData[0] = QPoint(4-l2,4-k2);
            returnData[1] = blueWhereToGo(k2,l2,depth,-infinity,infinity);
            int temp=4-returnData[1].x();
            returnData[1].setX(4-returnData[1].y());
            returnData[1].setY(temp);
            return returnData;
        }
        else
        {
            float value1 = blueMin(k1, l1, depth, -infinity, infinity);
            float value2 = blueMin(k2, l2, depth, -infinity, infinity);
            if (value1 > value2) //璇ヨ蛋k2,l2瀵瑰簲鐨勬瀛
            {
                returnData[0] = QPoint(4-l2,4-k2);
                returnData[1] = blueWhereToGo(k2,l2,depth,-infinity,infinity);
                int temp=4-returnData[1].x();
                returnData[1].setX(4-returnData[1].y());
                returnData[1].setY(temp);
                return returnData;
            }
            else
            {
                //璧版
                returnData[0] = QPoint(4-l1,4-k1);
                returnData[1] = blueWhereToGo(k1,l1,depth,-infinity,infinity);
                int temp=4-returnData[1].x();
                returnData[1].setX(4-returnData[1].y());
                returnData[1].setY(temp);
                return returnData;
            }
        }
    }

}



void Logic::setvirtueTable(const QVector<QVector<int> > &board)
{
    if(direction==1){
        for (int i=0;i<LINE;i++) {
            for (int j=0;j<LINE;j++) {
                virtueTable[i][j]=board[i][j];
            }
        }
    }
    else if (direction==-1) {
        for (int i=0;i<LINE;i++) {
            for (int j=0;j<LINE;j++) {
                virtueTable[i][j]=-board[4-j][4-i];
            }
        }


    }


}

void Logic::setRand(int rand){
    this->random=rand;
}//鑾峰彇楠板瓙鏁
void Logic::setDepth(int depth){
    this->depth=depth;
}
void Logic::setSente(int sente){
    this->sente=sente;
}
void Logic::setourColor(int ourColor){
    this->direction=ourColor;
}

