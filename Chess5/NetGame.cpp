#include "NetGame.h"
#include <QDebug>


NetGame::NetGame(bool server)
{
    _server = NULL;
    _socket = NULL;

    if (server)
    {
        _server = new QTcpServer (this);
        _server->listen(QHostAddress::Any,9999);

        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    }
    else
    {
        _socket = new QTcpSocket (this);
        _socket->connectToHost(QHostAddress("127.0.0.1"),9999);

       connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));
    }

}

void NetGame::click(int id, int row, int col)
{
    if (_selectid == -1&&id != -1)
    {
        if (_s[id]._red != _bSide)
            return ;
    }

    Board::click(id,row,col);//为了让棋子动起来

    //发送给对方

    char buf[4];
    buf[0]=2;
    buf[1]=9-row;
    buf[2]=8-col;
    buf[3]=id;
    _socket->write(buf,4);
}

void NetGame::back()
{
   if (_bRedTurn != _bSide)
       return;
   backOne();
   backOne();

  char buf[1];
  buf[0]=3;
  _socket->write(buf,1);
}

void NetGame::slotRecv()
{
    QByteArray ba = _socket->readAll();
    char cmd = ba[0];
    if (cmd==1)
    {
        //初始化
        char data = ba[1];
       init(data==1);
    }
    if (cmd == 2)
    {
        int row=ba[1];
        int col=ba[2];
        int id =ba[3];
        Board::click(id,row,col);
    }
    if (cmd== 3)
    {
        backOne();
        backOne();
    }
}

void NetGame::slotNewConnection()
{
    if (_socket) return ;

    //连接成功会有套接字
    _socket = _server->nextPendingConnection();

    //接受来自客户端的消息
    connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));

    //随机数来判断谁走红，谁走黑
    //走红要旋转，走黑不要旋转
    //走红bRedSide为true
    bool bRedSide = qrand()%2>0;
    init(bRedSide);


   //给对方发送数据

    char buf[2];
    buf[0] = 1;
    buf[1] = bRedSide?0:1;
    //如果我是走红，我不需要旋转
    //就要把0给客户端，让他旋转

    _socket->write(buf,2);

}
