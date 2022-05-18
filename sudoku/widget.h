#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include"mypushbutton.h"

QT_BEGIN_NAMESPACE

namespace Ui { class Widget; }

QT_END_NAMESPACE
class Form;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void clearNum(int r,int c);//清除按钮上的数字
    void setNum(int r,int c,int num);//设置按钮上的数字
    void create(int cnt);//创建局面
    void chooseOne(int r,int c);//点击格子后触发此函数让玩家9选1
    bool check();//检查当前是否正确
    int get_num(int r,int c)//得到给定位置预设的数字
    {
        return _num[r][c];
    }
    void set_Num(int r,int c,int x)//设定给定位置预设的数字
    {
        _num[r][c]=x;
    }
private:
    Ui::Widget *ui;
    int _num[9][9];//预设的9*9数独
    QPushButton _btn[9][9];//9*9的按钮
};
#endif // WIDGET_H
