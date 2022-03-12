#pragma once
#include<string>

// 数据库连接参数
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_USER_PASSWD "123456"
#define DB_NAME "box_man"
#define DB_PORT 3306
#define DB_UNIX_SOCKET NULL 
#define DB_CLIENT_FLAG 0
#define CHARSER "utf8"

// 本地地图最大容量
#define LINE 45
#define COLUMN 45

namespace BoxMan
{
	using namespace std;

	typedef struct _userinfo
	{
		int id;					// 用户id
		string username;		// 用户名
		string password;		// 密码
		int level_id;			// 当前关卡id
		_userinfo() :id(0), username(string("")),
			password(string("")), level_id(0) {};
	}userinfo;

	typedef struct _levelinfo
	{
		int id;					// 关卡id
		string name;			// 关卡名
		int map_row;			// 地图行数
		int map_column;			// 地图列数
		string map_data;		// 关卡数据
		int next_level_id;		// 下一关卡id
		_levelinfo() :id(0), name(string("")), map_row(0),
			map_column(0), map_data(string("")), next_level_id(0) {};
	}levelinfo;

	bool fetch_user_info(userinfo& user);			// 获取用户数据
	bool fetch_level_info(levelinfo& level,
		int level_id);								// 获取关卡数据
	bool transform_map_b2array(levelinfo level,
		int map[LINE][COLUMN]);						// 加载关卡数据到map中
	bool push_user_info(userinfo& user);			// 修改用户当前关卡数据

}




