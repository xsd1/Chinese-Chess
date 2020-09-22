#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include <QPushButton>
#include "SingleGame.h"
#include "NetGame.h"



class CtrlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlPanel(QWidget *parent = nullptr);

    QPushButton _button1;
    QPushButton _button2;
    QPushButton _button3;
    QPushButton _button4;

    Board board1;
    SingleGame board2;

    int _selected ;

    void paintEvent(QPaintEvent *event);
signals:

public slots:

};

#endif // CTRLPANEL_H
