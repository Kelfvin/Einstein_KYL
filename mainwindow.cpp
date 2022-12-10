#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"
#include "QPainter"
#include <QTime>
#include <QMouseEvent>
#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    boardWidth = boardHeight = 400;
    gridX = boardWidth/5;
    gridY = boardHeight/5;
    startX = 0;
    startY = 0;


    isChessSeted = false;

    searchDeep = 1;

    ui->backButton->setEnabled(false);
    ui->diceButton->setEnabled(false);

    initSearchDeepCombBox();
    initSetSenteCombBox();
    initSetOurColorCombBox();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBoardTap(int x, int y)
{
    if (!isChessSeted) // 表示还可以设置棋子
    {
            if (board.getChess(x,y) != 0) //不允许设置空白区
            {
                int chessNumber = getChessRoundIndex()*board.getOurColor();
                board.setChess(x,y,chessNumber);

            }


    }
    else//棋子数字已经设置完毕
    {
        if(selectedPosition['x'] == -1){
            selectedPosition['x'] = x;
            selectedPosition['y'] = y;
        }
        else{

            moveChess(selectedPosition['x'],selectedPosition['y'],x,y);
        }

    }

}

void MainWindow::showMsg(QString str)
{
    QMessageBox msg;
    msg.setText(str);
    msg.exec();
}

void MainWindow::initSearchDeepCombBox()
{
    for(int i = 1;i<=6;i++){
        ui->searchDeepCombBox->addItem(QString::asprintf("%d",i));
    }

}

void MainWindow::initSetOurColorCombBox()
{
    ui->setOurColorComboBox->addItem("蓝色");
    ui->setOurColorComboBox->addItem("红色");
}

void MainWindow::initSetSenteCombBox()
{
    ui->setSenteComboBox->addItem("蓝色");
    ui->setSenteComboBox->addItem("红色");
}

void MainWindow::drawChessNumber(int x, int y)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);

    int chessNumber = abs(board.getChess(x,y));

    painter.setPen(QPen(Qt::black, 1));
    painter.setFont(QFont(QString::fromLocal8Bit("微软雅黑"), 40, -1, false));
    painter.drawText(startX+ gridX * (x+0.3), startY+gridY* (y+0.9), QString::number(chessNumber));
}

void MainWindow::drawBoardNumber()
{
    //显示棋子数字
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (board.getChess(i,j) < 0 && board.getChess(i,j) > -7)
            {
               drawChessNumber(i,j);

            }
            if (board.getChess(i,j) > 0 && board.getChess(i,j) < 7)
            {
                drawChessNumber(i,j);
            }
        }
    }
}

void MainWindow::drawBoardColor()
{

    QPainter painter(this);
    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);
    // 绘制棋子颜色（不带数字）
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (board.getChess(i,j) != 0)
            {
                if (board.getChess(i,j) < 0) // 红方
                {
                    painter.setBrush(QBrush(QColor(255, 0, 0), Qt::SolidPattern));

                }
                else //蓝方
                {
                    painter.setBrush(QBrush(QColor(0, 0, 255), Qt::SolidPattern));
                }
                painter.drawRect(startX+i * gridX, startY+j*gridY, gridX, gridY);
            }
        }
    }

    if(selectedPosition.contains('x') && selectedPosition.contains('y')){
        int x = selectedPosition['x'];
        int y = selectedPosition['y'];
        if(x>=0 && y>=0){

        painter.setBrush(QBrush(QColor(185, 214, 163), Qt::SolidPattern));

        painter.drawRect(startX+x * gridX, startY+y*gridY, gridX, gridY);
        }
    }
}

void MainWindow::drawBoardLine()
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);
    //画棋盘
    for (int i = 0; i <= 5; i++) {
        //画横线
        painter.drawLine(startX, startY+gridY*i, startX+boardWidth, startY+gridY*i);
        //画竖线
        painter.drawLine(startX+i*gridX, startY, startX+i*gridX, startY+boardHeight);
    }
}

void MainWindow::letAIDo()
{
    logic.setvirtueTable(board.getBoard());
    QVector<QPoint> pointsArr = logic.getPointToGo();
    QPoint start =  pointsArr[0];
    QPoint end = pointsArr[1];
    qDebug() << start.x() << " " << start.y()<<" " << end.x() <<" "<< end.y();
    moveChess(start.x(),start.y(),end.x(),end.y());
    update();
}

void MainWindow::moveChess(int x1, int y1, int x2, int y2)
{



    bool success =  board.moveChess(x1,y1,x2,y2);


    selectedPosition['x'] = -1;
    selectedPosition['y'] = -1;
    if(success){
        char buffer[100];
        sprintf(buffer,"移动(%d,%d)->(%d,%d)",x1,y1,x2,y2);
        QString moveInfo = buffer;
        ui->boardStatusBar->append(intToColor(-board.getNowPlayer())+moveInfo);

        int win = board.checkWin();
    if(win== 0){
        ui->boardStatusBar->append("现在该"+board.getNowPlayerStr()+"出手");
    }

    else if(win == 1){
       showMsg("蓝方赢了！");
    }

    else{
       showMsg("红方赢了");
    }


    }else{
        showMsg("操作非法！");
    }

    update();
}

QString MainWindow::intToColor(int value)
{
    return value>0? "蓝色":"红色";
}



void MainWindow::paintEvent(QPaintEvent *event)
{

    drawBoardLine();

    drawBoardColor();


    drawBoardNumber();

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint point = event->pos();       //获取点击位置
    int x = qRound(float(point.x() / gridX)); //四舍五入
    int y = qRound(float(point.y() / gridY));


    // 点击棋盘
    if (x < 5 && y < 5)
    {
        onBoardTap(x,y);
        update();
    }

}



int MainWindow::getChessRoundIndex()
{
    static QVector<int> chessArr ={1,2,3,4,5,6};
    static int index = 0;

    int result = chessArr[index];
    index = (index+1)%6;
    return result;
}



// 掷骰子
void MainWindow::on_diceButton_clicked()
{
    qsrand(QTime::currentTime().msec());

    int dice = qrand()%6+1;
    board.setDice(dice);

    QString str = board.getNowPlayerStr()+"投掷出了" + QString::number(dice);
    ui->boardStatusBar->append(str);


    // 如果是我们的轮，就让 AI 算法来走
    if(board.getNowPlayer()==board.getOurColor()){
        letAIDo();
    }



    ui->backButton->setEnabled(true);
}

// 确定按钮按下，开始比赛。需要对布局的合法性进行评判
void MainWindow::on_startMatchButton_clicked()
{
    bool checkResult = board.checkBoardSet();

    if(checkResult){
        isChessSeted = true;
        QMessageBox msgBox;
        msgBox.setText("比赛开始！");
        msgBox.exec();
        board.setNowPlayer(board.getSente());

        ui->boardStatusBar->append("现在是"+board.getNowPlayerStr()+"出手");

        ui->startMatchButton->setEnabled(false);
        ui->diceButton->setEnabled(true);
        ui->backButton->setEnabled(true);
        ui->setSenteComboBox->setEnabled(false);
        ui->setOurColorComboBox->setEnabled(false);
    }

    else{
        QMessageBox msgBox;
        msgBox.setText("布局非法，请检查！");
        msgBox.exec();
    }
}



void MainWindow::on_searchDeepCombBox_currentIndexChanged(int index)
{
    searchDeep = index+1 ;
    logic.setDepth(searchDeep);
    ui->boardStatusBar->append("设置搜索深度为"+QString::number(searchDeep));
}

void MainWindow::on_backButton_clicked()
{
    board.undo();
    ui->boardStatusBar->append("撤销");
    if(!board.canUndo()){
//        ui->backButton->setEnabled(false);
    }
    update();
}

void MainWindow::on_replayMatchButton_clicked()
{
    // 调试模式下没用，需要 release 版本
    QString program = QApplication::applicationFilePath();
    QStringList arguments = QApplication::arguments();
    QString workingDirectory = QDir::currentPath();
    QProcess::startDetached(program, arguments, workingDirectory);
    QApplication::exit();

}

void MainWindow::on_setOurColorComboBox_currentIndexChanged(int index)
{
    int color = index == 0? 1:-1;
    board.setOurColor(color);
    logic.setourColor(board.getOurColor());
    ui->boardStatusBar->append("设置我方队伍颜色为"+intToColor(color));
}

void MainWindow::on_setSenteComboBox_currentIndexChanged(int index)
{
     int color = index == 0? 1:-1;
     board.setSente(color);
     logic.setSente(board.getSente());

     ui->boardStatusBar->append("设置先手颜色为"+intToColor(color));
}
