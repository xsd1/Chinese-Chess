#ifndef NETGAME_H
#define NETGAME_H
#include "Board.h"
#include <QTcpServer>
#include <QTcpSocket>

//执红方还是黑方，这个信息由服务器发出，客户端接收
//第一个字节固定是1，第二个字节是1，或者0.  1表示接收方走红棋，0表示走黑棋
//点击信息
//第一个字节固定为2，第二个字节是row，第三个字节是col，第四个字节是点击的棋子id

class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool server);

    QTcpServer* _server;
    QTcpSocket* _socket;

    void click(int id,int row,int col);
    void back();
public slots:
    void slotNewConnection ();
    void slotRecv ();
};

#endif // NETGAME_H
