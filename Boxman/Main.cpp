/*
* ģ�黮�֣�
* 1.��ͼ��ʼ��
* 2.�ȼ�����
* 3.�����ӿ���
* 4.��Ϸ����
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>  // easyxͼ�ο�
#include <conio.h>     // ����̨�������


using namespace std;


#define RATIO 61

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 550

#define RATIO 50  // ����ͼ�η�������

#define LINE 9
#define COLUMN 12

#define START_X 100  // ע��easexͼ�ο��x���Ǻ���ģ�y���������
#define START_Y 50

#define KEY_UP    'w'
#define KEY_LEFT  'a'
#define KEY_RIGHT 'd'
#define KEY_DOWN  's'
#define KEY_QUIT  'q'

#define isValid(pos) pos.x >= 0 && pos.x < COLUMN && pos.y >= 0 && pos.y < LINE

typedef struct _POS
{
	int x;
	int y;
}POS;

enum _PROPS
{
	WALL,    // ǽ
	FLOOR,   // �ذ�
	BOX_DES, // ����Ŀ�ĵ�
	MAN,     // С��
	BOX,     // ����
	PROPS_NUM
};

IMAGE images[PROPS_NUM];  // ����ͼ����
POS man;                  // ��¼��������λ��
bool reprint_box;         // �����Ŀ�ĵ��뿪ʱҪ���»���Ŀ�ĵ�
int des_count = 4;         // ʣ��Ŀ����

// ǽ��0���ذ壺1������Ŀ�ĵأ�2��С�ˣ�3�����ӣ�4����������Ŀ�꣺5
int map[LINE][COLUMN] =
{
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,1,1,1,1,1,1,0,0},
	{0,1,4,1,0,2,1,0,2,1,0,0},
	{0,1,0,1,0,1,0,0,1,1,1,0},
	{0,1,0,2,0,1,1,4,1,1,1,0},
	{0,1,1,1,0,3,1,1,1,4,1,0},
	{0,1,2,1,1,4,1,1,1,1,1,0},
	{0,1,0,0,1,0,1,1,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};


bool operator== (POS p1, POS p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}


void changeMap(POS* pos, _PROPS prop)
{
	map[pos->y][pos->x] = prop;
	putimage(START_X + pos->x * RATIO, START_Y + pos->y * RATIO, &images[prop]);
}


// ��Ϸ����������������-�жϷ����Ƿ����-�޸ĵ�ͼ��Ӧλ�ü�¼
void gameControl(char direct)
{
	POS next_pos=man;
	POS next_next_pos=man;

	switch (direct)
	{
	case KEY_UP:
		next_pos.y -=1;
		next_next_pos.y -=2;
		break;
	case KEY_DOWN:
		next_pos.y +=1;
		next_next_pos.y +=2;
		break;
	case KEY_LEFT:
		next_pos.x -=1;
		next_next_pos.x -=2;
		break;
	case KEY_RIGHT:
		next_pos.x +=1;
		next_next_pos.x +=2;
		break;
	default:
		return;
	}

	// ����˵�ǰ���ǵذ�
	if (isValid(next_pos) && map[next_pos.y][next_pos.x]==FLOOR)
	{
		if (reprint_box)
		{
			changeMap(&man, BOX_DES);
			reprint_box = false;
		}
		else
		{
			changeMap(&man, FLOOR);
		}
		changeMap(&next_pos, MAN);
		man = next_pos;
	}

	// ����˵�ǰ����Ŀ�ĵ�
	if (isValid(next_pos) && map[next_pos.y][next_pos.x] == BOX_DES)
	{
		if (reprint_box)
		{
			changeMap(&man, BOX_DES);
			reprint_box = false;
		}
		else
		{
			changeMap(&man, FLOOR);
		}
		changeMap(&next_pos, MAN);
		man = next_pos;
		reprint_box = true;
	}

	// ����˵�ǰ��������
	if (isValid(next_pos) && map[next_pos.y][next_pos.x] == BOX)
	{
		if (map[next_next_pos.y][next_next_pos.x] == FLOOR)
		{
			if (reprint_box)
			{
				changeMap(&man, BOX_DES);
				reprint_box = false;
			}
			else
			{
				changeMap(&man, FLOOR);
			}
			changeMap(&next_pos, MAN);
			changeMap(&next_next_pos, BOX);
			man = next_pos;
		}
		if (map[next_next_pos.y][next_next_pos.x] == BOX_DES)
		{
			if (reprint_box)
			{
				changeMap(&man, BOX_DES);
				reprint_box = false;
			}
			else
			{
				changeMap(&man, FLOOR);
			}
			changeMap(&next_pos, MAN);
			changeMap(&next_next_pos, WALL);
			des_count--;
			man = next_pos;
		}
	}
	
	// ��ǰ����ǽʱ����������

}


int main(void)
{
	IMAGE bg_image;

	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);  // ��ʼ��ͼ�δ���

	loadimage(&bg_image, _T("blackground.bmp"), SCREEN_WIDTH, SCREEN_HEIGHT, true);
	putimage(0, 0, &bg_image);  // ���ر���

	// ����С���ߵ�ͼ��
	loadimage(&images[WALL], _T("wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("box.bmp"), RATIO, RATIO, true);

	// ��ʾ��ͼ�����
	for (int y = 0; y < LINE; y++)
	{
		for (int x = 0; x < COLUMN; x++)
		{
			if (map[y][x] == MAN)  // �����˵�λ��
			{
				man.x = x;
				man.y = y;
			}
			putimage(START_X + x * RATIO, START_Y + y * RATIO, &images[map[y][x]]);
		}
	}

	// ��Ϸѭ��
	bool quit = false;
	while (!quit)
	{
		if (des_count == 0)
		{
			printf("you win !");
			break;
		}
		if (_kbhit()) // ����������ʱ
		{
			char ch = _getch();
			switch (ch)
			{
			case KEY_UP:
				 gameControl(KEY_UP);
				break;
			case KEY_DOWN:
				 gameControl(KEY_DOWN);
				break;
			case KEY_LEFT:
				 gameControl(KEY_LEFT);
				break;
			case KEY_RIGHT:
				 gameControl(KEY_RIGHT);
				break;
			case KEY_QUIT:
				quit = true;
				printf("you lose !");
				break;
			default:
				break;
			}
		}
	}
	

	closegraph();


	system("pause");
	return 0;
}


