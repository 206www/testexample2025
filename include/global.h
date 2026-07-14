#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define INF 32767
#define MAX_USERNAME 50
#define MAX_PASSWORD 50

// 用户角色枚举
typedef enum {
    ROLE_ADMIN = 1,    // 管理员
    ROLE_VISITOR = 2   // 普通游客
} UserRole;

// 用户结构体
typedef struct {
    char username[MAX_USERNAME];  // 账号
    char password[MAX_PASSWORD];  // 密码
    UserRole role;               // 角色标识
} User;

// 用户列表结构
typedef struct {
    User users[MAX_SIZE];
    int count;// 追踪当前实际存储的有效用户数量
} UserList;

// 边节点结构
typedef struct CNode {
    int index;           // 邻接点索引
    int length;          // 边的长度
    struct CNode *next;  // 指向下一个边节点
} CNode;

// 顶点结构
typedef struct VNode {
    char data[20];       // 景点名称
    CNode *first;        // 指向第一条边
} VNode,AdjList[MAX_SIZE];

// 邻接表图结构
typedef struct {
    VNode roadlist[MAX_SIZE];  // 顶点数组
    int nodenum;               // 顶点数
    int edgnum;                // 边数
} ALGraph;

#endif
