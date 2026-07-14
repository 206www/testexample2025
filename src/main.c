// main.c - 景区路径规划系统主程序入口
#include "main.h"
#include "user.h"
#include <windows.h>

// 数据文件路径常量（相对于工作目录，即项目根目录）
#define GRAPH_DATA_FILE "graph.dat"
#define USERS_DATA_FILE "users.dat"

// 全局变量声明
ALGraph graph;           // 景区景点图（邻接表结构）
ALGraph guidG;           // 导游图（有向图）
UserList userList;       // 用户列表
User *currentUser = NULL;// 当前登录用户指针

// 函数声明
void handleAdminMenu(void);
void handleVisitorMenu(void);
void handleRegister(void);
void handleLogin(void);

// 系统主函数
int main(void) {
    // 设置控制台编码为UTF-8，支持中文显示
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    int opt = 0;
    char buf[100];
    
    // 初始化用户列表并从文件加载用户数据
    initUserList(&userList);
    loadUsersFromFile(&userList, USERS_DATA_FILE);
    
    // 主循环：显示菜单并处理用户选择
    while (1) {
        showMenu();
        printf("请输入您的选择：");
        scanf("%s", buf);
        
        // 输入验证：确保输入为有效数字且在可选范围内
        while (1) {
            if (!isdigit(buf[0])) {
                printf("您输入的选择有误，请重新输入！\n");
                printf("请输入您的选择：");
                scanf("%s", buf);
            } else if (atoi(buf) < 0 || atoi(buf) > 2) {
                printf("您输入的选择有误，请重新输入！\n");
                printf("请输入您的选择：");
                scanf("%s", buf);
            } else {
                opt = atoi(buf);
                break;
            }
        }
        
        // 根据用户选择执行相应操作
        switch (opt) {
            case 0:
                // 退出系统，保存用户数据并释放内存
                saveUsersToFile(&userList, USERS_DATA_FILE);
                freeGraph(&graph);
                freeGraph(&guidG);
                printf("感谢使用景区路径规划系统，再见！\n");
                return 0;
            case 1:
                // 用户注册
                handleRegister();
                break;
            case 2:
                // 用户登录
                handleLogin();
                break;
            default:
                printf("无效选择，请重新输入！\n");
                break;
        }
    }
    return 0;
}

// 处理用户注册请求
void handleRegister(void) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    
    printf("\n用户注册\n");
    printf("请输入用户名：");
    scanf("%s", username);
    
    // 检查用户名是否为系统保留账户（管理员账户）
    if (strcmp(username, "wzj") == 0) {
        printf("该用户名已被系统保留！\n");
        return;
    }
    
    // 检查用户名是否已存在
    if (isUsernameExists(&userList, username)) {
        printf("用户名已存在！\n");
        return;
    }
    
    printf("请输入密码：");
    scanf("%s", password);
    
    // 所有注册用户默认都是游客角色
    int result = registerUser(&userList, username, password, ROLE_VISITOR);
    if (result == 1) {
        saveUsersToFile(&userList, "users.dat");
        printf("注册成功！您的角色为：游客\n");
    } else if (result == 0) {
        printf("用户数量已达上限！\n");
    }
}

// 处理用户登录请求
void handleLogin(void) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    
    printf("\n用户登录\n");
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);
    
    // 调用登录验证函数
    currentUser = loginUser(&userList, username, password);
    if (currentUser != NULL) {
        printf("登录成功！欢迎, %s (%s)\n", currentUser->username, getRoleName(currentUser->role));
        
        // 根据用户角色显示相应菜单
        if (currentUser->role == ROLE_ADMIN) {
            handleAdminMenu();
        } else if (currentUser->role == ROLE_VISITOR) {
            handleVisitorMenu();
        }
        
        currentUser = NULL;  // 退出登录后清空当前用户指针
    } else {
        printf("用户名或密码错误！\n");
    }
}





// 管理员菜单处理函数
void handleAdminMenu(void) {
    int opt = 0;
    char buf[100];
    int path[MAX_SIZE][MAX_SIZE];
    double shortpath[MAX_SIZE][MAX_SIZE];
    char confirm[10];
    char vertexName[20];
    
    while (1) {
        showAdminMenu();
        printf("请输入您的选择：");
        scanf("%s", buf);
        getchar();  // 吸收换行符
        
        // 输入验证
        while (1) {
            if (!isdigit(buf[0])) {
                printf("您输入的选择有误，请重新输入！\n");
                printf("请输入您的选择：");
                scanf("%s", buf);
                getchar();
            } else if (atoi(buf) < 0 || atoi(buf) > 12) {
                 printf("您输入的选择有误，请重新输入！\n");
                 printf("请输入您的选择：");
                 scanf("%s", buf);
                 getchar();
             } else {
                opt = atoi(buf);
                break;
            }
        }
        
        // 处理管理员操作
        switch (opt) {
            case 0:
                printf("退出管理员模式\n");
                return;
            case 1:
                createGraph(&graph);
                printf("创建景区景点图成功！\n");
                break;
            case 2:
                printGraph(&graph);
                printf("\n景区景点分布图已显示\n");
                break;
            case 3: {
                char e1[20], e2[20];
                char buf[100];
                while (1) {
                    printf("输入两个顶点名称: ");
                    fgets(buf, sizeof(buf), stdin);
                    if (sscanf(buf, "%19s %19s", e1, e2) == 2 && strlen(e1) > 0 && strlen(e2) > 0) {
                        break;
                    }
                    printf("输入格式错误！请输入两个顶点名称，用空格分隔。\n");
                }
                if (isedg(&graph, e1, e2)) {
                    printf("存在从 %s 到 %s 的边\n", e1, e2);
                } else {
                    printf("不存在从 %s 到 %s 的边\n", e1, e2);
                }
                break;
            }
            case 4:
                createGuideGraph(&graph, &guidG);
                if (islooptest(&guidG)) {
                    printf("导游路线中存在回路\n");
                } else {
                    printf("导游路线中不存在回路\n");
                }
                break;
            case 5:
                shortPath(&graph, path, shortpath);
                minDistance(&graph, path, shortpath);
                break;
            case 6: {
                char connectName[20];
                int length;
                char buf[100];
                
                // 输入景点名称，添加验证
                while (1) {
                    printf("请输入要添加的景点名称：");
                    fgets(buf, sizeof(buf), stdin);
                    if (sscanf(buf, "%19s", vertexName) == 1 && strlen(vertexName) > 0) {
                        break;
                    }
                    printf("景点名称不能为空，请重新输入！\n");
                }
                
                // 输入连接的景点名称，添加验证
                while (1) {
                    printf("请输入要连接的现有景点名称（不连接请输入 -）：");
                    fgets(buf, sizeof(buf), stdin);
                    if (sscanf(buf, "%19s", connectName) == 1 && strlen(connectName) > 0) {
                        break;
                    }
                    printf("输入不能为空，请重新输入！\n");
                }
                
                if (strcmp(connectName, "-") != 0) {
                    // 输入边长度，添加验证
                    while (1) {
                        printf("请输入边的长度：");
                        fgets(buf, sizeof(buf), stdin);
                        if (sscanf(buf, "%d", &length) == 1 && length > 0) {
                            break;
                        }
                        printf("边长度必须为正整数，请重新输入！\n");
                    }
                    addVertex(&graph, vertexName, connectName, length);
                } else {
                    addVertex(&graph, vertexName, NULL, 0);
                }
                break;
            }
            case 7: {
                char buf[100];
                while (1) {
                    printf("请输入要删除的景点名称：");
                    fgets(buf, sizeof(buf), stdin);
                    if (sscanf(buf, "%19s", vertexName) == 1 && strlen(vertexName) > 0) {
                        break;
                    }
                    printf("景点名称不能为空，请重新输入！\n");
                }
                removeVertex(&graph, vertexName);
                break;
            }
            case 8:
                printf("警告：此操作将清空所有用户信息，且无法恢复！\n");
                printf("请输入 '确认' 以继续，其他任意键取消：");
                scanf("%s", confirm);
                if (strcmp(confirm, "确认") == 0) {
                    if (clearUserFile("users.dat")) {
                        initUserList(&userList);
                        printf("用户信息文件清理成功！\n");
                    } else {
                        printf("清理失败，请检查文件权限！\n");
                    }
                } else {
                    printf("操作已取消\n");
                }
                break;
            case 9:
                viewUsers(&userList);
                break;
            case 10:
                saveGraphToFile(&graph, GRAPH_DATA_FILE);
                printf("景点图数据已保存到文件！\n");
                break;
            case 11:
                if (loadGraphFromFile(&graph, GRAPH_DATA_FILE)) {
                    printf("景点图数据已从文件加载！\n");
                }
                break;
            case 12:
                printf("警告：此操作将删除景点图数据文件，且无法恢复！\n");
                printf("请输入 '确认' 以继续，其他任意键取消：");
                scanf("%s", confirm);
                if (strcmp(confirm, "确认") == 0) {
                    if (remove(GRAPH_DATA_FILE) == 0) {
                        printf("景点图数据文件删除成功！\n");
                    } else {
                        printf("删除失败，文件可能不存在！\n");
                    }
                } else {
                    printf("操作已取消\n");
                }
                break;
            default:
                printf("无效选择，请重新输入！\n");
                break;
        }
    }
}





// 游客菜单处理函数
void handleVisitorMenu(void) {
    int opt = 0;
    char buf[100];
    int path[MAX_SIZE][MAX_SIZE];
    double shortpath[MAX_SIZE][MAX_SIZE];
    
    while (1) {
        showVisitorMenu();
        printf("请输入您的选择：");
        scanf("%s", buf);
        
        // 输入验证
        while (1) {
            if (!isdigit(buf[0])) {
                printf("您输入的选择有误，请重新输入！\n");
                printf("请输入您的选择：");
                scanf("%s", buf);
            } else if (atoi(buf) < 0 || atoi(buf) > 3) {
                printf("您输入的选择有误，请重新输入！\n");
                printf("请输入您的选择：");
                scanf("%s", buf);
            } else {
                opt = atoi(buf);
                break;
            }
        }
        
        // 处理游客操作
        switch (opt) {
            case 0:
                printf("退出游客模式\n");
                return;
            case 1:
                // 先尝试从文件加载景点图
                if (loadGraphFromFile(&graph, GRAPH_DATA_FILE)) {
                    printGraph(&graph);
                    printf("\n景区景点分布图已显示\n");
                } else {
                    printf("错误：无法加载景区景点分布图！请确保数据文件存在且不为空。\n");
                }
                break;
            case 2: {
                char e1[20], e2[20];
                printf("输入两个顶点名称: ");
                scanf("%s %s", e1, e2);
                if (isedg(&graph, e1, e2)) {
                    printf("存在从 %s 到 %s 的边\n", e1, e2);
                } else {
                    printf("不存在从 %s 到 %s 的边\n", e1, e2);
                }
                break;
            }
            case 3:
                shortPath(&graph, path, shortpath);
                minDistance(&graph, path, shortpath);
                break;
            default:
                printf("无效选择，请重新输入！\n");
                break;
        }
    }
}