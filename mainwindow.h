#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "board.h"
#include "logic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void onBoardTap(int x , int y); // 点击棋盘事件
    void showMsg(QString str);  // 展示提示框
    void initSearchDeepCombBox();   // 初始化搜索深度的下拉框
    void initSetOurColorCombBox();  // 初始化我方队伍颜色的下拉框
    void initSetDiceCombBox();  // 初始化指定 骰子数的下拉框
    void initSetSenteCombBox(); // 初始化先手的下拉框
    void drawChessNumber(int x,int y);  // 画棋盘上的一个棋子的数值
    void drawBoardNumber(); // 画整个棋盘上所有的棋子的编号
    void drawBoardColor();  // 画棋盘上的所有的棋子的颜色
    void drawBoardLine();   // 画棋盘的线
    void letAIDo(); // 交给AI来行走棋子，并显示在棋盘上
    void moveChess(int x1, int y1, int x2,int y2);  // 移动指定位置的棋子到指定的位置

    QString intToColor(int value);  // 返回棋子编号对应的颜色，正数是蓝色，负数为红色

protected:
    void paintEvent(QPaintEvent * event);   // 画面更新的时候，调用的一个函数
    void mousePressEvent(QMouseEvent *event);   // 点击鼠标的事件，如果是在棋盘的上面才进行进一步的判断
    int getChessRoundIndex();   // 返回1~6的随机数


// 信号槽
private slots:

    void on_diceButton_clicked();

    void on_startMatchButton_clicked();

    void on_searchDeepCombBox_currentIndexChanged(int index);

    void on_backButton_clicked();


    void on_replayMatchButton_clicked();

    void on_setOurColorComboBox_currentIndexChanged(int index);

    void on_setSenteComboBox_currentIndexChanged(int index);


    void on_useGivenDiceButton_clicked();

private:
    Ui::MainWindow *ui; // 指向ui文件里面的类创建的 对象

    // 博弈变量
    int searchDeep;

    // 棋局信息
    Board board;


    // 棋盘是否初始化完成（我方布局是否完成）
    bool isChessSeted;


    // 棋盘 Ui 参数
    int startX;
    int startY;
    int gridX;
    int gridY;
    int chessX;
    int chessY;
    int boardWidth;
    int boardHeight;

    Logic logic;

    QMap<char,int> selectedPosition = {
        {'x',-1},
        {'y',-1}
    };



};
#endif // MAINWINDOW_H
