/*
* 模块划分：
* 1.地图初始化
* 2.热键控制
* 3.推箱子控制
* 4.游戏结束
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>  // easyx图形库
#include <conio.h>     // 控制台输入输出


using namespace std;


#define RATIO 61

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 550

#define RATIO 50  // 道具图形放缩比例

#define LINE 9
#define COLUMN 12

#define START_X 100  // 注意easex图形库的x轴是横向的，y轴是纵向的
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
	WALL,    // 墙
	FLOOR,   // 地板
	BOX_DES, // 箱子目的第
	MAN,     // 小人
	BOX,     // 箱子
	PROPS_NUM
};

IMAGE images[PROPS_NUM];  // 道具图形类
POS man;                  // 记录人物所在位置
bool reprint_box;         // 人物从目的地离开时要重新绘制目的地
int des_count = 4;         // 剩余目的数

// 墙：0，地板：1，箱子目的地：2，小人：3，箱子：4，箱子命中目标：5
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


// 游戏控制器：接受输入-判断方向是否可行-修改地图对应位置记录
void gameControl(char direct)
{
	POS next_pos = man;
	POS next_next_pos = man;

	switch (direct)
	{
	case KEY_UP:
		next_pos.y -= 1;
		next_next_pos.y -= 2;
		break;
	case KEY_DOWN:
		next_pos.y += 1;
		next_next_pos.y += 2;
		break;
	case KEY_LEFT:
		next_pos.x -= 1;
		next_next_pos.x -= 2;
		break;
	case KEY_RIGHT:
		next_pos.x += 1;
		next_next_pos.x += 2;
		break;
	default:
		return;
	}

	// 如果人的前面是地板
	if (isValid(next_pos) && map[next_pos.y][next_pos.x] == FLOOR)
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

	// 如果人的前面是目的地
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

	// 如果人的前面是箱子
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

	// 人前面是墙时，函数结束

}


int main(void)
{
	IMAGE bg_image;

	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);  // 初始化图形窗口

	loadimage(&bg_image, _T("blackground.bmp"), SCREEN_WIDTH, SCREEN_HEIGHT, true);
	putimage(0, 0, &bg_image);  // 加载背景

	// 加载小道具的图像
	loadimage(&images[WALL], _T("wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("box.bmp"), RATIO, RATIO, true);

	// 显示地图与道具
	for (int y = 0; y < LINE; y++)
	{
		for (int x = 0; x < COLUMN; x++)
		{
			if (map[y][x] == MAN)  // 更新人的位置
			{
				man.x = x;
				man.y = y;
			}
			putimage(START_X + x * RATIO, START_Y + y * RATIO, &images[map[y][x]]);
		}
	}

	// 游戏循环
	bool quit = false;
	while (!quit)
	{
		if (des_count == 0)
		{
			printf("you win !");
			break;
		}
		if (_kbhit()) // 当按键按下时
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


