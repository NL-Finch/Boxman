#pragma once

#define SCREEN_WIDTH 800		// 窗口宽度
#define SCREEN_HEIGHT 550		// 窗口宽度

#define RATIO 50				// 道具图形放缩比例

#define START_X 100				// 注意easex图形库的x轴是横向的，y轴是纵向的
#define START_Y 50

#define KEY_UP    'w'			// 操作按键绑定
#define KEY_LEFT  'a'
#define KEY_RIGHT 'd'
#define KEY_DOWN  's'
#define KEY_QUIT  'q'

namespace Boxman
{
	typedef struct _POS	// 平面位置
	{
		int x;
		int y;
	}POS;

	enum _PROPS			// 图形资源类型
	{
		WALL,			// 墙
		FLOOR,			// 地板
		BOX_DES,		// 箱子目的第
		MAN,			// 小人
		BOX,			// 箱子
		PROPS_NUM		// 自动计数项
	};
}

void run_game();
