#include "board.h"
#include <QPainter>
#include <QRect>
#include <QMouseEvent>
#include <QDebug>
Board::Board(QWidget *parent)
    : QWidget(parent)
{
    for(int i = 0;i < 32;i++)
    {
        _s[i].init(i);
    }
    _selectid = -1;
    _bRedTurn = true;
}

Board::~Board()
{


}
void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 40;
    _r = d/2;
    //画10条横线
    for(int i = 1;i <= 10;i++)
    {
         painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //画9条竖线
    for(int i = 1;i <= 9;i++)
    {
        if(i == 1||i == 9)
        painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        else
        {
             painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
              painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }
    //九宫格
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    //绘制32个棋子
    for(int i = 0;i < 32;i++)
    {
        drawStone(painter ,i);
    }

}
//绘制象棋
void Board::drawStone(QPainter& painter ,int id)
{
    if(_s[id]._dead == true)
        return;//被吃掉的棋子不再显示
    QPoint c = center(id);
    QRect rect = QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);
    //设置象棋背景
    if(id == _selectid)//被选中的象棋颜色应该不一样
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));

    if(_s[id]._red)
        painter.setPen(Qt::red);
    else
        painter.setPen(Qt::black);

    painter.drawEllipse(center(id),_r,_r);
    //改变字体大小
    painter.setFont(QFont("system",_r,700));
    painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));
}
QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}
QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col+1)* _r*2;//列
    ret.ry() = (row+1)* _r*2;//行
    return ret;
}
//效率不高
bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    for(row = 0;row <= 9;row++)
    {
        for(col = 0;col <= 8;col++)
        {
            QPoint c = center(row,col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dis = dx*dx + dy*dy;
            if(dis < _r*_r)
                return true;
        }
    }
    return false;
}
int Board::getStoneCountLine(int row, int col, int row1, int col1)
{
    int sum = 0;
    if(row != row1 &&col != col1)
        return -1;
    if(row == row1)
    {
        int i =col > col1 ? col : col1;
        int j =col < col1 ? col : col1;
        j++;
        for(;j < i;j++)
           if(getStone(row,j))
                    sum++;
    }
    if(col == col1)
    {
        int i = row > row1 ? row : row1;
        int j = row < row1 ? row : row1;
        j++;
        for(;j < i;j++)
            if(getStone(j,col))
                    sum++;
    }
    return sum;
}
bool Board::getStone(int row, int col)
{
    for(int i = 0;i < 32;i++)
        if(_s[i]._row == row&&_s[i]._col == col&&_s[i]._dead == false)
            return true;//所在位置有子
    return false;
}
bool Board::isBottomSide(int moveid)
{
    if(_s[moveid]._red)
        return true;
    return false;
}
bool Board::canMove1(int moveid, int row, int col, int killid)
{
    //目标位置在九宫内
    //移动步长一格
    if(isBottomSide(moveid))
    {
        if(row > 2)return false;
    }
    else
    {
        if(row < 7)return false;
    }
    if(col < 3||col > 5)return false;
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 1||d == 10)
        return true;
    return false;
}
bool Board::canMove2(int moveid, int row, int col, int killid)
{
    if(isBottomSide(moveid))
    {
        if(row > 2)return false;
    }
    else
    {
        if(row < 7)return false;
    }
    if(col < 3||col > 5)return false;
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 11)//士走对角线
        return true;
    return false;
}
//象走对角线
bool Board::canMove3(int moveid, int row, int col, int killid)
{
    if(isBottomSide(moveid))
    {
        if(row > 4)return false;
    }
    else
    {
        if(row < 5)return false;
    }
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d != 22)
        return false;
    int row1 = (_s[moveid]._row + row)/2;
    int col1 = (_s[moveid]._col + col)/2;
    if(getStone(row1,col1))
        return false;
    return true;
}
//车走直线
bool Board::canMove4(int moveid, int row, int col, int killid)
{
    int ret = getStoneCountLine(_s[moveid]._row,_s[moveid]._col,row,col);
    if(ret == 0)
        return true;
    if(ret != 0)
    return false;
}
//马走日
bool Board::canMove5(int moveid, int row, int col, int killid)
{
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d != 12&&d != 21)
        return false;
    if(d == 12)
    {
            int i =(_s[moveid]._col + col)/2;
            if(getStone(_s[moveid]._row,i))
            return false;
    }
    if(d == 21)
    {
         int i = (_s[moveid]._row + row)/2;
         if(getStone(i,_s[moveid]._col))
            return false;
    }
    return true;
}
//炮隔着子打
bool Board::canMove6(int moveid, int row, int col, int killid)
{
    int ret = getStoneCountLine(_s[moveid]._row,_s[moveid]._col,row,col);
    if(killid != -1)
    {
        if(ret != 1)
            return false;
        return true;
    }
    else
    {
        if(ret != 0)
            return false;
        return true;
    }

}
//兵不能回头,在自己家不可以横着走，到达对岸可以横着走
bool Board::canMove7(int moveid, int row, int col, int killid)
{
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d != 1&&d != 10)
        return false;
    if(isBottomSide(moveid))
    {
        if(row < _s[moveid]._row)
            return false;
        if(_s[moveid]._row < 5&&_s[moveid]._row == row)
            return false;
    }
    else
    {
        if(row > _s[moveid]._row)
            return false;
        if(_s[moveid]._row >4&&_s[moveid]._row == row)
            return false;
    }
    return true;
}
bool Board::canMove(int moveid, int row, int col, int killid)
{
    if(killid != -1)//
    {
        if(_s[moveid]._red == _s[killid]._red)
        {
            //两次点的棋子颜色相同，更换选择
            _selectid = killid;
            update();
            return false;
        }
    }

    switch(_s[moveid]._type)
    {
    case Stone::JIANG:
         return canMove1(moveid, row, col, killid);
    case Stone::SHI:
         return canMove2(moveid, row, col, killid);
    case Stone::XIANG:
         return canMove3(moveid, row, col, killid);
    case Stone::CHE:
         return canMove4(moveid, row, col, killid);
    case Stone::MA:
         return canMove5(moveid, row, col, killid);
    case Stone::PAO:
         return canMove6(moveid, row, col, killid);
    case Stone::BING:
         return canMove7(moveid, row, col, killid);
    }
    return true;
}
void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    //获得鼠标点击的位置
    QPoint pt = ev->pos();
    //将pt转化成象棋的行列值
    //判断这个行列值上面有没有棋子
    int row,col;
    bool ret = getRowCol(pt,row,col);
    if(ret == false)
        return;//点击处不在棋盘上
    int i;
    int clickid = -1;
    for(i = 0;i < 32;i++)
    {
        if(_s[i]._row == row&&_s[i]._col == col&&_s[i]._dead==false)
            break;
    }
    if(i < 32)
    {
        clickid = i;
    }//点击处附近找到一颗棋子
    if(_selectid == -1)
    {
        if(clickid != -1)
        {
            if(_bRedTurn == _s[clickid]._red)
            {
                _selectid = clickid;
                update();
            }

        }
    }//第一次先选中一颗棋子
    else
    {
        //走棋
        if(canMove(_selectid,row,col,clickid))
        {
            _s[_selectid]._row = row;
            _s[_selectid]._col = col;
            if(clickid != -1)
            {
                _s[clickid]._dead = true;//被点的地方有棋子，表示要吃掉它
            }
            _selectid = -1;//下一次移动
            _bRedTurn = !_bRedTurn;//换另一方走子
            update();
        }

    }
}
