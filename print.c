#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#include "snake.h"

//移动光标位置
void gotoposition(int x,int y)
{
    COORD coord;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(hOutput,coord);

    //隐藏光标
    CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;    
	cursor.dwSize = sizeof(cursor);
	SetConsoleCursorInfo(hOutput, &cursor);
}

//在指定位置打印黑方块
void gotoprint(int x,int y)
{
    gotoposition(x,y);
    putchar(254);
    gotoposition(0,0);
}


//绘制游戏边框
void PrintMap()
{
    int i;

    for(i=1;i<=InterfaceWidth+3;i+=2)
    {
        gotoposition(i,1);
        putchar(254);

        gotoposition(i,InterfaceHeight+2);
        putchar(254);
    }


    for(i=2;i<InterfaceHeight+2;i++)
    {
        gotoposition(1,i);
        putchar(254);

        gotoposition(InterfaceWidth+3,i);
        putchar(254);
    } 

}


//显示分数
void ScoreShow()
{
    gotoposition(InterfaceWidth+15,(InterfaceHeight+4)/2-3);
    printf("==========");
    gotoposition(InterfaceWidth+15,(InterfaceHeight+4)/2);
    printf("Score:%4d",Snake.length-3);
    gotoposition(InterfaceWidth+15,(InterfaceHeight+4)/2+3);
    printf("==========");
}