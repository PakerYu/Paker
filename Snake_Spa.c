#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#include "snake.h"

struct Section left, *head; //head 指向第一节
                            //left 储存原最后一节的内容
                            //最后一节的next链接到left

struct Snake_Info Snake;

struct Food food;

int Section_Size = sizeof(struct Section);   //Section的内存大小,用于动态链表创建 

int Drct=3;                 //蛇移动的方向:0,1,2,3分别代表上下左右
boolean Eatenflag=FALSE;    //判断蛇是否吃掉了食物
clock_t GameTime,FlashTime;
int Timeing=750;

boolean isDead=FALSE;       //判断是否死亡

//生成食物
void FoodCreate()
{
    int i;
    int a,b;
    struct Section *p;
    boolean sign=TRUE;

    srand((int)time(NULL));

    p=head;

    i=0;
   do
   {
        a=(rand()%(InterfaceWidth/2)+1)*2+1;
        b=rand()%(InterfaceHeight)+2;
        if(i==Snake.length-1)
            sign=FALSE;
        if(a == p->x || b == p->y)
        {
            sign=TRUE;
        }
        else
        {
            i++;
            p=p->next;
        }
   } while (sign);
   
    food.x=a;
    food.y=b;

    gotoposition(a,b);
    putchar('*');

}


//创建蛇身
void CreateBody()
{
    struct Section *p;
    int w,h;

    w=InterfaceWidth/2+1*2+1;
    h=InterfaceHeight/2+2;


    //第一节
    head=malloc(Section_Size);
    p=head;
    p->num=0;p->direction=3;p->x=w;p->y=h;
    gotoprint(p->x,p->y);
    p->next=malloc(Section_Size);
    p=p->next;
    //第二节
    p->num=1;p->direction=3;p->x=w-2;p->y=h;
    gotoprint(p->x,p->y);
    p->next=malloc(Section_Size);
    p=p->next;
    //第三节
    p->num=2;p->direction=3;p->x=w-4;p->y=h;
    gotoprint(p->x,p->y);
    p->next=&left;

    //left节
    left.x=w-6;left.y=h;
    left.next=NULL;left.num=SIZE+1;

    Snake.length=3;
}



//蛇身成长
void GrowUp()
{
    int i;
    struct Section *p;
    p=head;
    do
    {
        p=p->next;
    } while (p->num!=Snake.length-1);

    //此时p指向蛇的尾节
    p->next=malloc(Section_Size);
    p=p->next;

    p->num=Snake.length;
    Snake.length++;
    p->x=left.x;p->y=left.y;
    p->next=&left;

    int count;
    //蛇速度改变
    count=(Snake.length-3);
    if(count>0 && count<=5)
        Timeing=600;
    else if(count>5 && count<=10)
        Timeing=450;
    else if(count>10 && count<=15)
        Timeing=300;
    else if(count>15)
        Timeing=150;
}


//刷新蛇的状态
void RefreshSnake()
{
    int i,dir,t;
    static boolean sign;       //sign记录食物被吃时left是否被处理

    struct Section *p;
    p=head;
    dir=Drct;

    for(i=0;i<=Snake.length;i++)    //i=length时p指向的是left
    {
        if((sign) && i==Snake.length)
        {
            sign=FALSE;
            break;
        }
        
        t=p->direction;
        p->direction=dir;
        dir=t;                      //dir表示原该点储存的方向

        //此处直接更新每个节的坐标  
        switch(p->direction)
        {
            case 0: p->y-=1;break;

            case 1: p->y+=1;break;

            case 2: p->x-=2;break;

            case 3: p->x+=2;break;
        }  
        p=p->next;
    }

    //检测头部是否迟到食物
    if(head->x==food.x && head->y==food.y)
    {    
        Eatenflag=TRUE;
        sign=TRUE;
        GrowUp();
    }

    //检测是否撞墙
    if(head->x==InterfaceWidth+3 || head->x==1)
        isDead=TRUE;
    else if(head->y==InterfaceHeight+2 || head->y==1)
        isDead=TRUE;

    p=head->next;
    //检测是否撞到自身  
    for(i=1;i<Snake.length;i++)
    {
        if(head->x==p->x && head->y==p->y)
        {
            isDead=TRUE;
            break;
        }

        p=p->next;
    }
}


//键盘输入,返回0表示输入不合法
boolean keyscan()
{
    boolean flag;
    char ch;

    flag=TRUE;

    if(_kbhit())
    {
        ch=_getch();
        
        switch(ch)
        {
            case 72:    Drct=0;break;   //0--向上

            case 80:    Drct=1;break;   //1--向下

            case 75:    Drct=2;break;   //2--向左

            case 77:    Drct=3;break;   //3--向右
        }
    }

    GameTime=clock();
        if((GameTime-FlashTime)>=Timeing)    //计数1秒后刷新蛇的状态
        {
            FlashTime=GameTime;
            RefreshSnake();
            if(isDead)  
            {
                flag=FALSE;
                goto out;
            }
            RefreshScreen();
        }
out:
    return flag;
}



void main()
{
    Section_Size=sizeof(struct Section);

    ConsoleSizeSet(InterfaceWidth+50,InterfaceHeight+4);

    SetConsoleOutputCP(437);        //显示大于127的ascii码(但是不能输出中文)

    BeginingScreen();

    PrintMap();

    CreateBody();

    FoodCreate();

    do
    {
        ;
    }while(keyscan());

    EndScreen();

}