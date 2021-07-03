
#include "Snake2_1_0.h"


/********************************************************/

/***************************����*****************************/


//���ɰ�ť
struct Button* createBT(int x, int y, int width, int height, COLORREF color, const TCHAR* text)
{
	struct Button* pB = (struct Button*)malloc(sizeof(struct Button));
	pB->x = x;
	pB->y = y;
	pB->width = width;
	pB->height = height;
	pB->color = color;
	pB->pText = (TCHAR*)malloc(_tcslen(text) + 1);
	_tcscpy_s(pB->pText, _tcslen(text) + 1, text);

	setbkcolor(color);
	setbkmode(TRANSPARENT);
	setfillcolor(color);
	solidrectangle(x, y, x + width, y + height);

	settextcolor(BLACK);
	settextstyle(25, 0, _T("Consolas"));
	outtextxy(x + (width - textwidth(text)) / 2, y + (height - textheight(text)) / 2, text);

	return pB;
}

//��ť�����
int clickBT(struct Button* bt, MOUSEMSG m)
{
	if (m.x > bt->x && m.x<bt->x + bt->width && m.y>bt->y && m.y < bt->y + bt->height)
		if (m.uMsg == WM_LBUTTONDOWN)
			return 0;
	return 1;
}

void ClickCheck(int x, int y, int width, int height, const TCHAR* s, COLORREF color)
{
	struct Button* start = createBT(x, y, width, height, color, s);

	//ButtonCheck
	while (clickBT(start, GetMouseMsg()))
	{
		;
	}
}


//����ʳ��
void FoodCreate()
{
	int i;
	int a, b;
	struct Section* p;
	boolean sign = TRUE;

	//���ԭʳ��
	setfillcolor(BLACK);
	solidrectangle(toPixel(food.x), toPixel(food.y), toPixel(food.x) + EdgeLenth, toPixel(food.y) + EdgeLenth);

	srand((int)time(NULL));

	i = 0;
	do
	{
		a = rand() % (xNet - 1) + 1;
		b = rand() % (yNet - 1) + 1;
		
		sign = FALSE;
		p = head;
		while (1)
		{
			if (p->x == a && p->y == b)
			{
				sign = TRUE;
				break;
			}
			if (p->next == NULL)
				break;

			NextSection(p);
		}
	} while (sign);

	food.x = a;
	food.y = b;

	//������Ҫ��ӡʳ��
	setfillcolor(0xCCD5F0);
	solidcircle(toPixel(a) + EdgeLenth / 2, toPixel(b) + EdgeLenth / 2, EdgeLenth / 2 - 4);
}


//��������
void CreateBody()
{
	struct Section* p;
	int w, h;

	w = (int)(xNet / 2);
	h = (int)(yNet / 2);

	//��һ��
	head = (Section*)malloc(Section_Size);
	p = head;
	p->pre = NULL;
	p->num = 0; p->direction = RIGHT; p->x = w; p->y = h;
	p->Gx = toPixel(p->x);
	p->Gy = toPixel(p->y);
	SecPrint(p, WHITE);

	//�ڶ���
	p->next = (Section*)malloc(Section_Size);
	p->next->pre = p;
	NextSection(p);
	p->num = 1; p->direction = RIGHT; p->x = w - 1; p->y = h;
	p->Gx = toPixel(p->x);
	p->Gy = toPixel(p->y);
	SecPrint(p, WHITE);

	//������
	p->next = (Section*)malloc(Section_Size);
	p->next->pre = p;
	NextSection(p);
	tail = p;
	p->num = 2; p->direction = RIGHT; p->x = w - 2; p->y = h;
	p->Gx = toPixel(p->x);
	p->Gy = toPixel(p->y);
	SecPrint(p, WHITE);

	p->next = (struct Section*)malloc(Section_Size);
	left = p->next;

	Snake.length = 3;
	//left��
	left->num = Snake.length;
	left->direction = RIGHT;
	left->x = w - 3; left->y = h;
	left->Gx = NULL; left->Gy = NULL;
	left->pre = p;
	left->next = NULL; left->num = Snake.length;
}


//ˢ���ߵ�״̬
int RefreshSnake()
{
	/*
	* eaten_flag ���ڼ�¼�Ƿ�Ե�ʳ��,
	* ���Ե�ʳ����Ϊ 1
	* �����ж�left�ڵ��Ƿ���Ҫ�ƶ�
	* ����һ��ˢ�º� left �ڵ��ƶ�, eaten_flag����Ϊ 0
	*/
	static int eaten_flag = 0;

	if (eaten_flag)
	{
		//�Ե�ʳ��
		
		directInherit(tail);
		coordinatesRefresh(tail);
		eaten_flag = 0;
	}
	else
	{
		directInherit(left);
		coordinatesRefresh(left);
	}

	if (foodCheck() == 1)		//���Ե�ʳ��
	{
		CreateSection(-1);
		eaten_flag = 1;
	}

	if (crashCheck() == 1)
	{
		Snake.alive = FALSE;
	}

	return eaten_flag;
}

//��������,����0��ʾ���벻�Ϸ�
int keyscan()
{
	char ch;
	int drct = -1;

	if (_kbhit())
	{
		ch = _getch();

		switch (ch)
		{
		case 72:    drct = UP; break;   //0--����

		case 80:    drct = DOWN; break;   //1--����

		case 75:    drct = LEFT; break;   //2--����

		case 77:    drct = RIGHT; break;   //3--����

		case 32:		//��ͣ
			EndBatchDraw();
			Sleep(300);
			while (_getch() != 32);		//�ȴ�����ո�
			Sleep(200);
			BeginBatchDraw();
		}
	}

	return drct;
}

/**
 * @Description  : ��� ����, �����β��
 * @Author       : Paker
 * @Time         : 2021-07-03 14:16:50
 * 
 * @param         {*}
 * @return        {*}
 */
void Clear_Data()
{
    struct Section *section_p=tail;

    // �ͷ�left�Ŀռ�
    free(left);

    do
    {
        PreSection(tail);
        free(tail->next);
    } while (head != tail);

    free(tail);

    tail = NULL;
    head = NULL;
    left = NULL;
}

/***************************************************************/


int main()
{
	int food_eaten_sign;		//����Ƿ�Ե�ʳ��
	int drct;

	//��������
	initWin(winWidth, Height, BLACK);


	loadBar();

Restart:
	//����, �ṹ���ʼ��
	food_eaten_sign = 0;
	RefreshStructure_Init(&RefreshInfo);	//��ʼ��RefreshInfo(ˢ����Ϣ)
	SnakeStructure_Init(&Snake);
	AnimeStructure_Init(&Anime);

	Sleep(300);

	//Begining Screen
	setbkcolor(BLACK);
	cleardevice();
	showTitle();
	showVersion();
	showRules();

	ClickCheck(winWidth / 2 - 50, Height / 2 - 20, 100, 40, _T("Start"), WHITE);

	setbkcolor(WHITE);
	Sleep(400);
	cleardevice();

	//tips printing
	const TCHAR* stoptips = _T("press Space key to stop");
	settextstyle(17, 0, _T("Consolas"));
	outtextxy(Width + (200 - textwidth(stoptips)) / 2, Border, stoptips);

	PrintMap();
	CreateBody();
	NewTime = clock();
	PreTime = NewTime;

	//show score
	outtextxy(Width + Border + (200 - textwidth(_T("score:"))) / 2, Height / 2 - 30, _T("score:"));
	setbkmode(OPAQUE);
	settextstyle(25, 0, _T("Consolas"));
	ScoreShow();

	FoodCreate();

	
	timeStructure_Init();

	BeginBatchDraw();
	while (Snake.alive)
	{
		if ((drct = keyscan()) != -1)
			Snake.direction = drct;
		
		if (Anime.isaction)
		{
			if (timeOffset(&Screen_Time, RefreshInfo.period.screen))
			{
				if (Anime.anime_freshed_number <
					RefreshInfo.frequency)
				{
					PrintMap();
					if (food_eaten_sign)
					{
						pixel_Compute_Print(tail->pre);
						added_Section_Print(tail, WHITE);
					}
					else
					{
						pixel_Compute_Print(tail);
					}
					Anime.anime_freshed_number++;
					PrintFood();
				}
				else
				{
					/*Anime.anime_freshed_number>=
					RefreshInfo.frequency*/
					PrintMap();
					print_Verify(tail);
					PrintFood();
					Anime.isaction = FALSE;
					Snake.isstatus = TRUE;
					Anime.anime_freshed_number = 0;
				}
			}
			FlushBatchDraw();
		}
		if(Snake.isstatus)
		{
			//isaction=FALSE
			if (timeOffset(&Status_Time, RefreshInfo.period.status))
			{
				//�������״̬ˢ������, ִ��״̬ˢ�¶���
				if (food_eaten_sign=RefreshSnake())
				{
					Snake.score += 5;
					Period_Calculate(&RefreshInfo);
					FoodCreate();
				}
				else
				{
					Snake.score += 1;
				}
				Anime.isaction = TRUE;
				Snake.isstatus = FALSE;

				ScoreShow();
			}
		}
	}
	EndBatchDraw();

	if (EndScreen())
    {
        setbkcolor(BLACK);
        cleardevice();
        settextcolor(WHITE);
        settextstyle(25, 0, _T("Consolas"));
        outtextxy((winWidth - textwidth(_T("Loading..."))) / 2, (Height-textheight(_T("Loading...")))/2, _T("Loading..."));
        Clear_Data();   //���������
        Sleep(2000);
        goto Restart;
    }

	return 0;
}