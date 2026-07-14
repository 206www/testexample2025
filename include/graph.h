#ifndef GRAPH_H
#define GRAPH_H

#include "global.h"
int locate(ALGraph *graph, char *e);
void createGraph(ALGraph *graph);//创建图G的邻接表
void printGraph(ALGraph *graph);//查看整个图信息
void createGuideGraph(ALGraph *G,ALGraph *guidG);//创建有向图G的导游图
int addVertex(ALGraph *graph, char *name, char *connectName, int length);//添加景点并建立边
int removeVertex(ALGraph *graph, char *name);//删除景点
int saveGraphToFile(ALGraph *graph, const char *filename);//保存景点图到文件
int loadGraphFromFile(ALGraph *graph, const char *filename);//从文件加载景点图
void freeGraph(ALGraph *graph);//释放图的内存

#endif