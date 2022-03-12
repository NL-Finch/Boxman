#include <mysql.h>
#include <iostream>
#include "DataBase.h"

namespace BoxMan
{
	using namespace std;

	/********************************************
	* 功能：连接数据库
	* 输入：
	*        mysql - MySQL句柄
	* 返回值：
	*        连接成功返回true，失败返回false
	*
	*********************************************/
	static bool connect_db(MYSQL& mysql)
	{
		mysql_init(&mysql);    // 初始化数据库
		mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, CHARSER);  // 设置字符编码
		return mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWD,  // 连接数据库
			DB_NAME, DB_PORT, DB_UNIX_SOCKET, DB_CLIENT_FLAG) != NULL;

	}

	/********************************************
	* 功能：通过用户名和密码获取用户信息
	* 输入：
	*        user - 用户信息结构体
	* 返回值：
	*        获取成功返回true，失败返回false
	*
	*********************************************/
	bool fetch_user_info(userinfo& user)
	{
		MYSQL mysql;           // 数据库句柄
		MYSQL_RES* res;        // 查询结果集
		MYSQL_ROW row;         // 记录结构体
		char sql[256];         // SQL语句缓冲区
		bool ret = false;      // 查询状态

		// 1.连接数据库
		if (!connect_db(mysql))
		{
			printf("错误原因： %s\n", mysql_error(&mysql));
			printf("连接失败！\n");
			exit(-1);
		}

		// 2.根据用户名和密码获取用户信息（id，level_id）
		snprintf(sql, 256, "select id,level_id from users where user_name='%s' and password=md5('%s');",
			user.username.c_str(), user.password.c_str());  // 将查询语句加载到sql缓冲区中
		ret = mysql_query(&mysql, sql);  // 执行查询语句，成功返回0

		if (ret)
		{
			printf("数据库查询出错：%s \n错误原因：%s \n", sql, mysql_error(&mysql));
			mysql_close(&mysql);
			return false;
		}

		// 3.获取结果
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);

		if (row == NULL)  // 没有查询到记录
		{
			mysql_free_result(res);  // 释放结果集引用对象
			mysql_close(&mysql);  // 关闭数据库连接
			return false;
		}

		user.id = atoi(row[0]);
		user.level_id = atoi(row[1]);
		//if(debug)
		//	printf("user_id: %d  level_id: %d\n", user.id, user.level_id);

		// 4.返回结果
		mysql_free_result(res);  // 释放结果集引用对象
		mysql_close(&mysql);  // 关闭数据库连接
		return true;
	}

	/********************************************
	* 功能：通过用户当前关卡id获取关卡数据
	* 输入：
	*        levelinfo level - 关卡信息结构体
	*        leve_id - 用户当前关卡id
	* 返回值：
	*        获取成功返回true，失败返回false
	*
	*********************************************/
	bool fetch_level_info(levelinfo& level, int level_id)
	{
		MYSQL mysql;           // 数据库句柄
		MYSQL_RES* res;        // 查询结果集
		MYSQL_ROW row;         // 记录结构体
		char sql[256];         // SQL语句缓冲区
		bool ret = false;      // 查询状态

		// 1.连接数据库
		if (!connect_db(mysql))
		{
			printf("错误原因： %s\n", mysql_error(&mysql));
			printf("连接失败！\n");
			exit(-1);
		}

		// 2.根据用户名和密码获取用户信息（id，level_id）
		snprintf(sql, 256, "select * from levels where id='%s';", to_string(level_id).c_str());  // 将查询语句加载到sql缓冲区中
		ret = mysql_query(&mysql, sql);  // 执行查询语句，成功返回0

		if (ret)
		{
			printf("数据库查询出错：%s \n错误原因：%s \n", sql, mysql_error(&mysql));
			mysql_close(&mysql);
			return false;
		}

		// 3.获取结果
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);

		if (row == NULL)  // 没有查询到记录
		{
			mysql_free_result(res);  // 释放结果集引用对象
			mysql_close(&mysql);  // 关闭数据库连接
			return false;
		}

		level.id = atoi(row[0]);
		level.name = string(row[1]);
		level.map_row = atoi(row[2]);
		level.map_column = atoi(row[3]);
		level.map_data = string(row[4]);
		level.next_level_id = atoi(row[5]);

		//if (debug)
		//{
		//	printf
		//	(
		//		"id: %d  name: %s  row: %d  column: %d  next_level_id: %d\n",
		//		level.id,
		//		level.name.c_str(),
		//		level.map_row,
		//		level.map_column,
		//		level.next_level_id
		//	);
		//	system("pause");
		//}


		// 4.返回结果
		mysql_free_result(res);  // 释放结果集引用对象
		mysql_close(&mysql);  // 关闭数据库连接


		return true;
	}

	/********************************************
	* 功能：将关卡数据转换成二维数组
	* 输入：
	*        levelinfo level - 关卡信息结构体
	*        map - 数组指针
	* 返回值：
	*        获取成功返回true，失败返回false
	*
	*********************************************/
	bool transform_map_b2array(levelinfo level, int map[LINE][COLUMN])
	{
		if (level.map_column > COLUMN || level.map_row > LINE)
		{
			printf("地图超大，请重新设置！\n");
			return false;
		}

		if (level.map_data.length() < 1)
		{
			printf("地图数据有问题，请重新设置！\n");
			return false;
		}

		__int64 start = 0;
		__int64 end = 0;
		int i = 0;
		while (1)
		{
			end = level.map_data.find(' ', start);                    // 查找行尾空格所在位置
			if (end < 0)
			{
				end = level.map_data.length();
			}
			if (start >= end)break;                                   // 当start大于end时，行数据已全部取出

			string line = level.map_data.substr(start, end - start);  // 获取行数据
			printf("get line: %s\n", line.c_str());                   // 打印取出的行数据

			for (__int64 j = 0; j < level.map_column; j++)
			{
				map[i][j] = line[2 * j] - '0';                        // 将字符数据转换成int64存在map二维数组中
			}
			start = end + 1;                                          // 更新查找开始位置start，和map的行索引i
			i++;
		}
		return true;
	}

	/********************************************
	* 功能：将user的修改上传到数据库中
	* 输入：
	*        user - 用户信息结构体
	* 返回值：
	*        获取成功返回true，失败返回false
	*
	*********************************************/
	bool push_user_info(userinfo& user)
	{
		MYSQL mysql;           // 数据库句柄
		char sql[256];         // SQL语句缓冲区
		bool ret = false;      // 查询状态

		// 1.连接数据库
		if (!connect_db(mysql))
		{
			printf("错误原因： %s\n", mysql_error(&mysql));
			printf("连接失败！\n");
			exit(-1);
		}

		// 2.更新用户当前关卡id
		snprintf(sql, 256, "update users set level_id=%d where id=%d;", user.level_id, user.id);  // 将查询语句加载到sql缓冲区中
		ret = mysql_query(&mysql, sql);  // 执行查询语句，成功返回0
		if (ret)
		{
			printf("数据库查询出错：%s \n错误原因：%s \n", sql, mysql_error(&mysql));
			mysql_close(&mysql);
			return false;
		}

		// 释放连接
		mysql_close(&mysql);  // 关闭数据库连接
		return true;
	}

}
