Version 6

服务器（Server）收到消息数据格式

注册
数据协议格式：1 用户名 密码
服务器回复：
0 成功
1 失败
2 服务器出错

登录
数据协议格式：2 用户名 用户密码
服务器回复：
0 用户未注册（不存在）
1 用户已在别处登录
2 登录成功
3 密码错误


发送群消息
数据协议格式：3 发送用户名 群聊名 消息内容
服务器回复：
转发给已加入群聊的在线用户

添加好友
数据协议格式：4 请求用户 被请求添加用户名
服务器回复：
0 目标用户不存在
1 已转发添加好友请求
2 已存在好友关系
3 目标用户不在线
4 发送者尚未登陆


 一对一聊天消息
数据协议格式：5 发送者（用户名） 接收者 消息内容
服务器回复：
0 目标用户不在线
1 与目标用户非好友关系

添加好友回复
数据协议格式：6 发送者（用户名） 接收者 回复内容
服务器回复：
服务器向请求和应答用户提示添加信息

获取用户所有好友用户名
数据协议格式：7 发送者（用户名）
服务器回复：
0 用户好友表不存在
应答成功数据格式：好友1 好友2...(每个用户名间隔一个空格)

更新在线套接字
数据协议格式：8 发送者（用户名）
服务器回复：
null

创建群聊
数据协议格式：9 创建者 群聊名称
服务器回复：
0 成功
1 群聊已存在
2 失败

加入群聊申请
数据协议格式：a 申请用户名 群聊名称
服务器回复：
0 群聊不存在
1 该用户已加入该群聊
2 群主不在线
3 请求发送成功

获取某个用户加入的所有群聊名称
数据协议格式：b 用户名
返回所有群聊名称（每个名称以一个空格间隔）

获取某个群聊所有的用户名
数据协议格式：c 群聊名称
返回该群聊所有用户用户名（每个名称以一个空格间隔）

加入群聊回复（群主在线拒绝或同意）
数据协议格式: d 群主 目标用户名 群名 群主回复（同意或拒绝）
转发给请求加入的用户


数据库关键表

用户表
| Users | CREATE TABLE `Users` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci |

群聊表
| chatRoomGroups | CREATE TABLE `chatRoomGroups` (
  `group_id` int NOT NULL AUTO_INCREMENT,
  `group_name` varchar(255) NOT NULL,
  PRIMARY KEY (`group_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci |

群聊成员表
| groupname_members | CREATE TABLE `groupname_members` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `type` enum('master','manager','member') NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci |

用户已加入群聊表
| username_joined_groups | CREATE TABLE `username_joined_groups` (
  `id` int NOT NULL AUTO_INCREMENT,
  `groupname` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci |

用户好友表
| username_friends | CREATE TABLE `username_friends` (
  `friends` char(30) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci |
