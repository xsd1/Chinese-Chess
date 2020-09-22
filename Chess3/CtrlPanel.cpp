#include "CtrlPanel.h"
#include <QFont>
#include <QPainter>



CtrlPanel::CtrlPanel(QWidget *parent) : QWidget(parent)
{


    NetGame * board3 = new NetGame(true);
    NetGame * board4 = new NetGame(false);
    QFont font ("华文行楷",15,75);
    this->resize(550,500);
    _button1.setFont(font);
    _button1.setText("人人对战");
    _button1.setParent(this);
    _button1.move(350,100);
    _button2.setFont(font);
    _button2.setText("人机对战");
    _button2.setParent(this);
    _button2.move(350,200);
    _button3.setFont(font);
    _button3.setText("网络对战(服务端)");
    _button3.setParent(this);
    _button3.move(350,300);
    _button4.setFont(font);
    _button4.setText("网络对战(客户端)");
    _button4.setParent(this);
    _button4.move(350,400);

    connect(&_button1,&QPushButton::clicked,
            [=]()
    {
          this->hide();
          board1.show();
    }
            );

    connect(&_button2,&QPushButton::clicked,
            [=]()
    {
          this->hide();
          board2.show();
    }
            );
    connect(&_button3,&QPushButton::clicked,
            [=]()
    {
          this->hide();
          board3->show();
    }
            );
    connect(&_button4,&QPushButton::clicked,
            [=]()
    {
          this->hide();
          board4->show();
    }
            );

}

void CtrlPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter (this);
    painter.drawPixmap(0,0,width(),height(),QPixmap("../222.jpg"));
}
