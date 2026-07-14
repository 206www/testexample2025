// user.c - 用户管理功能实现
#include "user.h"

// 初始化用户列表
void initUserList(UserList *list) { 
    list->count = 0;
}

// 从文件加载用户数据
int loadUsersFromFile(UserList *list, const char *filename) { 
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 0;  // 文件不存在或打开失败
    }
    fread(list, sizeof(UserList), 1, fp);
    fclose(fp);
    return 1;  // 加载成功
}

// 保存用户数据到文件
int saveUsersToFile(UserList *list, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;  // 文件打开失败
    }
    fwrite(list, sizeof(UserList), 1, fp);
    fclose(fp);
    return 1;  // 保存成功
}

// 检查用户名是否已存在
int isUsernameExists(UserList *list, const char *username) {
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->users[i].username, username) == 0) {
            return 1;  // 用户名已存在
        }
    }
    return 0;  // 用户名不存在
}

// 用户注册
int registerUser(UserList *list, const char *username, const char *password, UserRole role) {
    if (list->count >= MAX_SIZE) {
        return 0;  // 用户数量已达上限
    }
    if (isUsernameExists(list, username)) {
        return -1;  // 用户名已存在
    }
    // 复制用户名和密码
    strcpy(list->users[list->count].username, username);
    strcpy(list->users[list->count].password, password);
    list->users[list->count].role = role;
    list->count++;
    return 1;  // 注册成功
}

// 用户登录
User* loginUser(UserList *list, const char *username, const char *password) {
    // 系统管理员账户：wzj/216206
    if (strcmp(username, "wzj") == 0 && strcmp(password, "216206") == 0) {
        static User adminUser;
        strcpy(adminUser.username, "wzj");
        strcpy(adminUser.password, "216206");
        adminUser.role = ROLE_ADMIN;
        return &adminUser;
    }
    
    // 普通用户登录验证
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->users[i].username, username) == 0 &&
            strcmp(list->users[i].password, password) == 0) {
            // 普通用户登录后默认为游客角色
            list->users[i].role = ROLE_VISITOR;
            return &list->users[i];
        }
    }
    return NULL;  // 登录失败
}

// 获取角色名称字符串
const char* getRoleName(UserRole role) {
    switch (role) {
        case ROLE_ADMIN:
            return "管理员";
        case ROLE_VISITOR:
            return "游客";
        default:
            return "未知";
    }
}

// 清理用户信息文件（清空所有用户数据）
int clearUserFile(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;  // 文件打开失败
    }
    
    UserList emptyList;
    initUserList(&emptyList);
    fwrite(&emptyList, sizeof(UserList), 1, fp);
    fclose(fp);
    return 1;  // 清理成功
}

// 查看所有已注册用户信息
void viewUsers(UserList *list) {
    if (list->count == 0) {
        printf("暂无已注册用户！\n");
        return;
    }
    
    printf("\n==================================================\n");
    printf("              已注册用户列表\n");
    printf("==================================================\n");
    printf("%-15s | %-15s | %-10s\n", "用户名", "密码", "角色");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < list->count; i++) {
        printf("%-15s | %-15s | %-10s\n", 
               list->users[i].username, 
               list->users[i].password,
               getRoleName(list->users[i].role));
    }
    
    printf("--------------------------------------------------\n");
    printf("                              共 %d 个用户\n", list->count);
    printf("==================================================\n");
}
