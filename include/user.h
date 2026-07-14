#ifndef USER_H
#define USER_H

#include "global.h"

// 初始化用户列表
void initUserList(UserList *list);

// 从文件加载用户数据
int loadUsersFromFile(UserList *list, const char *filename);

// 保存用户数据到文件
int saveUsersToFile(UserList *list, const char *filename);

// 用户注册
int registerUser(UserList *list, const char *username, const char *password, UserRole role);

// 用户登录
User* loginUser(UserList *list, const char *username, const char *password);

// 检查用户名是否已存在
int isUsernameExists(UserList *list, const char *username);

// 获取角色名称
const char* getRoleName(UserRole role);

// 清理用户信息文件
int clearUserFile(const char *filename);

// 查看所有已注册用户信息
void viewUsers(UserList *list);

#endif
