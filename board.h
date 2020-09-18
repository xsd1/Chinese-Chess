#ifndef BOARD_H
#define BOARD_H
#include "stone.h"
#include <QWidget>
#include <QPainter>
class Board : public QWidget
{
    Q_OBJECT

public:
    Board(QWidget *parent = 0);
    ~Board();
    //绘制棋盘
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    Stone _s[32];
    int _r;//棋子半径
    int _selectid;//记录被选中的棋子
    bool _bRedTurn;

    void drawStone(QPainter& painter ,int id);
    //返回象棋棋盘行列对应的像素坐标
    QPoint center(int row,int col);
    QPoint center(int id);
    //点到棋盘外面返回false
    bool getRowCol(QPoint pt,int &row,int &col);

    bool canMove(int moveid, int row, int col, int killid);
    bool canMove1(int moveid, int row, int col, int killid);
    bool canMove2(int moveid, int row, int col, int killid);
    bool canMove3(int moveid, int row, int col, int killid);
    bool canMove4(int moveid, int row, int col, int killid);
    bool canMove5(int moveid, int row, int col, int killid);
    bool canMove6(int moveid, int row, int col, int killid);
    bool canMove7(int moveid, int row, int col, int killid);
    int getStoneCountLine(int row,int col,int row1,int col1);
    bool getStone(int row,int col);
    bool isBottomSide(int moveid);
};

#endif // BOARD_H
