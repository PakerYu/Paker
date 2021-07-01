#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#ifndef _SNAKE_H_
#define _SNAKE_H_

/*************************************************************/
//各类全局变量

#define SIZE 100    //蛇身的最大长度
#define InterfaceWidth 2*40  //界面宽度
#define InterfaceHeight 40  //界面高度

extern int Drct;
extern boolean Eatenflag;
extern clock_t GameTime,FlashTime;
extern int Timeing;

extern boolean isDead;


//食物坐标
struct Food
{
    int x;
    int y;
};

//蛇信息
struct Snake_Info
{
    int length;
    int x[SIZE];
    int y[SIZE];
};


//蛇每一节的信息(用于制作链表)
struct Section
{
    int num;                //节的编号
    int x;                  //节的x坐标
    int y;                  //节的y坐标
    int direction;          //蛇节运动方向
    struct Section *next;   //下一节的地址
};

extern struct Section *head, left;

extern struct Snake_Info Snake;

extern struct Food food;

extern int Section_Size;

/*******************************************************************************/

//函数声明
void ConsoleSizeSet();

void EndScreen();       //结束界面

void BeginingScreen();

void gotoposition(int x,int y);     //移动光标位置

void gotoprint(int x,int y);        //在指定位置打印黑方块

void PrintMap();                    //绘制游戏边框

void ScoreShow();                   //显示分数

void RefreshScreen();               //刷新屏幕

void FoodCreate();                  //生成食物

#endif