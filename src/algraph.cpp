
// algraph.cpp

#include "algraph.h"

#include "sqqueue.h"
#include "sqstack.h"


int
equal(ElemType a, ElemType b)
{
    if(a.adjvex == b.adjvex) return 1;
    else return 0;
}


void
visit(VertexType i)
{
    printf("%s ", i);
}


// 初始条件: 不带头结点的单链表L已存在， equal() 是数据元素判定函数(满足为1，否则为0)
// 操作结果: 返回L中第一个与 e 满足关系 equal() 的数据元素的位序。
// 若这样的数据元素不存在，则返回值为0
int
LocateElem(LinkList L, ElemType e, int (*equal)(ElemType, ElemType))
{
    LinkList p = L; // L是不带头结点的单链表

    int i = 0;
    while(p)
    {
        i++;
        if(equal(p->data, e)) return i; // 找到这样的数据元素
        p = p->next;
    }
    return 0;
}


// 查找表 L 中满足条件的结点。如找到，返回指向该结点的指针，p 指向该结点的前驱（若该结点是首元结点，则 p = NULL ）。
// 如表 L 中无满足条件的结点，则返回NULL，p无定义。函数 equal() 的两形参的关键字相等，返回 OK ；否则返回 ERROR
LinkList
Point(LinkList L, ElemType e, int (*equal)(ElemType, ElemType), LinkList& p)
{
    int i, j;
    i = LocateElem(L, e, equal);

    // 找到
    if(i)
    {
        if(i == 1) // 是首元结点
        {
            p = NULL;
            return L;
        }

        p = L;
        for(j = 2; j < i; j++) p = p->next;
        return p->next;
    }
    else return NULL; // 没找到
}


// 在不带头结点的单链线性表 L 中第 i 个位置之前插入元素 e
int
ListInsert(LinkList& L, int i, ElemType e)
{
    int      j = 1;
    LinkList p = L, s;

    if(i < 1) return 0;                               // i 值不合法

    s       = (LinkList)malloc(sizeof(struct LNode)); // 生成新结点
    s->data = e;                                      // 给s的data域赋值
    if(i == 1)                                        // 插在表头
    {
        s->next = L;
        L       = s; // 改变L
    }
    else
    {                         // 插在表的其余处
        while(p && j < i - 1) // 寻找第i-1个结点
        {
            p = p->next;
            j++;
        }
        if(!p) return 0; // i大于表长+1
        s->next = p->next;
        p->next = s;
    }
    return 1;
}


// 采用邻接表 存储结构，由文件构造没有相关信息图或网G（用一个函数构造4种图）
// 采用数组(邻接矩阵)表示法，由文件构造无向网G
// 输入格式：
// 第一行：图的种类（0-有向图，1-有向网，2-无向图，3-无向网）
// 第二行：文件名
// 文件内容格式：
// 第一行：顶点数和弧数
// 第二行：顶点信息（每个顶点一行）
// 接下来每行表示一条边或弧的信息
// 对于有向网和无向网：
// v1 v2 weight
// 对于有向图和无向图：
// v1 v2
// 其中v1和v2是顶点信息，weight是边的权值（仅对有向网和无向网有效）
void
CreateGraphF(ALGraph& G)
{
    std::string filename;

    // printf("请输入图的种类（0-有向图，1-有向网，2-无向图，3-无向网）: ");
    scanf("%d", &G.kind);
    // printf("请输入数据文件名: ");
    scanf("%s", filename.c_str());

    // 打开文件
    // printf("打开文件 %s...\n", filename.c_str());
    FILE* file = fopen(filename.c_str(), "r");
    if(!file)
    {
        printf("无法打开文件 %s\n", filename.c_str());
        exit(EXIT_FAILURE);
    }

    // 读取顶点数和弧数
    // printf("读取顶点数和弧数...\n");
    fscanf(file, "%d %d", &G.vexnum, &G.arcnum);

    // 读取顶点信息
    for(int i = 0; i < G.vexnum; i++)
    {
        fscanf(file, "%s", G.vertices[i].data);
    }

    // 初始化邻接矩阵
    for(int i = 0; i < G.vexnum; i++)
    {
        for(int j = 0; j < G.vexnum; j++)
        {
            G.vertices[i].firstarc = NULL; // 初始化邻接表
        }
    }
    while(!feof(file))              // 读取剩下的行
    {
        if(G.kind == GraphKind::DN) // 有向网
        {
            // 读取有向边
            VertexType v1, v2;
            int        weight;
            fscanf(file, "%s %s %d", v1, v2, &weight);

            // 查找顶点在图中的位置
            int v1_index = LocateVex(G, v1);
            int v2_index = LocateVex(G, v2);

            // 如果顶点存在，则设置邻接矩阵
            if(v1_index != -1 && v2_index != -1)
            {
                // 创建新弧结点
                LinkList new_arc = (LinkList)malloc(sizeof(LNode));

                new_arc->data.adjvex          = v2_index;                      // 设置指向的顶点位置
                new_arc->data.info            = weight;                        // 设置权值
                new_arc->nextarc              = G.vertices[v1_index].firstarc; // 插入到表头
                G.vertices[v1_index].firstarc = new_arc;                       // 更新表头
            }
        }
        else if(G.kind == GraphKind::UDN) // 无向网
        {
            // 读取无向边
            VertexType v1, v2;
            int        weight;
            fscanf(file, "%s %s %d", v1, v2, &weight);

            // 查找顶点在图中的位置
            int v1_index = LocateVex(G, v1);
            int v2_index = LocateVex(G, v2);

            // 如果顶点存在，则设置邻接矩阵
            if(v1_index != -1 && v2_index != -1)
            {
                // 创建新弧结点
                LinkList new_arc1 = (LinkList)malloc(sizeof(LNode));
                LinkList new_arc2 = (LinkList)malloc(sizeof(LNode));

                new_arc1->data.adjvex         = v2_index;                      // 设置指向的顶点位置
                new_arc1->data.info           = weight;                        // 设置权值
                new_arc1->nextarc             = G.vertices[v1_index].firstarc; // 插入到表头
                G.vertices[v1_index].firstarc = new_arc1;                      // 更新表头

                new_arc2->data.adjvex         = v1_index;                      // 设置指向的顶点位置
                new_arc2->data.info           = weight;                        // 设置权值
                new_arc2->nextarc             = G.vertices[v2_index].firstarc; // 插入到表头
                G.vertices[v2_index].firstarc = new_arc2;                      // 更新表头
            }
        }
        else if(G.kind == GraphKind::DG) // 有向图
        {
            // 读取有向边
            VertexType v1, v2;
            fscanf(file, "%s %s", v1, v2);

            // 查找顶点在图中的位置
            int v1_index = LocateVex(G, v1);
            int v2_index = LocateVex(G, v2);

            // 如果顶点存在，则设置邻接矩阵
            if(v1_index != -1 && v2_index != -1)
            {
                // 创建新弧结点
                LinkList new_arc = (LinkList)malloc(sizeof(LNode));

                new_arc->data.adjvex          = v2_index;                      // 设置指向的顶点位置
                new_arc->data.info            = 1;                             // 有向图用1表示相邻
                new_arc->nextarc              = G.vertices[v1_index].firstarc; // 插入到表头
                G.vertices[v1_index].firstarc = new_arc;                       // 更新表头
            }
        }
        else if(G.kind == GraphKind::UDG) // 无向图
        {
            // 读取无向边
            VertexType v1, v2;
            fscanf(file, "%s %s", v1, v2);

            // 查找顶点在图中的位置
            int v1_index = LocateVex(G, v1);
            int v2_index = LocateVex(G, v2);

            // 如果顶点存在，则设置邻接矩阵
            if(v1_index != -1 && v2_index != -1)
            {
                // 创建新弧结点
                LinkList new_arc1 = (LinkList)malloc(sizeof(LNode));
                LinkList new_arc2 = (LinkList)malloc(sizeof(LNode));

                new_arc1->data.adjvex         = v2_index;                      // 设置指向的顶点位置
                new_arc1->data.info           = 1;                             // 无向图用1表示相邻
                new_arc1->nextarc             = G.vertices[v1_index].firstarc; // 插入到表头
                G.vertices[v1_index].firstarc = new_arc1;                      // 更新表头

                new_arc2->data.adjvex         = v1_index;                      // 设置指向的顶点位置
                new_arc2->data.info           = 1;                             // 无向图用1表示相邻
                new_arc2->nextarc             = G.vertices[v2_index].firstarc; // 插入到表头
                G.vertices[v2_index].firstarc = new_arc2;                      // 更新表头
            }
        }
    }
}


// 输出图的邻接表G
// 初始条件：图G存在
// 操作结果：输出图G的邻接表存储表示
/*
预期输出：
无向网
8个顶点：
北京 天津 郑州 徐州 武汉 上海 株洲 南昌
9条弧(边):
北京→郑州 :695    北京→天津 :137
天津→徐州 :674    天津→北京 :137
郑州→武汉 :534    郑州→徐州 :349    郑州→北京 :695
徐州→上海 :651    徐州→郑州 :349    徐州→天津 :674
武汉→株洲 :409    武汉→郑州 :534
上海→徐州 :651    上海→南昌 :825
株洲→南昌 :367    株洲→武汉 :409
南昌→上海 :825    南昌→株洲 :367
上海 郑州 天津
*/
void
Display(ALGraph G)
{
    switch(G.kind) // 根据图的种类输出不同格式
    {
    case GraphKind::DG: printf("有向图\n"); break;
    case GraphKind::DN: printf("有向网\n"); break;
    case GraphKind::UDG: printf("无向图\n"); break;
    case GraphKind::UDN: printf("无向网\n"); break;
    default: printf("未知图种类\n"); break;
    }

    printf("%d个顶点:\n", G.vexnum);
    for(int i = 0; i < G.vexnum; i++)
    {
        printf("%s ", G.vertices[i].data); // 输出顶点信息
    }

    printf("\n%d条弧(边):\n", G.arcnum);
    for(int i = 0; i < G.vexnum; i++)
    {
        LinkList p = G.vertices[i].firstarc; // 获取顶点i的邻接表
        while(p)
        {
            printf("%s→%s ", G.vertices[i].data, G.vertices[p->data.adjvex].data); // 输出弧信息

            // 如果是图，不输出权
            if(G.kind == GraphKind::DN || G.kind == GraphKind::UDN) // 有向网或无向网
            {
                printf(":%d\t", p->data.info);                      // 输出权值
            }

            p = p->nextarc; // 移动到下一个邻接结点
        }
        printf("\n");
    }
}


// 初始条件：图G存在，u和G中顶点有相同特征
// 操作结果：若G中存在顶点u，则返回该顶点在图中位置；否则返回-1
int
LocateVex(ALGraph G, VertexType u)
{
    for(int i = 0; i < G.vexnum; i++)
    {
        if(strcmp(G.vertices[i].data, u) == 0) // 比较顶点信息
        {
            return i;                          // 返回顶点u在图G中的位置
        }
    }
    return -1; // 顶点u不存在
}


// 初始条件：图G存在，v是G中某个顶点
// 操作结果：返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点，则返回-1
int
FirstAdjVex(ALGraph G, VertexType v)
{
    int v_index = LocateVex(G, v);
    if(v_index == -1) return -1;               // 顶点不存在

    LinkList p = G.vertices[v_index].firstarc; // 获取顶点v的第一个邻接表结点
    if(p == NULL) return -1;                   // 没有邻接顶点

    return p->data.adjvex;                     // 返回第一个邻接顶点的序号
}


// 初始条件：图G存在，v是G中某个顶点，w是v的邻接顶点
// 操作结果：返回v的(相对于w的)下一个邻接顶点的序号。若w是v的最后一个邻接点，则返回-1
int
NextAdjVex(ALGraph G, VertexType v, VertexType w)
{
    int v_index = LocateVex(G, v);
    if(v_index == -1) return -1;               // 顶点v不存在
    int w_index = LocateVex(G, w);
    if(w_index == -1) return -1;               // 顶点w不存在

    LinkList p = G.vertices[v_index].firstarc; // 获取顶点v的第一个邻接表结点
    while(p)
    {
        if(p->data.adjvex == w_index)           // 找到邻接顶点w
        {
            if(p->nextarc)                      // 如果有下一个邻接顶点
            {
                return p->nextarc->data.adjvex; // 返回下一个邻接顶点的序号
            }
            else
            {
                return -1; // w是v的最后一个邻接顶点
            }
        }
        p = p->nextarc; // 继续查找
    }
    return -1;          // 没有找到邻接顶点w
}


int visited[MAX_VERTEX_NUM]; // 访问标志数组(全局量)

// 从第v个顶点出发递归地深度优先遍历图G
void
DFS(ALGraph G, int v)
{
    visited[v] = 1;                      // 标记顶点v为已访问
    visit(G.vertices[v].data);           // 访问顶点v

    LinkList p = G.vertices[v].firstarc; // 获取顶点v的邻接表
    while(p)
    {
        // 如果邻接顶点未访问，则递归访问
        if(!visited[p->data.adjvex]) DFS(G, p->data.adjvex);

        // 访问完邻接顶点后，继续遍历下一个邻接弧
        p = p->nextarc;
    }
}
// 对图G作深度优先遍历
void
DFSTraverse(ALGraph G)
{
    for(int i = 0; i < G.vexnum; i++) visited[i] = 0; // 初始化访问标志数组
    for(int i = 0; i < G.vexnum; i++)
    {
        if(!visited[i]) DFS(G, i);
    }
}


// 按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited
void
BFSTraverse(ALGraph G)
{
    SqQueue Q;    // 辅助队列
    InitQueue(Q); // 初始化队列

    // 初始化访问标志数组
    for(int i = 0; i < G.vexnum; i++) visited[i] = 0;
    for(int i = 0; i < G.vexnum; i++)
    {
        // 如果顶点i未被访问
        if(!visited[i])
        {
            visited[i] = 1;            // 标记为已访问
            visit(G.vertices[i].data); // 访问顶点i
            EnQueue(Q, i);             // 将顶点i入队

            while(!QueueEmpty(Q))      // 当队列不为空时
            {
                int v;
                DeQueue(Q, v);                                  // 出队一个顶点v
                LinkList p = G.vertices[v].firstarc;            // 获取顶点v的邻接表

                while(p)                                        // 遍历顶点v的所有邻接弧
                {
                    if(!visited[p->data.adjvex])                // 如果邻接顶点未被访问
                    {
                        visited[p->data.adjvex] = 1;            // 标记为已访问
                        visit(G.vertices[p->data.adjvex].data); // 访问邻接顶点
                        EnQueue(Q, p->data.adjvex);             // 将邻接顶点入队
                    }
                    p = p->nextarc;                             // 移动到下一个邻接弧
                }
            }
        }
    }
}


// 计算图G每个顶点的入度，并且保存在in_degree数组
void
FindInDegree(ALGraph G, int in_degree[])
{
    for(int i = 0; i < G.vexnum; i++) in_degree[i] = 0; // 赋初值为 0
    for(int i = 0; i < G.vexnum; i++)
    {
        ArcNode* p = G.vertices[i].firstarc;
        while(p)
        {
            in_degree[p->data.adjvex]++;
            p = p->nextarc;
        }
    }
}


// 有向图G采用邻接表存储结构。
// 若G无回路，则输出G的顶点的一个拓扑序列并返回1，否则返回0。
int
TopologicalSort(ALGraph algraph)
{
    int v_index;                      // 弹出的顶点
    int v_count = 0;                  // count记录已输出的顶点数
    int in_degree[MAX_VERTEX_NUM];    // 入度数组
    int top[MAX_VERTEX_NUM];          // 存放拓扑序列的栈

    SqStack sqstack;                  // 辅助栈
    InitStack(sqstack);               // 初始化栈

    FindInDegree(algraph, in_degree); // 求入度

    // 将所有入度为0的顶点入栈
    for(int i = 0; i < algraph.vexnum; i++)
    {
        if(in_degree[i] == 0) Push(sqstack, i);
    }

    // 记录拓扑序列的顶点
    while(!StackEmpty(sqstack))
    {
        Pop(sqstack, v_index);    // 弹出一个顶点k
        top[v_count++] = v_index; // 存入拓扑序列

        // 遍历k的邻接顶点，减少它们的入度
        for(LinkList p = algraph.vertices[v_index].firstarc; p; p = p->nextarc)
        {
            in_degree[p->data.adjvex]--;                                      // 入度减1
            if(in_degree[p->data.adjvex] == 0) Push(sqstack, p->data.adjvex); // 如果邻接顶点的入度变为 0 ，将其入栈
        }
    }

    for(int i = 0; i < v_count; i++) printf("%s ", algraph.vertices[top[i]].data); // 输出拓扑序列
    if(v_count < algraph.vexnum)                                                   // 如果输出的顶点数小于总顶点数，说明有回路
    {
        printf("\n此有向图有回路");
        return 0;
    }

    return 1;
}


// 获取图G中顶点v的信息
VertexType&
GetVex(ALGraph G, int v)
{
    return G.vertices[v].data;
}
