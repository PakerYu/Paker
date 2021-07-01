#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#include "snake.h"

//设置窗体大小
void ConsoleSizeSet(int w,int h)
{
    char stcmd[32];

    sprintf(stcmd,"mode con cols=%d lines=%d",w,h);

    system(stcmd);
}

//结束界面
void EndScreen()
{
    if(isDead)
    {
        system("cls");
        gotoposition((InterfaceWidth+50)/2-10,(InterfaceHeight+4)/2-2);
        printf("--------------------");
        gotoposition((InterfaceWidth+50)/2-5,(InterfaceHeight+4)/2);
        printf("Game Over!");
        gotoposition((InterfaceWidth+50)/2-10,(InterfaceHeight+4)/2+2);
        printf("--------------------");
        gotoposition((InterfaceWidth+50)/2-8,(InterfaceHeight+4)/2+4);
        printf("Your score is %d",Snake.length-3);
        gotoposition((InterfaceWidth+50)/2-10,InterfaceHeight);
        printf("Press any key to exit!");
        getch();
    }
}



//开始界面
void BeginingScreen()
{
    system("cls");
    gotoposition((InterfaceWidth+50)/2-8,(InterfaceHeight+4)/2-4);
    printf("GLUTTONOUS SNAKE");
    gotoposition((InterfaceWidth+50)/2-3,(InterfaceHeight+4)/2);
    putchar(254);putchar(254);putchar(254);

    gotoposition((InterfaceWidth+50)/2-10,InterfaceHeight);
    printf("Press any key to START!");

    getch();
}


//刷新界面
void RefreshScreen()
{
    int i;
    struct Section *p;
    p=head;
    system("cls");
    PrintMap();
    if(Eatenflag)
    {
        FoodCreate();
        Eatenflag=FALSE;
    }
    else
    {
        gotoposition(food.x,food.y);
        putchar('*');
    }

    
    for(i=0;i<Snake.length;i++)
    {
        gotoprint(p->x,p->y);

        p=p->next;
    }

    ScoreShow();
}