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
    void onBoardTap(int x , int y);
    void showMsg(QString str);
    void initSearchDeepCombBox();
    void initSetOurColorCombBox();
    void initSetSenteCombBox();
    void drawChessNumber(int x,int y);
    void drawBoardNumber();
    void drawBoardColor();
    void drawBoardLine();
    void letAIDo();
    void moveChess(int x1, int y1, int x2,int y2);

    QString intToColor(int value);

protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
    int getChessRoundIndex();



private slots:

    void on_diceButton_clicked();

    void on_startMatchButton_clicked();

    void on_searchDeepCombBox_currentIndexChanged(int index);

    void on_backButton_clicked();


    void on_replayMatchButton_clicked();

    void on_setOurColorComboBox_currentIndexChanged(int index);

    void on_setSenteComboBox_currentIndexChanged(int index);


private:
    Ui::MainWindow *ui;

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
