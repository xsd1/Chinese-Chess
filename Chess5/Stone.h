#ifndef STONE_H
#define STONE_H

#include <QString>

class Stone
{
public:
    Stone();
    ~Stone ();

    enum TYPE{JIANG,CHE,PAO,MA,BING,SHI,XIANG};

    int _row;//行
    int _col;//列
    int _id; //编号
    bool _dead;
    bool _red;
    TYPE _type;


    QString name();

    void rotate();

    void init(int id);
};

#endif // STONE_H
