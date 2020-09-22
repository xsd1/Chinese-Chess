#include "SingleGame.h"
#include <QTimer>

void SingleGame::click(int id, int row, int col)
{
    if (!this->_bRedTurn)
        return;

    //如果是人走，就调用这个函数
    Board::click(id,row,col);

    //电脑走
    if (!this->_bRedTurn)
    {
        //启动0.1s定时器,在0.1s后，电脑再思考
        QTimer::singleShot(100,this,SLOT(computerMove()));
    }
}

void SingleGame::computerMove()
{
    Step * step= getBestMove();
    moveStone(step->_moveid,step->_killid,step->_rowTo,step->_colTo);
    delete  step;
    update();
}
//得到所有可以走的步骤，并保存在step中
void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
     int min=16,max=32;
     if (this->_bRedTurn)
     {
         min = 0;
         max=16;
     }
     for (int  i=min;i<max;i++)
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

//进行模拟的移动
void SingleGame::fakeMove(Step *step)
{
    killStone(step->_killid,1);
    moveStone(step->_moveid,step->_rowTo,step->_colTo);
}

//恢复棋子
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

int SingleGame::getMaxScore(int level,int curMinScore)
{
    if (level==0) return calcScore();
    //1.看看有哪些步骤可以走
    QVector<Step *> steps;
    getAllPossibleMove(steps); //人走的（即红棋的）所有可能步数

    int maxScore = -100000;
    while (steps.count())
    {
        Step * step = steps.back();
        steps.removeLast();

        fakeMove (step);
        int score = getMinScore (level-1,maxScore);
        unfakeMove (step);

        delete step;
        if (score >= curMinScore)
        {
            while (steps.count())
            {
                Step * step = steps.back();
                steps.removeLast();
                delete  step;
            }
            return score;
        }
        if (score > maxScore)
        {
            maxScore = score;
        }
    }
    return maxScore;

}

int SingleGame::getMinScore(int level,int curMaxScore)
{
    if (level==0) return calcScore();
    //1.看看有哪些步骤可以走
    QVector<Step *> steps;
    getAllPossibleMove(steps); //人走的（即红棋的）所有可能步数

    int minScore = 100000;
    while (steps.count())
    {
        Step * step = steps.back();
        steps.removeLast();
        fakeMove (step);
        int score = getMaxScore (level-1,minScore);
        unfakeMove (step);
        delete step;
        if (score<=curMaxScore)
        {
            while (steps.count())
            {
                Step * step = steps.back();
                steps.removeLast();
                delete  step;
            }
            return score;
        }
        if (score < minScore)
        {
            minScore = score;
        }

    }
    return minScore;
}
Step* SingleGame::getBestMove()
{
    //1.看看有哪些步骤可以走
    QVector<Step *> steps;
    getAllPossibleMove(steps);

    //2.试着走一下
    int maxScore = -100000;
    Step *ret = NULL;
    while (steps.count())
    {
        Step * step = steps.back();
        steps.removeLast();
        fakeMove (step);
        //机器走后，在人走的步数中得到的最小值(在机器可以走的几步中)
        int score = getMinScore (_level-1,maxScore);
        //之后再恢复
        unfakeMove (step);

        //找到最好的移动
        //在最小值中找到最大值
        if (score > maxScore)
        {
            maxScore = score;
            if (ret) delete ret;
            ret = step;
        }
        else
        {
            delete  step;
        }
    }

    //4.取最好的结果作为参考
    return ret;
}
