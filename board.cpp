#include "board.h"
#include <QVector>
#include <QTime>
#include <chrono>
#include <utility>
#include <QSet>
#include <QDebug>


Board::Board()
{
    ourColor = 1;
    backupIndex = -1;


    initDefaultChessSet();
}

bool Board::setChess(int x, int y, int value)
{

    if(x<0||x>4||y<0||y>4){
        return false;
    }

    removeFromChessMap(board[x][y]);

    addToChessMap(value,x,y);

    board[x][y] = value;
    return true;

}

bool Board::moveChess(int startx, int starty, int endx, int endy)
{

    if(startx == endx && starty == endy){
        return  false;
    }

    int chess = board[startx][starty];
    if(chess == 0){
        return false;
    }


    // 自己只能移动自己的棋子
    if(nowPlayer * chess <0){
        return false;
    }


    // 棋子的移动终点是否合法 ps: 这里先写人人对战，鼠标的点击位置不会超过棋盘，后期要改
    // 蓝方
    if(nowPlayer>0){
        if(endx>startx || endy >starty){
            return false;
        }
        if(endx<startx-1||endy<starty-1){
            return false;
        }
    }
    // 红方
    else{
        if(endx<startx || endy < starty){
            return false;
        }
        if(endx>startx+1||endy>starty+1){
            return false;
        }
    }

    backup();

    int endGrid = board[endx][endy];
    if(endGrid!= 0){
        removeFromChessMap(endGrid);
    }
    addToChessMap(chess,startx,starty);

    board[endx][endy] = board[startx][starty];
    board[startx][starty] = 0;

    nowPlayer = -nowPlayer;


    return true;
}

bool Board::undo()
{
    if(backupIndex <= 0){
        return false;
    }

    else{

        board = backupBoards[--backupIndex];
        nowPlayer = -nowPlayer;
        return true;
    }


}

bool Board::cancelUndo()
{
    if(backupIndex == backupBoards.length()-1){
        return false;
    }

   else{
       backupIndex++;
       return true;
   }
}

void Board::backup()
{
    if(backupIndex+1==backupBoards.length()){
        backupBoards.append(board);
    }

    else{
        backupBoards[++backupIndex]=board;
    }
}

bool Board::checkBoardSet()
{
    addToChessMap(board[0][0],0,0);
    addToChessMap(board[0][1],0,1);
    addToChessMap(board[0][2],0,2);
    addToChessMap(board[1][0],1,0);
    addToChessMap(board[1][1],1,1);
    addToChessMap(board[2][0],2,0);

    addToChessMap(board[4][2],4,2);
    addToChessMap(board[3][3],3,3);
    addToChessMap(board[4][3],4,3);
    addToChessMap(board[2][4],2,4);
    addToChessMap(board[3][4],3,4);
    addToChessMap(board[4][4],4,4);

    qDebug()<<redChessMap.size() << endl;
    qDebug()<<blueChessMap.size() << endl;
    if(redChessMap.size()==6&&blueChessMap.size()==6){
        return true;
    }

    else{
        return false;
    }
}

int Board::getChess(int x, int y)
{
    return board[x][y];
}

bool Board::canUndo()
{
    if(backupIndex <= 0){
        return false;
    }

    return true;
}

int Board::getNowPlayer()
{
    return  nowPlayer;
}

int Board::getOurColor()
{
    return ourColor;
}

bool Board::setDice(int dice)
{
    if(dice<1||dice>6){
        return false;
    }

    this->dice = dice;
    return true;
}

void Board::setOurColor(int color)
{

    ourColor = color;
}

void Board::setNowPlayer(int color)
{
    nowPlayer = color;
}

const QVector<QVector<int> > &Board::getBoard()
{
    return board;
}

int Board::getSente()
{
    return sente;
}

void Board::setSente(int color)
{
    sente = color;
}

int Board::getDice()
{
    return dice;
}

QString Board::getNowPlayerStr()
{
    return nowPlayer >0? "蓝方":"红方";
}



int Board::checkWin()
{
    if(board[0][0] > 0 || redChessMap.count()==0){
        return 1;
    }

    else if(board[4][4]<0 || blueChessMap.count() == 0){
        return -1;
    }

    else{
        return 0;
    }
}

void Board::addToChessMap(int value, int x, int y)
{
    if(value>0){
        blueChessMap[value].setX(x);
        blueChessMap[value].setY(y);
    }else{
        redChessMap[value].setX(x);
        redChessMap[value].setY(y);
    }
}



void Board::initDefaultChessSet()
{

    board = {
        {-7,-7,-7, 0, 0},
        {-7,-7, 0, 0, 0},
        {-7, 0, 0, 0, 7},
        { 0, 0, 0, 7, 7},
        { 0, 0, 7, 7, 7}
    };

    QVector<int> arr = {1,2,3,4,5,6};
    unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count();
    std::shuffle(arr.begin(),arr.end(),std::default_random_engine(seed));

    // 蓝方棋子
    board[4][2] = arr[0];
    board[3][3] = arr[1];
    board[4][3] = arr[2];
    board[2][4] = arr[3];
    board[3][4] = arr[4];
    board[4][4] = arr[5];

    // 红方棋子
    seed = std::chrono::system_clock::now ().time_since_epoch ().count();
    std::shuffle(arr.begin(),arr.end(),std::default_random_engine(seed));

    board[0][0] = -arr[0];
    board[0][1] = -arr[1];
    board[0][2] = -arr[2];
    board[1][0] = -arr[3];
    board[1][1] = -arr[4];
    board[2][0] = -arr[5];

}

void Board::removeFromChessMap(int value)
{
    if(value>0){
        blueChessMap.remove(value);
    }else{
        redChessMap.remove(value);
    }
}



