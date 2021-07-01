#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include <TChar.h>

/****************************���Զ���******************************/


#define SIZE 100    //�������󳤶�
#define Border 20   //�߿�(��λ:����)
#define xNet 30     //x���������(0~29)
#define yNet 30     //y���������(0~29)
#define gap 3       //����֮��ļ��(��λ:����)
#define EdgeLenth 20    //����߳�(����gap)    

const int Width = xNet * EdgeLenth + 2 * Border;   //������
const int Height = yNet * EdgeLenth + 2 * Border;  //����߶�
const int winWidth = Width + 200;	//���ڿ��

int Drct = 3;         //���ƶ��ķ���:0,1,2,3�ֱ������������
clock_t GameTime, FlashTime;
long Timeing = CLOCKS_PER_SEC;

boolean Alive = TRUE;       //�ж��Ƿ�����
int Score;

//ʳ������
struct FoodInfo
{
	int x;
	int y;
}food;

//����Ϣ
struct snakeInfo
{
	int length;
	int x[SIZE];
	int y[SIZE];
}Snake;


//��ÿһ�ڵ���Ϣ(������������)
struct Section
{
	int num;                //�ڵı��
	int x;                  //�ڵ�x����
	int y;                  //�ڵ�y����
	int direction;          //�߽��˶�����
	struct Section* next;   //��һ�ڵĵ�ַ
}left;      //left����ԭ���һ�ڵ�����
			//���һ�ڵ�next���ӵ�left

struct Section* head;       //ָ���һ��

int Section_Size = sizeof(struct Section);    //Section���ڴ��С,���ڶ�̬������ 

struct Button   //��ť����Ϣ
{
	int x;
	int y;
	int width;
	int height;
	COLORREF color;
	TCHAR* pText;
};

/********************************************************/



/***************************����*****************************/

/***********ͼ����************/

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

	setcolor(color);
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


//�����ʼ��
void initWin(int width, int height, COLORREF bkcolor)
{
	initgraph(width, height);
	setbkcolor(bkcolor);
	cleardevice();
}

//��ֵת��
int toPixel(int Net)
{
	return Net * EdgeLenth + Border;
}

//��ӡ��Ԫ����
void SecPrint(struct Section* p, COLORREF color)
{
	setfillcolor(color);
	solidrectangle(toPixel(p->x) + gap, toPixel(p->y) + gap, toPixel(p->x) + Border - gap, toPixel(p->y) + Border - gap);
}

//��������
int EndScreen()
{
	setbkcolor(BLACK);
	cleardevice();
	TCHAR s[35];
	_tcscpy_s(s, 35, _T("Your Final Score Is: "));
	TCHAR n[10];
	_itow_s(Score, n, 10);
	n[9] = _T('\0');
	_tcscat_s(s, 45, n);
	setbkmode(1);
	settextcolor(WHITE);
	settextstyle(25, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s)) / 2, (Height - textheight(s)) / 2, s);

	ClickCheck(winWidth / 2 - 60, Height / 2 + 40, 120, 40, _T("Restart"), WHITE);
	return 1;
}

void ShowTitle()
{
	const TCHAR* s = _T("SNAKE");

	settextcolor(WHITE);
	settextstyle(50, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s)) / 2, 60, s);
}

void Showversion()
{
	const TCHAR* s = _T("v2.0.1");
	settextcolor(WHITE);
	settextstyle(20, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s)) / 2, 120, s);
}

void ShowRules()
{
	const TCHAR* s1= _T("Each step earns 1 point.");
	const TCHAR* s2= _T("Food earns 10 points.");

	settextcolor(WHITE);
	settextstyle(23, 0, _T("Consolas"));
	outtextxy((winWidth - textwidth(s1)) / 2, (Height - textheight(s1)) / 2 + 30, s1);
	outtextxy((winWidth - textwidth(s2)) / 2, (Height - textheight(s2)) / 2 + 50, s2);
}

//������Ϸ�߿�
void PrintMap()
{
	setfillcolor(BLACK);
	solidrectangle(Border, Border, Width - Border, Height - Border);
}

//��ʾ����
void ScoreShow(int x)
{
	Score += x;
	TCHAR s[10];
	_itow_s(Score, s, 10);
	outtextxy(Width + Border + (200 - textwidth(s)) / 2, (Height - textheight(s)) / 2, s);
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

	p = head;

	i = 0;
	do
	{
		a = rand() % (xNet - 1) + 1;
		b = rand() % (yNet - 1) + 1;
		if (i == Snake.length - 1)
			sign = FALSE;
		if (a == p->x || b == p->y)
		{
			sign = TRUE;
		}
		else
		{
			i++;
			p = p->next;
		}
	} while (sign);

	food.x = a;
	food.y = b;

	//������Ҫ��ӡʳ��
	setfillcolor(0xCCD5F0);
	solidcircle(toPixel(a) + EdgeLenth / 2, toPixel(b) + EdgeLenth / 2, EdgeLenth / 2 - 4);
}

void PrintFood()
{
	setfillcolor(0xCCD5F0);
	solidcircle(toPixel(food.x) + EdgeLenth / 2, toPixel(food.y) + EdgeLenth / 2, EdgeLenth / 2 - 2);
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
	p->num = 0; p->direction = 3; p->x = w; p->y = h;
	SecPrint(p, WHITE);

	//�ڶ���
	p->next = (Section*)malloc(Section_Size);
	p = p->next;
	p->num = 1; p->direction = 3; p->x = w - 1; p->y = h;
	SecPrint(p, WHITE);

	//������
	p->next = (Section*)malloc(Section_Size);
	p = p->next;
	p->num = 2; p->direction = 3; p->x = w - 2; p->y = h;
	SecPrint(p, WHITE);


	p->next = &left;

	Snake.length = 3;
	//left��
	left.x = w - 3; left.y = h;
	left.next = NULL; left.num = Snake.length;

}

//����ɳ�
void GrowUp()
{
	struct Section* p;
	p = head;

	//��ʱpָ���ߵ�β��
	do
	{
		p = p->next;
	} while (p->num != Snake.length - 1);

	p->next = (struct Section*)malloc(Section_Size);
	p = p->next;
	*p = left;
	p->next = &left;
	p->num = Snake.length;
	Snake.length++;
	left.num = Snake.length;

	int count;
	//���ٶȸı�
	count = (Snake.length - 3);
	Timeing = (long)(CLOCKS_PER_SEC / ((-pow(exp(1.0), -count / 14.0) + 1) * 2.5 + 1.25));
}

//ˢ���ߵ�״̬
void RefreshSnake()
{
	int i, dir, t;
	boolean flag = FALSE;
	static boolean sign = FALSE;       //sign��¼ʳ�ﱻ��ʱleft�Ƿ񱻴���

	struct Section* p;
	p = head;
	dir = Drct;

	for (i = 0; i <= Snake.length; i++)    //i=lengthʱpָ�����left
	{
		if (p->num == Snake.length && sign)
		{
			sign = FALSE;
			break;
		}

		t = p->direction;
		p->direction = dir;
		dir = t;                      //dir��ʾԭ�õ㴢��ķ���

		//�˴�ֱ�Ӹ���ÿ���ڵ�����  
		switch (p->direction)
		{
		case 0: p->y -= 1; break;

		case 1: p->y += 1; break;

		case 2: p->x -= 1; break;

		case 3: p->x += 1; break;
		}

		if (i != Snake.length)
			p = p->next;
	}

	//���ͷ���Ƿ�Ե�ʳ��
	if (head->x == food.x && head->y == food.y)
	{
		sign = TRUE;
		flag = TRUE;

		//����ɳ�
		GrowUp();
		FoodCreate();
		ScoreShow(10);
	}

	SecPrint(head, WHITE);
	if (!flag)
		SecPrint(&left, BLACK);
	else
		flag = FALSE;

	//����Ƿ�ײǽ
	if (head->x < 0 || head->x >= xNet)
		Alive = FALSE;
	else if (head->y < 0 || head->y >= yNet)
		Alive = FALSE;

	p = head->next;
	//����Ƿ�ײ������
	for (i = 1; i < Snake.length; i++)
	{
		if (head->x == p->x && head->y == p->y)
		{
			Alive = FALSE;
			break;
		}

		p = p->next;
	}

	if (Alive)
		ScoreShow(1);

}



//��������,����0��ʾ���벻�Ϸ�
void keyscan()
{
	char ch;

	if (_kbhit())
	{
		ch = _getch();

		switch (ch)
		{
		case 72:    Drct = 0; break;   //0--����

		case 80:    Drct = 1; break;   //1--����

		case 75:    Drct = 2; break;   //2--����

		case 77:    Drct = 3; break;   //3--����

		case 32:
			Sleep(300);
			while (_getch() != 32);
			Sleep(200);
		}
	}


	GameTime = clock();
	if ((GameTime - FlashTime) >= Timeing)    //����1���ˢ���ߵ�״̬
	{
		FlashTime = GameTime;
		RefreshSnake();
	}
}


/***************************************************************/

int main()
{

	//reset all of the codes of Map_priting
	initWin(winWidth, Height, BLACK);

	//load bar
	cleardevice();
	//bar_border
	rectangle(winWidth / 2 - 53, Height / 2 - 23, winWidth / 2 + 53, Height / 2 + 23);
	//bar
	for (int i = 0; i < 100; i++)
	{
		line(winWidth / 2 - 50 + i, Height / 2 - 20, winWidth / 2 - 50 + i, Height / 2 + 20);
		Sleep(40);
	}

Restart:
	//variable init
	Score = 0;

	Sleep(300);

	//Begining Screen
	cleardevice();
	ShowTitle();
	Showversion();
	ShowRules();

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
	PrintFood();
	FlashTime = clock();

	//show score
	outtextxy(Width + Border + (200 - textwidth(_T("score:"))) / 2, Height / 2 - 30, _T("score:"));
	setbkmode(OPAQUE);
	settextstyle(25, 0, _T("Consolas"));
	ScoreShow(0);

	FoodCreate();

	while (Alive)
	{
		keyscan();
	}

	if (EndScreen())
		goto Restart;

	return 0;
}