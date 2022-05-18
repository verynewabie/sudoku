#include "widget.h"
#include "ui_widget.h"
#include"form.h"
#include<QDebug>
#include<QPushButton>
#include<QColor>
#include<QString>
#include<QDialog>
#include<QAction>
#include<QMessageBox>
#include<QRandomGenerator>
#include<QString>
#include"mypushbutton.h"

#define cout qDebug().noquote().nospace()
//我这里#define是为了调试方便

void Widget::chooseOne(int r,int c)
{
    QPoint point=this->cursor().pos();//获取鼠标位置
    Form* fm = new Form(point,0,this,r,c);//新建一个9选1的
    fm->exec();
}

void Widget::create(int cnt)//随机生成一个一定有解的状态，cnt表示给玩家的数字的个数
{
    //我这里使用《编程之美》一书P97页左右的算法来生成数独，这本书上还有另一种更好的算法，不过比较麻烦，我就没用
    //而且这种算法能够生成9!种数独，够玩了
    QRandomGenerator rnd;
    rnd.seed(time(0));
    int mid[9]={0};
    bool use[9]={0};
    for(int i=0;i<9;i++)
    {
        bool flag=1;
        while(flag)
        {
            int tmp=(rnd()%9+9)%9;
            if(!use[tmp])
            {
                use[tmp]=1;
                mid[i]=tmp;
                flag=0;
            }
        }
    }
    int num[9][9]=
    {
        {mid[8],mid[6],mid[7],mid[2],mid[0],mid[1],mid[5],mid[3],mid[4]},
        {mid[2],mid[0],mid[1],mid[5],mid[3],mid[4],mid[8],mid[6],mid[7]},
        {mid[5],mid[3],mid[4],mid[8],mid[6],mid[7],mid[2],mid[0],mid[1]},
        {mid[6],mid[7],mid[8],mid[0],mid[1],mid[2],mid[3],mid[4],mid[5]},
        {mid[0],mid[1],mid[2],mid[3],mid[4],mid[5],mid[6],mid[7],mid[8]},
        {mid[3],mid[4],mid[5],mid[6],mid[7],mid[8],mid[0],mid[1],mid[2]},
        {mid[7],mid[8],mid[6],mid[1],mid[2],mid[0],mid[4],mid[5],mid[3]},
        {mid[1],mid[2],mid[0],mid[4],mid[5],mid[3],mid[7],mid[8],mid[6]},
        {mid[4],mid[5],mid[3],mid[7],mid[8],mid[6],mid[1],mid[2],mid[0]}
    };

    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            num[i][j]++;
            set_Num(i,j,num[i][j]);//我们这里还有记好我们生成的数独，因为我们使用查看答案还是要用到的
        }
    }
    //调试代码
    //for(int i=0;i<9;i++)
            //cout<<num[i][0]<<" "<<num[i][1]<<" "<<num[i][2]<<" "<<num[i][3]<<" "<<num[i][4]<<" "<<num[i][5]<<" "<<num[i][6]<<" "<<num[i][7]<<" "<<num[i][8];
    bool s[81]={0};
    int count=0;
    //确定保留的数的位置
    while(count!=cnt)
    {
        int tmp=(rnd()%81+81)%81;
        if(s[tmp]==0) s[tmp]=1,count++;
    }

    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            int tar=i*9+j;
            if(s[tar]==1)//对于要保留的数，我们让按钮失效
            {
                _btn[i][j].setText(QString::number(num[i][j]));//这里的默认字体是灰色
                //_btn[i][j].setStyleSheet("background-color:rgb(0,255,0)");
                _btn[i][j].setEnabled(false);
            }
        }
    }
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("sudoku");
    QStringList color;
    //我们给相邻的宫给上不同的颜色，便于区分
    color   <<"background-color:rgb(255,192,203)"//粉红
            <<"background-color:rgb(0,255,0)"//绿
            <<"background-color:rgb(176,196,222)"//淡钢蓝
            <<"background-color:rgb(176,196,222)"
            <<"background-color:rgb(255,192,203)"
            <<"background-color:rgb(0,255,0)"
            <<"background-color:rgb(0,255,0)"
            <<"background-color:rgb(176,196,222)"
            <<"background-color:rgb(255,192,203)";
    for(int r=0;r<9;r++)
    {

        for(int c=0;c<9;c++)
        {
            _btn[r][c].setFixedSize(65,65);
            QFont ft(nullptr,35);
            _btn[r][c].setFont(ft);
            _btn[r][c].setText("");
            _btn[r][c].setEnabled(true);
            int tor=r/3,toc=c/3;
            int tar=tor*3+toc;
            _btn[r][c].setStyleSheet(color[tar]);//这里的默认字体是黑色，正好能让我们区分开
            ui->gridLayout->addWidget(&_btn[r][c],r,c);
            connect(&_btn[r][c],&QPushButton::clicked,[=](){
                QPoint point=this->cursor().pos();
                Form* fm = new Form(point,0,this,r,c);
                fm->exec();
            });
        }
    }
    //选择难度
    QMessageBox* choice=new QMessageBox(this);
    choice->setWindowTitle("Your difficulty choice");
    choice->setText("  Please select the game difficulty.");
    choice->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::Ok);
    choice->setButtonText(QMessageBox::Yes,"simple");
    choice->setButtonText(QMessageBox::Cancel,"hard");
    choice->setButtonText(QMessageBox::Ok,"middle");
    choice->setDefaultButton(QMessageBox::Yes);//默认难度为简单
    auto tmp=choice->exec();
    int cnt=0;//cnt是初始给玩家的梳数字数
    if(tmp==QMessageBox::Yes)//选择简单难度
    {
        create(40);
        cnt=40;
    }
    else if(tmp==QMessageBox::Ok)//选择中等难度
    {
        create(32);
        cnt=32;
    }
    else if(tmp==QMessageBox::Cancel)//选择困难难度
    {
        create(24);
        cnt=24;
    }
    //点击游戏帮助后弹出一个消息对话框
    connect(ui->btn_help,&QPushButton::clicked,[=](){
        QMessageBox::information(this,"help","数独是源自18世纪瑞士的一种数学游戏。是一种"
                                             "运用纸、笔进行演算的逻辑游戏。玩家需要根据9×"
                                             "9盘面上的已知数字，推理出所有剩余空格的数字，"
                                             "并满足每一行、每一列、每一个粗线宫（3*3）内的数"
                                             "字均含1-9，不重复。\n数独盘面是个九宫，每一宫又"
                                             "分为九个小格。"
                                             "在这八十一格中给出一定的已知数字和解题条"
                                             "件，利用逻辑和推理，在其他的空格上填入1-"
                                             "9的数字。使1-9每个数字在每一行、每一列和"
                                             "每一宫中都只出现一次，所以又称“九宫格”。(以上全部摘自百度百科)\n"
                                             "鼠标点击每个小格子即可选择要填入的数字或者清除小格子的数字。\n点击提交答案即可检验你的答案是否正确"
                                             "。\n点击查看答案程序会自动为你填上最终答案。\n点击生成局面即能再来一局。\n");
    });
    //点击查看答案后填上最终答案
    connect(ui->btn_answer,&QPushButton::clicked,[=](){
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                setNum(i,j,_num[i][j]);
            }
        }
    });
    //这里又copy了一遍上面create函数的代码，主要自己太懒了
    connect(ui->btn_create,&QPushButton::clicked,[=](){
        QRandomGenerator rnd;
        rnd.seed(time(0));
        int mid[9]={0};
        bool use[9]={0};
        for(int i=0;i<9;i++)
        {
            bool flag=1;
            while(flag)
            {
                int tmp=(rnd()%9+9)%9;
                if(!use[tmp])
                {
                    use[tmp]=1;
                    mid[i]=tmp;
                    flag=0;
                }
            }
        }
        int num[9][9]=
        {
            {mid[8],mid[6],mid[7],mid[2],mid[0],mid[1],mid[5],mid[3],mid[4]},
            {mid[2],mid[0],mid[1],mid[5],mid[3],mid[4],mid[8],mid[6],mid[7]},
            {mid[5],mid[3],mid[4],mid[8],mid[6],mid[7],mid[2],mid[0],mid[1]},
            {mid[6],mid[7],mid[8],mid[0],mid[1],mid[2],mid[3],mid[4],mid[5]},
            {mid[0],mid[1],mid[2],mid[3],mid[4],mid[5],mid[6],mid[7],mid[8]},
            {mid[3],mid[4],mid[5],mid[6],mid[7],mid[8],mid[0],mid[1],mid[2]},
            {mid[7],mid[8],mid[6],mid[1],mid[2],mid[0],mid[4],mid[5],mid[3]},
            {mid[1],mid[2],mid[0],mid[4],mid[5],mid[3],mid[7],mid[8],mid[6]},
            {mid[4],mid[5],mid[3],mid[7],mid[8],mid[6],mid[1],mid[2],mid[0]}
        };

        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                num[i][j]++;
            }
        }
        //调试代码
        //for(int i=0;i<9;i++)
                //cout<<num[i][0]<<" "<<num[i][1]<<" "<<num[i][2]<<" "<<num[i][3]<<" "<<num[i][4]<<" "<<num[i][5]<<" "<<num[i][6]<<" "<<num[i][7]<<" "<<num[i][8];
        bool s[81]={0};
        int count=0;
        while(count!=cnt)
        {
            int tmp=(rnd()%81+81)%81;
            if(s[tmp]==0) s[tmp]=1,count++;
        }

        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                int tar=i*9+j;
                if(s[tar]==1)
                {
                    _btn[i][j].setText(QString::number(num[i][j]));
                    //_btn[i][j].setStyleSheet("background-color:rgb(0,255,0)");
                    _btn[i][j].setEnabled(false);
                }
                else
                {
                    _btn[i][j].setText("");
                    _btn[i][j].setEnabled(true);
                }

            }
        }
    });

    connect(ui->btn_submit,&QPushButton::clicked,[=](){
        int col[9]={0},row[9]={0},cell[3][3]={0};
        bool flag=1;
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                int num=_btn[i][j].text().toInt()-1;
                if(row[i]>>num&1||col[j]>>num&1||cell[i/3][j/3]>>num&1)
                {
                    flag=0;
                }
                col[j]|=1<<num;
                row[i]|=1<<num;
                cell[i/3][j/3]|=1<<num;
            }
        }
        if(flag==1)
        {
            QMessageBox::information(this,"congratulations","Accepted");
        }
        else
        {
            QMessageBox::information(this,"what a pity","Wrong Answer");
        }
    });
}
void Widget::setNum(int r, int c, int num)
{
    _btn[r][c].setText(QString::number(num));
}

void Widget::clearNum(int r, int c)
{
    _btn[r][c].setText("");
}

//这个可能等会就不用了
bool Widget::check()
{
    int col[9]={0},row[9]={0},cell[3][3]={0};
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            int num=_btn[i][j].text().toInt()-1;
            if(col[i]>>num&1||col[j]>>num&1||cell[i/3][j/3]>>num&1)
            {
                return false;
            }
            col[j]|=1<<num;
            row[i]|=1<<num;
            cell[i/3][j/3]|=1<<num;
        }
    }
    return true;
}

Widget::~Widget()
{
    delete ui;
}

