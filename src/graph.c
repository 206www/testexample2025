// graph.c - 图的创建、操作和显示功能实现
#include "graph.h"

// 创建图的邻接表
void createGraph(ALGraph *graph) {
    char buf[100];
    int n, e;
    
    // 输入景点数和边数，添加格式验证
    while (1) {
        printf("请输入旅游图的景点数和边数: ");
        fgets(buf, sizeof(buf), stdin);
        if (sscanf(buf, "%d %d", &n, &e) == 2 && n > 0 && n <= MAX_SIZE && e >= 0) {
            graph->nodenum = n;
            graph->edgnum = e;
            break;
        }
        printf("输入格式错误！请输入两个正整数，景点数不超过%d，边数非负。\n", MAX_SIZE);
    }
    
    // 初始化所有顶点
    for(int i = 0; i < graph->nodenum; i++) {
        while (1) {
            printf("请输入第%d个景点名称: ", i+1);
            fgets(buf, sizeof(buf), stdin);
            if (sscanf(buf, "%19s", graph->roadlist[i].data) == 1 && strlen(graph->roadlist[i].data) > 0) {
                graph->roadlist[i].first = NULL;  // 初始化边链表为空
                break;
            }
            printf("景点名称不能为空，请重新输入！\n");
        }
    }
    
    // 录入每条边（无向图，每条边双向存储）
    for(int k = 0; k < graph->edgnum; k++) {
        while (1) {
            printf("请输入第%d条边的两个顶点和长度: ", k+1);
            fgets(buf, sizeof(buf), stdin);
            
            int length = 0;
            char c1[20], c2[20];
            
            if (sscanf(buf, "%19s %19s %d", c1, c2, &length) != 3 || length <= 0) {
                printf("输入格式错误！请输入：景点1 景点2 长度（长度必须为正整数）\n");
                continue;
            }
            
            int i = locate(graph, c1);
            int j = locate(graph, c2);
            
            // 验证顶点是否存在
            if(i == -1 || j == -1) {
                printf("顶点不存在，请重新输入！\n");
                continue;
            }
            
            // 创建两个方向的边节点
            CNode *node1 = (CNode *)malloc(sizeof(CNode));
            CNode *node2 = (CNode *)malloc(sizeof(CNode));
            if(node1 == NULL || node2 == NULL){
                printf("内存分配失败！\n");
                exit(EXIT_FAILURE);
            }
            
            node1->length = length;
            node2->length = length;
            node1->index = j;
            node2->index = i;
            
            // 使用头插法将边节点插入邻接表
            node1->next = graph->roadlist[i].first;
            graph->roadlist[i].first = node1;
            
            node2->next = graph->roadlist[j].first;
            graph->roadlist[j].first = node2;
            
            break;  // 成功添加边，跳出循环
        }
    }
}

// 计算字符串显示宽度（中文字符算2个宽度，英文字符算1个）
int getStringWidth(char *str) {
    int width = 0;
    while(*str) {
        if((*str & 0xF0) == 0xE0) {  // UTF-8 三字节字符（中文等）
            width += 2;
            str += 3;
        } else if((*str & 0xE0) == 0xC0) {  // UTF-8 双字节字符
            width += 2;
            str += 2;
        } else {  // 单字节字符
            width += 1;
            str += 1;
        }
    }
    return width;
}

// 输出字符串并补齐空格到指定宽度
void printStringWithWidth(char *str, int width) {
    printf("%s", str);
    int strWidth = getStringWidth(str);
    int spaces = width - strWidth;
    for(int i = 0; i < spaces; i++) {
        printf(" ");
    }
}

// 以邻接矩阵形式显示图的信息
void printGraph(ALGraph *graph) {
    if(graph->nodenum == 0) {
        printf("图为空，请先创建景区景点图！\n");
        return;
    }
    
    int i, j;
    // 动态分配邻接矩阵内存
    int **parray = (int **)malloc(sizeof(int *) * graph->nodenum);
    for(i = 0; i < graph->nodenum; i++) {
        parray[i] = (int *)malloc(sizeof(int) * graph->nodenum);
    }
    
    // 初始化邻接矩阵
    for(i = 0; i < graph->nodenum; i++) {
        for(j = 0; j < graph->nodenum; j++) {
            if(i == j) {
                parray[i][j] = 0;  // 对角线为0（顶点到自身）
            } else {
                parray[i][j] = INF;  // 其他位置初始化为无穷大
            }
        }
    }
    
    // 将邻接表中的边信息复制到邻接矩阵
    CNode *p;
    for(i = 0; i < graph->nodenum; i++) {
        for(p = graph->roadlist[i].first; p != NULL; p = p->next) {
            j = p->index;
            parray[i][j] = p->length;
        }
    }
    
    // 找出最长的景点名称宽度，用于对齐
    int maxWidth = 0;
    for(i = 0; i < graph->nodenum; i++) {
        int w = getStringWidth(graph->roadlist[i].data);
        if(w > maxWidth) maxWidth = w;
    }
    // 确保最小宽度
    if(maxWidth < 8) maxWidth = 8;
    
    // 输出邻接矩阵
    printf("\n景区景点分布图（邻接矩阵）:\n");
    printf("    ");  // 左上角空白
    for(i = 0; i < graph->nodenum; i++) {
        printStringWithWidth(graph->roadlist[i].data, maxWidth);
        printf(" ");
    }
    printf("\n");
    
    for(i = 0; i < graph->nodenum; i++) {
        printStringWithWidth(graph->roadlist[i].data, maxWidth);
        printf(" ");
        for(j = 0; j < graph->nodenum; j++) {
            if(parray[i][j] == INF) {
                printf("%-*s", maxWidth, "∞");
            } else {
                printf("%-*d", maxWidth, parray[i][j]);
            }
            printf(" ");
        }
        printf("\n");
    }
    
    // 释放邻接矩阵内存
    for(i = 0; i < graph->nodenum; i++) {
        free(parray[i]);
    }
    free(parray);
}

// 创建导游图（有向图，基于无向图生成）
void createGuideGraph(ALGraph *graph, ALGraph *guidgraph) {
    if(graph->nodenum == 0) {
        printf("原图为空，请先创建景区景点图！\n");
        return;
    }
    
    // 复制顶点数量和边数量
    guidgraph->nodenum = graph->nodenum;
    guidgraph->edgnum = graph->edgnum;
    
    // 复制顶点信息
    for(int i = 0; i < graph->nodenum; i++) {
        strcpy(guidgraph->roadlist[i].data, graph->roadlist[i].data);
        guidgraph->roadlist[i].first = NULL;
    }
    
    // 将无向边转换为双向有向边
    CNode *p;
    for(int i = 0; i < graph->nodenum; i++) {
        for(p = graph->roadlist[i].first; p != NULL; p = p->next) {
            int j = p->index;
            
            // 创建有向边 i -> j
            CNode *node = (CNode *)malloc(sizeof(CNode));
            node->length = p->length;
            node->index = j;
            node->next = guidgraph->roadlist[i].first;
            guidgraph->roadlist[i].first = node;
        }
    }
    
    printf("导游图创建成功！\n");
}

// 添加景点并可选择性建立边
int addVertex(ALGraph *graph, char *name, char *connectName, int length) {
    // 检查景点数量是否已达上限
    if(graph->nodenum >= MAX_SIZE) {
        printf("景点数量已达上限！\n");
        return 0;
    }
    
    // 检查景点是否已存在
    if(locate(graph, name) != -1) {
        printf("景点 '%s' 已存在！\n", name);
        return -1;
    }
    
    // 添加新景点到图中
    strcpy(graph->roadlist[graph->nodenum].data, name);
    graph->roadlist[graph->nodenum].first = NULL;
    int newIndex = graph->nodenum;
    graph->nodenum++;
    
    printf("景点 '%s' 添加成功！\n", name);
    
    // 如果指定了连接的景点，建立双向边
    if(connectName != NULL && strlen(connectName) > 0) {
        int connectIndex = locate(graph, connectName);
        if(connectIndex != -1) {
            CNode *node1 = (CNode *)malloc(sizeof(CNode));
            CNode *node2 = (CNode *)malloc(sizeof(CNode));
            if(node1 == NULL || node2 == NULL){
                printf("内存分配失败！\n");
                exit(EXIT_FAILURE);
            }
            
            node1->length = length;
            node2->length = length;
            node1->index = connectIndex;
            node2->index = newIndex;
            
            // 头插法插入边
            node1->next = graph->roadlist[newIndex].first;
            graph->roadlist[newIndex].first = node1;
            
            node2->next = graph->roadlist[connectIndex].first;
            graph->roadlist[connectIndex].first = node2;
            
            graph->edgnum++;
            printf("已建立 '%s' 与 '%s' 之间的边（长度：%d）\n", name, connectName, length);
        } else {
            printf("连接景点 '%s' 不存在，未建立边\n", connectName);
        }
    }
    
    return 1;
}

// 删除指定景点及其相关边
int removeVertex(ALGraph *graph, char *name) {
    int pos = locate(graph, name);
    if(pos == -1) {
        printf("景点 '%s' 不存在！\n", name);
        return -1;
    }
    
    // 释放被删除景点的边链表
    CNode *p, *q;
    p = graph->roadlist[pos].first;
    while(p != NULL) {
        q = p;
        p = p->next;
        free(q);
    }
    graph->roadlist[pos].first = NULL;
    
    // 删除其他景点指向被删除景点的边，并更新索引
    for(int i = 0; i < graph->nodenum; i++) {
        if(i == pos) continue;
        
        // 删除指向被删除顶点的边
        CNode **pp = &graph->roadlist[i].first;
        while(*pp != NULL) {
            if((*pp)->index == pos) {
                q = *pp;
                *pp = (*pp)->next;
                free(q);
                graph->edgnum--;
            } else {
                // 更新大于pos的索引
                if((*pp)->index > pos) {
                    (*pp)->index--;
                }
                pp = &(*pp)->next;
            }
        }
    }
    
    // 将后面的顶点向前移动填补空缺
    for(int i = pos; i < graph->nodenum - 1; i++) {
        strcpy(graph->roadlist[i].data, graph->roadlist[i+1].data);
        graph->roadlist[i].first = graph->roadlist[i+1].first;
        
        // 更新边索引
        p = graph->roadlist[i].first;
        while(p != NULL) {
            if(p->index > pos) {
                p->index--;
            }
            p = p->next;
        }
    }
    
    // 清空最后一个顶点
    graph->roadlist[graph->nodenum - 1].data[0] = '\0';
    graph->roadlist[graph->nodenum - 1].first = NULL;
    
    graph->nodenum--;
    printf("景点 '%s' 删除成功！\n", name);
    return 1;
}





// 保存景点图到文件
int saveGraphToFile(ALGraph *graph, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL) {
        printf("无法打开文件 '%s' 进行写入！\n", filename);
        return 0;
    }
    
    // 写入顶点数和边数
    fwrite(&graph->nodenum, sizeof(int), 1, fp);
    fwrite(&graph->edgnum, sizeof(int), 1, fp);
    
    // 写入顶点名称
    for(int i = 0; i < graph->nodenum; i++) {
        fwrite(graph->roadlist[i].data, sizeof(char), 20, fp);
    }
    
    // 写入边信息（邻接表结构）
    for(int i = 0; i < graph->nodenum; i++) {
        CNode *p = graph->roadlist[i].first;
        // 先写入该顶点的边数量
        int edgeCount = 0;
        CNode *temp = p;
        while(temp != NULL) {
            edgeCount++;
            temp = temp->next;
        }
        fwrite(&edgeCount, sizeof(int), 1, fp);
        
        // 写入每条边的信息
        while(p != NULL) {
            fwrite(&p->index, sizeof(int), 1, fp);
            fwrite(&p->length, sizeof(int), 1, fp);
            p = p->next;
        }
    }
    
    fclose(fp);
    return 1;
}

// 释放图的内存
void freeGraph(ALGraph *graph) {
    for(int i = 0; i < graph->nodenum; i++) {
        CNode *p = graph->roadlist[i].first;
        while(p != NULL) {
            CNode *q = p;
            p = p->next;
            free(q);
        }
        graph->roadlist[i].first = NULL;
    }
    graph->nodenum = 0;
    graph->edgnum = 0;
}

// 从文件加载景点图
int loadGraphFromFile(ALGraph *graph, const char *filename) {
    // 在加载前释放原有图的边链表内存，防止内存泄漏
    for(int i = 0; i < graph->nodenum; i++) {
        CNode *p = graph->roadlist[i].first;
        while(p != NULL) {
            CNode *q = p;
            p = p->next;
            free(q);
        }
        graph->roadlist[i].first = NULL;
    }
    
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL) {
        printf("无法打开文件 '%s' 进行读取！\n", filename);
        return 0;
    }
    
    // 读取顶点数和边数
    fread(&graph->nodenum, sizeof(int), 1, fp);
    fread(&graph->edgnum, sizeof(int), 1, fp);
    
    // 读取顶点名称
    for(int i = 0; i < graph->nodenum; i++) {
        fread(graph->roadlist[i].data, sizeof(char), 20, fp);
        graph->roadlist[i].first = NULL;
    }
    
    // 读取边信息
    for(int i = 0; i < graph->nodenum; i++) {
        int edgeCount;
        fread(&edgeCount, sizeof(int), 1, fp);
        
        for(int j = 0; j < edgeCount; j++) {
            int index, length;
            fread(&index, sizeof(int), 1, fp);
            fread(&length, sizeof(int), 1, fp);
            
            // 创建边节点
            CNode *node = (CNode *)malloc(sizeof(CNode));
            if(node == NULL) {
                printf("内存分配失败！\n");
                fclose(fp);
                return 0;
            }
            node->index = index;
            node->length = length;
            node->next = graph->roadlist[i].first;
            graph->roadlist[i].first = node;
        }
    }
    
    fclose(fp);
    return 1;
}