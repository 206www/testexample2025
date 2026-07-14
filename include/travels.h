#ifndef TRAVELS_H
#define TRAVELS_H

#include "global.h"
int locate(ALGraph *graph,char *e);
//定位
void DFSTravels(ALGraph graph);
//深度优先遍历
void DFS(ALGraph *graph,int v);
//深度优先遍历的递归函数
int isedg(ALGraph *graph,char *e1,char *e2);
//判断是否是边
void shortPath(ALGraph *graph,int path[][MAX_SIZE],double shortpath[][MAX_SIZE]);
//最短路径
void printPath(ALGraph *graph,int path[][MAX_SIZE],double shortpath[][MAX_SIZE],int i,int j);
//打印最短路径
int islooptest(ALGraph *graph);
//是否有回路
//查询最短路径
void minDistance(ALGraph *graph,int path[][MAX_SIZE],double shortpath[][MAX_SIZE]);
//查询从s1到s2的最短路径

#endif


