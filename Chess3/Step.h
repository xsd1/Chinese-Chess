#ifndef STEP_H
#define STEP_H

#include <QWidget>

class Step : public QWidget
{
    Q_OBJECT
public:
    explicit Step(QWidget *parent = nullptr);

    //行走棋子的id
    int _moveid;
    //杀掉棋子的id
    int _killid;
    //起始行坐标
    int _rowFrom;
    //起始列坐标
    int _colFrom;
    //目标位置行坐标
    int _rowTo;
    //目标位置列坐标
    int _colTo;

signals:

public slots:
};

#endif // STEP_H
