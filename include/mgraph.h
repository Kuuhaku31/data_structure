
// mgraph.h

#pragma once

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>


// 图的数组(邻接矩阵)存储表示
#define INFINITY INT_MAX     // 用整型最大值代替∞
#define MAX_VERTEX_NUM 20    // 最大顶点个数

typedef int  VRType;         // 顶点关系类型
typedef char VertexType[20]; // 顶点类型

// 图的种类标志
typedef enum
{
    DG,  // 有向图
    DN,  // 有向网
    UDG, // 无向图
    UDN  // 无向网
} GraphKind;


// 顶点关系类型
// 对无权图，用1(是)或0(否)表示相邻否
// 对带权图，则为权值
typedef struct
{
    VRType adj;
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 二维数组


// 图的数组(邻接矩阵)存储
typedef struct
{
    VertexType vexs[MAX_VERTEX_NUM]; // 顶点向量
    AdjMatrix  arcs;                 // 邻接矩阵
    int        vexnum;               // 图的当前顶点数
    int        arc_count;            // 图的当前弧数
    GraphKind  kind;                 // 图的种类标志
} MGraph;


void visit(VertexType i);                                     // 访问顶点 i 的操作
int  IsElemEq(VertexType a, VertexType b);                    // 判断顶点 a 和 b 是否相等

void        CreateGraphF(MGraph& G);                          // 采用数组(邻接矩阵)表示法，由文件构造无向网G
void        Display(MGraph G);                                // 输出邻接矩阵存储表示的图 G
int         LocateVex(MGraph G, VertexType u);                // 若G中存在顶点u，则返回该顶点在图中位置；否则返回 -1
VertexType& GetVex(MGraph G, int v);                          // v是G中某个顶点的序号，返回v的值
int         FirstAdjVex(MGraph G, VertexType v);              // v是图G中某个顶点，返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点，则返回 -1
int         NextAdjVex(MGraph G, VertexType v, VertexType w); // v是G中某个顶点，w是v的邻接顶点，返回v的(相对于w的)下一个邻接顶点的序号，若w是v的最后一个邻接顶点，则返回 -1
void        DestroyGraph(MGraph& G);                          // 销毁图G


/* 遍历算法 */

void DFSTraverse(MGraph G); // 从第1个顶点起，按深度优先遍历图G
void BFSTraverse(MGraph G); // 从第1个顶点起，按广度优先遍历图G