#ifndef GRAPH_H
#define GRAPH_H

#include "global.h"
void createGraph(ALGraph *G);//创建图G的邻接表
void printGraph(ALGraph *G);//查看整个图信息
void createGuideGraph(ALGraph *G,ALGraph *guidG);//创建有向图G的导游图

#endif