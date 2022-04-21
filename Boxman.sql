create database boxman;

use boxman;

CREATE TABLE `users` (
  `id` int NOT NULL DEFAULT '1',
  `user_name` varchar(64) NOT NULL,
  `password` varchar(32) NOT NULL,
  `level_id` int DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `levels` (
  `id` int NOT NULL DEFAULT '1',
  `name` varchar(64) NOT NULL,
  `map_row` int NOT NULL,
  `map_column` int NOT NULL,
  `map_data` varchar(4096) NOT NULL,
  `next_level_id` int DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

insert into users values
(1, 'man', md5(md5(123456)), 1);

insert into levels values
(
0, 
'关卡循环', 
3, 
3, 
'0,0,0,
 0,3,0,
 0,0,0,
 ', 
 1
),
(
1, 
'简单-1', 
7, 
7, 
'0,0,0,0,0,0,0,
 0,0,0,2,0,0,0,
 0,0,0,4,0,0,0,
 0,2,4,3,4,2,0,
 0,0,0,4,0,0,0,
 0,0,0,2,0,0,0,
 0,0,0,0,0,0,0,
 ', 
 2
),
(
2, 
'简单-2', 
9, 
12, 
'0,0,0,0,0,0,0,0,0,0,0,0,
 0,1,0,1,1,1,1,1,1,1,0,0,
 0,1,4,1,0,2,1,0,2,1,0,0,
 0,1,0,1,0,1,0,0,1,1,1,0,
 0,1,0,2,0,1,1,4,1,1,1,0,
 0,1,1,1,0,3,1,1,1,4,1,0,
 0,1,2,1,1,4,1,1,1,1,1,0,
 0,1,0,0,1,0,1,1,0,0,1,0,
 0,0,0,0,0,0,0,0,0,0,0,0, ', 
 0
 );