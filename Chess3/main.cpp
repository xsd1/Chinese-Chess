#include <QApplication>
#include "SingleGame.h"
#include "NetGame.h"
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include "CtrlPanel.h"



int main (int argc,char * argv[])
{
    QApplication app (argc,argv);
    CtrlPanel board;
    board.show();
   /* QMessageBox::StandardButton ret;
    ret = QMessageBox::question(NULL,"server or client","作为服务器启动");
    bool bServer = false;
    if (ret == QMessageBox::Yes)
    {
        bServer = true;
    }

    NetGame board(bServer);*/
    //SingleGame board;
    //Board board;
    return app.exec();
}
