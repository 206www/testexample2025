// travels.c - 图遍历和路径查找相关功能实现
#include "travels.h"

// 定位顶点在图中的索引位置
int locate(ALGraph *graph, char *e) {
    for(int i = 0; i < graph->nodenum; i++) {
        // 使用 strcmp 正确处理UTF-8编码的中文字符串
        if(strcmp(e, graph->roadlist[i].data) == 0) {
            return i;
        }
    }
    return -1;  // 未找到返回-1
}

// 全局变量用于DFS遍历标记
int visited[MAX_SIZE];

// 深度优先遍历的递归函数
void DFS(ALGraph *graph, int v) {
    visited[v] = 1;  // 标记当前顶点已访问
    printf("%s ", graph->roadlist[v].data);
    
    CNode *p = graph->roadlist[v].first;
    while(p != NULL) {
        if(!visited[p->index]) {
            DFS(graph, p->index);  // 递归访问未访问的邻接顶点
        }
        p = p->next;
    }
}

// 深度优先遍历整个图
void DFSTravels(ALGraph graph) {
    // 初始化访问标记数组
    for(int i = 0; i < graph.nodenum; i++) {
        visited[i] = 0;
    }
    
    printf("深度优先遍历结果: ");
    for(int i = 0; i < graph.nodenum; i++) {
        if(!visited[i]) {
            DFS(&graph, i);
        }
    }
    printf("\n");
}

// 判断两个顶点之间是否存在边
int isedg(ALGraph *graph, char *e1, char *e2) {
    int i = locate(graph, e1);  // 获取第一个顶点的索引
    int j = locate(graph, e2);  // 获取第二个顶点的索引
    
    if(i == -1 || j == -1) {
        return 0;  // 任一顶点不存在
    }
    
    // 遍历顶点i的邻接表，查找是否有指向j的边
    CNode *p = graph->roadlist[i].first;
    while(p != NULL) {
        if(p->index == j) {
            return 1;  // 存在边
        }
        p = p->next;
    }
    
    return 0;  // 不存在边
}

// 使用Floyd-Warshall算法计算所有顶点间的最短路径
void shortPath(ALGraph *graph, int path[][MAX_SIZE], double shortpath[][MAX_SIZE]) {
    int i, j, k;
    
    // 初始化距离矩阵和路径矩阵
    for(i = 0; i < graph->nodenum; i++) {
        for(j = 0; j < graph->nodenum; j++) {
            if(i == j) {
                shortpath[i][j] = 0;  // 顶点到自身距离为0
            } else {
                shortpath[i][j] = INF;  // 初始化为无穷大
            }
            path[i][j] = -1;  // -1表示没有中间节点
        }
    }
    
    // 将邻接表转换为邻接矩阵形式
    CNode *p;
    for(i = 0; i < graph->nodenum; i++) {
        for(p = graph->roadlist[i].first; p != NULL; p = p->next) {
            j = p->index;
            shortpath[i][j] = p->length;
            path[i][j] = i;  // 记录直接可达的路径
        }
    }
    
    // Floyd-Warshall核心算法：通过中间节点k优化路径
    for(k = 0; k < graph->nodenum; k++) {
        for(i = 0; i < graph->nodenum; i++) {
            for(j = 0; j < graph->nodenum; j++) {
                if(shortpath[i][k] + shortpath[k][j] < shortpath[i][j]) {
                    shortpath[i][j] = shortpath[i][k] + shortpath[k][j];
                    path[i][j] = path[k][j];
                }
            }
        }
    }
}

// 递归打印从顶点i到顶点j的路径
void printPath(ALGraph *graph, int path[][MAX_SIZE], double shortpath[][MAX_SIZE], int i, int j) {
    if(path[i][j] == -1) {
        printf("%s", graph->roadlist[i].data);
        return;
    }
    printPath(graph, path, shortpath, i, path[i][j]);
    printf(" -> %s", graph->roadlist[j].data);
}

// 查询两个景点之间的最短路径和距离
void minDistance(ALGraph *graph, int path[][MAX_SIZE], double shortpath[][MAX_SIZE]) {
    if(graph->nodenum == 0) {
        printf("图为空，请先创建景区景点图！\n");
        return;
    }
    
    char s1[20], s2[20];
    printf("请输入起点景点名称: ");
    scanf("%s", s1);
    printf("请输入终点景点名称: ");
    scanf("%s", s2);
    
    int i = locate(graph, s1);
    int j = locate(graph, s2);
    
    if(i == -1) {
        printf("起点景点 %s 不存在！\n", s1);
        return;
    }
    if(j == -1) {
        printf("终点景点 %s 不存在！\n", s2);
        return;
    }
    
    if(shortpath[i][j] == INF) {
        printf("从 %s 到 %s 没有路径！\n", s1, s2);
        return;
    }
    
    printf("从 %s 到 %s 的最短路径: ", s1, s2);
    printPath(graph, path, shortpath, i, j);
    printf("\n最短距离: %d\n", (int)shortpath[i][j]);
}

// 判断有向图是否存在回路（使用DFS检测）
int islooptest(ALGraph *graph) {
    if(graph->nodenum == 0) {
        return 0;
    }
    
    // state数组：0-未访问，1-正在访问（在递归栈中），2-已访问完成
    int state[MAX_SIZE];
    for(int i = 0; i < graph->nodenum; i++) {
        state[i] = 0;
    }
    
    // 对每个未访问的顶点进行DFS
    for(int i = 0; i < graph->nodenum; i++) {
        if(state[i] == 0) {
            int stack[MAX_SIZE], top = -1;
            stack[++top] = i;
            state[i] = 1;  // 标记为正在访问
            
            while(top >= 0) {
                int v = stack[top];
                CNode *p = graph->roadlist[v].first;
                int found = 0;
                
                // 查找未访问的邻接顶点
                while(p != NULL && !found) {
                    int w = p->index;
                    if(state[w] == 1) {
                        return 1;  // 发现后向边，存在回路
                    } else if(state[w] == 0) {
                        stack[++top] = w;
                        state[w] = 1;
                        found = 1;
                    }
                    p = p->next;
                }
                
                if(!found) {
                    state[v] = 2;  // 该顶点DFS完成
                    top--;
                }
            }
        }
    }
    
    return 0;  // 没有回路
}
