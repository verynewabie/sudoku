#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include<widget.h>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = 0);

    ~MyPushButton();

signals:
    void clicked(int r,int c);
};

#endif // MYPUSHBUTTON_H
