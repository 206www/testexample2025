// menu.c - 系统菜单显示功能实现
#include "menu.h"
#include "global.h"

// 显示系统主菜单
void showMenu() {
    printf("\n**************************\n");
    printf("      欢迎使用景区路径规划系统\n\n");
    printf("请输入您的选择：\n");
    printf("**************************\n\n");
    printf("1 用户注册\n\n");
    printf("2 用户登录\n\n");
    printf("0 退出系统\n\n");
}

// 显示管理员菜单
void showAdminMenu() {
    printf("\n==========================\n");
    printf("      管理员菜单\n");
    printf("==========================\n\n");
    printf("1 创建景区景点图\n\n");
    printf("2 景区景点分布图\n\n");
    printf("3 导游路线\n\n");
    printf("4 导游路线中的回路\n\n");
    printf("5 两个景点间的最短路径和距离\n\n");
    printf("6 添加景点\n\n");
    printf("7 删除景点\n\n");
    printf("8 清理用户信息文件\n\n");
    printf("9 查看已注册用户信息\n\n");
    printf("10 保存景点图到文件\n\n");
    printf("11 从文件加载景点图\n\n");
    printf("12 删除景点图数据文件\n\n");
    printf("0 返回登录\n\n");
}

// 显示游客菜单
void showVisitorMenu() {
    printf("\n==========================\n");
    printf("      游客菜单\n");
    printf("==========================\n\n");
    printf("1 景区景点分布图\n\n");
    printf("2 导游路线\n\n");
    printf("3 两个景点间的最短路径和距离\n\n");
    printf("0 返回登录\n\n");
}
