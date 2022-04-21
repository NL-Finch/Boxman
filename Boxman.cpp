#include <stdlib.h>
#include <stdio.h>
#include <conio.h> 
#include <graphics.h>  
#include <iostream>
#include <string>

#include "DataBase.h"
#include "Boxman.h"

#define isValid(pos) pos.x >= 0 && pos.x < level.map_column && pos.y >= 0 && pos.y < level.map_row

namespace Boxman
{
	using namespace std;

	const int MAX_LOGIN_TINES = 5;	// 登录最大重试数

	int map[LINE][COLUMN] = { 0 };	// 地图二维数组
	int map_row = 0;				// 地图行数
	int map_column = 0;				// 地图列数

	IMAGE bg_image;					// 背景图片
	IMAGE images[PROPS_NUM];		// 资源图片
	userinfo user;					// 用户信息
	levelinfo level;				// 关卡信息

	POS man;						// 人物当前位置		
	int MAX_STEPS = 100;			// 最大步数
	int steps = 0;					// 当前剩余步数
	bool reprint_box;				// 重绘目的地标志
	int des_count = 4;				// 剩余目标数

	bool operator== (POS p1, POS p2)
	{
		return p1.x == p2.x && p1.y == p2.y;
	}

	bool login(userinfo& user)
	{

		int times = MAX_LOGIN_TINES;
		while (times)
		{
			::system("cls");
			cout << "用户名: ";
			cin >> user.username;
			cout << "密码: ";
			cin >> user.password;

			if (fetch_user_info(user))
			{
				cout << "户数据获取成功！" << endl;
				printf("用户数据-> user_id: %d  level_id: %d\n", user.id, user.level_id);
				::system("pause");
				break;
			}
			else
			{
				cout << "用户数据获取失败！" << endl;
			}
			cout << "还有 " << --times << " 次重新输入机会" << endl;
			::system("pause");
		}
		if (times == 0)
		{
			return false;
		}
		return true;
	}

	void load_image()
	{
		initgraph(SCREEN_WIDTH, SCREEN_HEIGHT, SHOWCONSOLE);
		// 加载背景图片
		loadimage(&bg_image, _T("blackground.png"), SCREEN_WIDTH, SCREEN_HEIGHT, true);
		// 加载网格图片
		loadimage(&images[WALL], _T("wall_right.png"), RATIO, RATIO, true);
		loadimage(&images[FLOOR], _T("floor.png"), RATIO, RATIO, true);
		loadimage(&images[BOX_DES], _T("des.png"), RATIO, RATIO, true);
		loadimage(&images[MAN], _T("man.png"), RATIO, RATIO, true);
		loadimage(&images[BOX], _T("box.png"), RATIO, RATIO, true);
	}

	void load_level()
	{

		if (fetch_level_info(level, user.level_id))
		{
			cout << "关卡数据获取成功！" << endl;
			printf
			(
				"id: %d  name: %s  row: %d  column: %d  next_level_id: %d\n",
				level.id,
				level.name.c_str(),
				level.map_row,
				level.map_column,
				level.next_level_id
			);
		}
		else
		{
			cout << "关卡数据获取失败！" << endl;
			::system("pause");
			exit(-1);
		}
	}

	void load_map()
	{
		if (transform_map_b2array(level, map))
		{
			cout << "地图数据加载成功" << endl;
		}
		else
		{
			cout << "地图数据加载失败" << endl;
			::system("pause");
			exit(-1);
		}
	}

	void show_map()
	{
		putimage(0, 0, &bg_image);
		for (int y = 0; y < level.map_row; y++)
		{
			for (int x = 0; x < level.map_column; x++)
			{
				if (map[y][x] == MAN)
				{
					man.x = x;
					man.y = y;
				}
				putimage(START_X + x * RATIO, START_Y + y * RATIO, &images[map[y][x]]);
			}
		}
	}

	void changeMap(POS* pos, _PROPS prop)
	{
		map[pos->y][pos->x] = prop;
		putimage(START_X + pos->x * RATIO, START_Y + pos->y * RATIO, &images[prop]);
	}

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
			steps++;
		}


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
			steps++;
		}


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
			steps++;
		}

	}

	bool gameNextScene()
	{
		putimage(0, 0, &bg_image);
		settextcolor(GREEN);
		RECT rec = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
		settextstyle(40, 0, _T("黑体"));
		drawtext(_T("此关已经通关，任意键跳转到下一关！"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		::system("pause");
		cleardevice();


		user.level_id = level.next_level_id;
		load_level();
		load_map();
		show_map();
		if (user.level_id != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void run_game()
	{
		// 登录并获取用户数据
		if (!login(user))
		{
			exit(-1);
		}

		// 加载图像资源
		load_image();

		// 获取关卡数据
		load_level();

		// 将关卡数据加载到map中
		load_map();

		// 显示地图
		show_map();

		// 游戏循环
		bool quit = false;
		cout << "按q键退出游戏！" << endl;
		cout << "最大步数为：" << MAX_STEPS << endl;
		while (1)
		{
			if (level.id == 0)
			{
				char tmp;
				do
				{
					cout << "已完成本周目是否重新开始？(y/n)" << endl;
					cin >> tmp;

				} while (tmp != 'y' && tmp != 'n');
				if (tmp == 'y')
				{
					des_count = 4;
					steps = 0;
					gameNextScene();
				}
				else
				{
					quit = true;
				}
			}
			if (_kbhit())
			{
				cout << "已经走了" << steps << "步" << endl;
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
					break;
				default:
					break;
				}
			}
			if (des_count == 0)
			{
				des_count = 4;
				steps = 0;
				gameNextScene();
			}
			if (steps == MAX_STEPS)
			{
				char tmp = ' ';

				cout << "已达到最大步数！！" << endl;
				do
				{
					cout << "是否重新开始游戏(y/n)" << endl;
					cin >> tmp;

				} while (tmp != 'y' && tmp != 'n');
				if (tmp == 'y')
				{
					des_count = 4;
					steps = 0;
					cleardevice();
					load_map();
					show_map();
				}
				else
				{
					quit = true;
				}
			}
			if (quit)
			{
				if (push_user_info(user))
				{
					printf("用户信息保存成功！\n");
				}
				printf("退出游戏...\n");
				break;
			}
			Sleep(100);
		}

		// 退出游戏
		closegraph();  // 关闭图形库
		system("pause");
	}
}


int main(void)
{
	::system("chcp 65001");
	Boxman::run_game();
	return 0;
}


