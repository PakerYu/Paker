#include "Snake.h"

//????????
void initWin(int width, int height, COLORREF bkcolor)
{
	initgraph(width, height);
	setbkcolor(bkcolor);
	cleardevice();
}


//??????
void PrintFood()
{
	setfillcolor(0xCCD5F0);
	solidcircle(toPixel(food.x) + EdgeLenth / 2, toPixel(food.y) + EdgeLenth / 2, EdgeLenth / 2 - 2);
}


//??????????
void PrintMap()
{
	setfillcolor(BLACK);
	solidrectangle(Border, Border, Width - Border, Height - Border);
}

//???????
void ScoreShow()
{
	TCHAR s[10];
	_itot(Snake.score, s, 10);
	outtextxy(Width + Border + (200 - textwidth(s)) / 2, (Height - textheight(s)) / 2, s);
}

//????????
int EndScreen()
{
	setbkcolor(BLACK);
	cleardevice();
	TCHAR s[35];
	_tcscpy_s(s, 34, _T("Your Final Score Is: "));
	TCHAR n[10];
	_itot(Snake.score, n, 10);
	n[9] = _T('\0');
	_tcscat_s(s, 34, n);
	setbkmode(1);
	settextcolor(WHITE);
	settextstyle(25, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s)) / 2, (Height - textheight(s)) / 2, s);

	ClickCheck(winWidth / 2 - 60, Height / 2 + 40, 120, 40, _T("Restart"), WHITE);
	return 1;
}



//??????????
void SecPrint(struct Section* p, COLORREF color)
{
	setfillcolor(color);
	solidrectangle(p->Gx + Gap, p->Gy + Gap, p->Gx + Border - Gap, p->Gy + Border - Gap);
}


/************************?????????????***************************/

//???????
void showTitle()
{
	const TCHAR* s = _T("SNAKE");

	settextcolor(WHITE);
	settextstyle(50, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s)) / 2, 60, s);
}

//??????
void showVersion()
{
	const TCHAR* s = _T("v2.1.1");
	settextcolor(WHITE);
	settextstyle(20, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s)) / 2, 120, s);
}

//???????
void showRules()
{
	const TCHAR* s1 = _T("Each step earns 1 point.");
	const TCHAR* s2 = _T("Food earns 5 points.");

	settextcolor(WHITE);
	settextstyle(23, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s1)) / 2, (Height - textheight(s1)) / 2 + 30, s1);
	outtextxy((winWidth - textwidth(s2)) / 2, (Height - textheight(s2)) / 2 + 50, s2);
}

//?????????
void loadBar()
{
	cleardevice();
	//bar_border
	rectangle(winWidth / 2 - 53, Height / 2 - 23, winWidth / 2 + 53, Height / 2 + 23);
	//bar
	for (int i = 0; i < 100; i++)
	{
		line(winWidth / 2 - 50 + i, Height / 2 - 20, winWidth / 2 - 50 + i, Height / 2 + 20);
		Sleep(20);
	}
}

//??????????????????
void added_Section_Print(struct Section* new_section, COLORREF color)
{
	int edge = Anime.anime_freshed_number < 2 * Gap ?
		0 : (Anime.anime_freshed_number - 2 * Gap) * Anime.pixel_pre_step;

	setfillcolor(color);
	switch (new_section->direction)
	{
	case UP:
		solidrectangle(
			new_section->Gx + Gap,
			new_section->Gy + EdgeLenth - edge - Gap,
			new_section->Gx + EdgeLenth - Gap,
			new_section->Gy + EdgeLenth - Gap);
		break;
	case DOWN:
		solidrectangle(
			new_section->Gx + Gap,
			new_section->Gy + Gap,
			new_section->Gx + EdgeLenth - Gap,
			new_section->Gy + Gap + edge);
		break;
	case LEFT:
		solidrectangle(
			new_section->Gx + EdgeLenth - Gap - edge,
			new_section->Gy + Gap,
			new_section->Gx + EdgeLenth - Gap,
			new_section->Gy + EdgeLenth - Gap);
		break;
	case RIGHT:
		solidrectangle(
			new_section->Gx + Gap,
			new_section->Gy + Gap,
			new_section->Gx + Gap + edge,
			new_section->Gy + EdgeLenth - Gap);
	}
}

//????????, ????????????????????????
void print_Verify(struct Section* end_section)
{
	struct Section* p = end_section;
	while (1)
	{
		p->Gx = toPixel(p->x);
		p->Gy = toPixel(p->y);
		SecPrint(p, WHITE);
		if (p->pre == NULL)
			break;

		PreSection(p);
	}
}