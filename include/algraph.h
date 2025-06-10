
// algraph.h

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define MAX_VERTEX_NUM 20

typedef char VertexType[20]; // 顶点类型为字符串


// 图的种类标志
typedef enum
{
    DG,  // 有向图 Directed Graph
    DN,  // 有向网 Directed Network
    UDG, // 无向图 Undirected Graph
    UDN  // 无向网 Undirected Network
} GraphKind;


// 边或弧的元素类型
typedef struct
{
    int adjvex; // 记录该弧终点的下标
    int info;   // 如果是网图（有权重的图），这里存的是权值
} ElemType;


// 邻接表的单链表结点
typedef struct ArcNode
{
    ElemType        data;    // 存储弧的基本信息，包括终点编号和权值
    struct ArcNode* nextarc; // 指向下一个邻接弧的指针，构成单链表
} ArcNode;


// 顶点结点结构与邻接表数组
// VNode: 图中一个顶点的邻接表结点
// AdjList: 是一个邻接表数组，表示整个图的顶点数组
typedef struct
{
    VertexType data;     // 顶点信息
    ArcNode*   firstarc; // 第一个表结点的地址,指向第一条依附该顶点的弧的指针
} VNode, AdjList[MAX_VERTEX_NUM];


// 图的邻接表存储结构
typedef struct
{
    AdjList   vertices;       // 图的邻接表数组
    int       vexnum, arcnum; // 图的当前顶点数和弧数
    GraphKind kind;           // 图的种类标志
} ALGraph;


#define LNode ArcNode      // 定义单链表的结点类型是图的表结点的类型
#define next nextarc       // 定义单链表结点的指针域是表结点指向下一条弧的指针域
typedef ArcNode* LinkList; // 定义指向单链表结点的指针是指向图的表结点的指针


/*
| 数据类型/宏      | 定义     | 含义说明                                       |
| ---------------- | -------- | ---------------------------------------------- |
| `MAX_VERTEX_NUM` | -        | 图中最大顶点数（邻接表数组大小）               |
| `VertexType`     | -        | 顶点的数据类型，长度不超过 19 的字符串         |
| `GraphKind`      | 枚举类型 | 图的种类：有向图、有向网、无向图、无向网       |
| `ElemType`       | 结构体   | 弧的元素：终点下标 + 权值                      |
| `ArcNode`        | 结构体   | 弧结点类型，构成邻接表链表                     |
| `VNode`          | 结构体   | 顶点结点类型，包括顶点数据和邻接链表头指针     |
| `AdjList`        | 数组类型 | 所有顶点构成的邻接表数组                       |
| `ALGraph`        | 结构体   | 图的完整结构，包括邻接表、顶点数、弧数、图类型 |
| `LNode`          | 宏定义   | 将弧结点作为单链表结点使用                     |
| `next`           | 宏定义   | 将 `nextarc` 替代为 `next`，兼容链表操作       |
| `LinkList`       | 指针类型 | 指向链表结点的指针（弧结点指针）               |
*/


int      LocateElem(LinkList L, ElemType e, int (*equal)(ElemType, ElemType));         // 返回L中第1个与e满足关系equal()的数据元素的位序。若这样的数据元素不存在，则返回值为0
LinkList Point(LinkList L, ElemType e, int (*equal)(ElemType, ElemType), LinkList& p); // 查找表L中满足条件的结点。如找到，返回指向该结点的指针，p指向该结点的前驱(若该结点是首元结点，则p=NULL)；如表L中无满足条件的结点，则返回NULL，p无定义。函数equal()的两形参的关键字相等，返回1；否则返回0
int      ListInsert(LinkList& L, int i, ElemType e);                                   // 在不带头结点的单链线性表L中第i个位置之前插入元素e


int  equal(ElemType a, ElemType b); // 判断顶点 a 和 b 是否相等
void visit(VertexType i);           // 访问顶点 i 的操作


void CreateGraphF(ALGraph& G);                          // 采用邻接表存储结构，由文件构造没有相关信息图或网G
void Display(ALGraph G);                                // 输出图的邻接表G
int  LocateVex(ALGraph G, VertexType u);                // 若G中存在顶点u，则返回该顶点在图中位置；否则返回-1
int  FirstAdjVex(ALGraph G, VertexType v);              // 返回v的第一个邻接顶点的序号；否则返回-1
int  NextAdjVex(ALGraph G, VertexType v, VertexType w); // v是图G中某个顶点，w是v的邻接顶点，返回v的(相对于w的)下一个邻接顶点的序号


/* 遍历算法 */

void DFSTraverse(ALGraph G); // 对图G作深度优先遍历
void BFSTraverse(ALGraph G); // 按广度优先非递归遍历图G
