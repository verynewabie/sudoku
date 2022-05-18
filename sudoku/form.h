#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include<QPushButton>
#include<QDialog>

namespace Ui {
    class Form;
}
class Widget;

class Form : public QDialog
{
    Q_OBJECT

public:
    //第一个参数是鼠标的位置，第二个参数是父亲，第三个参数是主窗口，最后两个参数确定玩家点击的按钮的位置
    explicit Form(QPoint point,QWidget *parent = nullptr,Widget* widget=0,int r=0,int c=0);
    ~Form();
    void clearNumWhenClicked();//配合清除当前方格的槽函数
private:
    Ui::Form *ui;
    QPushButton _btn[9];//9选1的按钮
    Widget* _widget;
    int _r,_c;
    QPoint _point;
};

#endif // FORM_H
