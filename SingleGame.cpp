#include "SingleGame.h"

void SingleGame::click(int id, int row, int col)
{
    if (!this->_bRedTurn)
        return;

    Board::click(id,row,col);

    if (!this->_bRedTurn)
    {
       Step * step= getBestMove();
       moveStone(step->_moveid,step->_killid,step->_rowTo,step->_colTo);
    }
}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
     for (int  i=0;i<16;i++)
     {
         if (_s[i]._dead) continue;
         for (int row=0;row<=9;++row)
         {
             for  (int col=0;col<=8;++col)
             {
                 int killid = this->getStoneId(row,col);
                 if (sameColor(killid,i)) continue;

                 if (canMove(i,row,col,killid))
                 {
                     saveStep(i,killid,row,col,steps);
                 }
             }
         }
     }
}

void SingleGame::fakeMove(Step *step)
{
    killStone(step->_killid);
    moveStone(step->_moveid,step->_rowTo,step->_colTo);
}

void SingleGame::unfakeMove(Step *step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid,step->_rowFrom,step->_colFrom);
}

//评价局面分
int SingleGame::calcScore()
{
    int redTotalScore = 0;
    int blackTotalScore = 0;
    //enum TYPE{JIANG,CHE,PAO,MA,BING,SHI,XIANG};
    static int chessScore[] = {100,50,50,20,1500,10,10};

    //黑棋分的总数-红棋分的总数
    for  (int i=16;i<32;i++)
    {
         if (_s[i]._dead) continue;
         redTotalScore += chessScore[_s[i]._type];
    }
    for  (int i=0;i<16;i++)
    {
         if (_s[i]._dead) continue;
         blackTotalScore += chessScore[_s[i]._type];
    }
    return blackTotalScore-redTotalScore;
}
Step* SingleGame::getBestMove()
{
    //1.看看有哪些步骤可以走
    QVector<Step *> steps;
    getAllPossibleMove(steps);

    //2.试着走一下
    int maxScore = -100000;
    Step *ret ;
    for (QVector<Step *>::iterator it=steps.begin();it!=steps.end();++it)
    {
        Step * step = *it;
        fakeMove (step);
        int score = calcScore ();
        unfakeMove (step);

        if (score > maxScore)
        {
            maxScore = score;
            ret = step;
        }
    }

    //4.取最好的结果作为参考
    return ret;
}
