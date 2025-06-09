
// mgraph.cpp

#include "mgraph.h"

#include "sqqueue.h"


// 访问顶点 i 的操作
void
visit(VertexType i)
{
    printf("%s ", i);
}

// 判断顶点 a 和 b 是否相等
int
IsElemEq(VertexType a, VertexType b)
{
    return strcmp(a, b) == 0; // 使用字符串比较函数判断相等
}


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
CreateGraphF(MGraph& G)
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
    fscanf(file, "%d %d", &G.vexnum, &G.arc_count);

    // 读取顶点信息
    for(int i = 0; i < G.vexnum; i++)
    {
        fscanf(file, "%s", G.vexs[i]);
    }

    // 初始化邻接矩阵
    for(int i = 0; i < G.vexnum; i++)
    {
        for(int j = 0; j < G.vexnum; j++)
        {
            // 有向图和无向图初始化为0
            // 有向网和无向网初始化为∞
            if(G.kind == GraphKind::DG || G.kind == GraphKind::UDG)
            {
                G.arcs[i][j].adj = 0; // 无权图用0表示不相邻
            }
            else
            {
                G.arcs[i][j].adj = INFINITY; // 带权图用∞表示不相邻
            }
        }
    }
    while(!feof(file))              // 读取剩下的行
    {
        if(G.kind == GraphKind::DN) // 有向网
        {
            // 读取有向边
            VertexType v1, v2;
            VRType     weight;
            fscanf(file, "%s %s %d", v1, v2, &weight);

            // 查找顶点在图中的位置
            int v1_index = LocateVex(G, v1);
            int v2_index = LocateVex(G, v2);

            // 如果顶点存在，则设置邻接矩阵
            if(v1_index != -1 && v2_index != -1)
            {
                G.arcs[v1_index][v2_index].adj = weight;
            }
        }
        else if(G.kind == GraphKind::UDN) // 无向网
        {
            // 读取无向边
            VertexType v1, v2;
            VRType     weight;
            fscanf(file, "%s %s %d", v1, v2, &weight);

            // 查找顶点在图中的位置
            int v1_index = LocateVex(G, v1);
            int v2_index = LocateVex(G, v2);

            // 如果顶点存在，则设置邻接矩阵
            if(v1_index != -1 && v2_index != -1)
            {
                G.arcs[v1_index][v2_index].adj = weight;
                G.arcs[v2_index][v1_index].adj = weight; // 无向图需要对称设置
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
                G.arcs[v1_index][v2_index].adj = 1; // 有向图用1表示相邻
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
                G.arcs[v1_index][v2_index].adj = 1; // 无向图用1表示相邻
                G.arcs[v2_index][v1_index].adj = 1; // 无向图需要对称设置
            }
        }
    }
}


void
Display(MGraph G)
{
    switch(G.kind)
    {
    case GraphKind::DN: printf("有向网\n"); break;
    case GraphKind::UDN: printf("无向网\n"); break;
    case GraphKind::DG: printf("有向图\n"); break;
    case GraphKind::UDG: printf("无向图\n"); break;
    default: break;
    }

    printf("%d个顶点%d条边。顶点依次是: ", G.vexnum, G.arc_count);
    for(int i = 0; i < G.vexnum; i++) printf("%s ", G.vexs[i]);

    printf("\n图的邻接矩阵:\n");
    for(int i = 0; i < G.vexnum; i++)
    {
        for(int j = 0; j < G.vexnum; j++)
        {
            if(G.arcs[i][j].adj == INFINITY)
                printf("∞\t");
            else
                printf("%d\t", G.arcs[i][j].adj);
        }
        printf("\n");
    }
}


// 初始条件：图G存在，u和G中顶点有相同特征
//  操作结果：若G中存在顶点u，则返回该顶点在图中位置；否则返回-1
int
LocateVex(MGraph G, VertexType u)
{
    for(int i = 0; i < G.vexnum; i++)
    {
        if(IsElemEq(G.vexs[i], u))
        {
            return i; // 返回顶点u在图G中的位置
        }
    }
    return -1;
}


// 初始条件：图G存在，v是G中某个顶点的序号。操作结果：返回v的值
VertexType&
GetVex(MGraph G, int v)
{
    if(v < 0 || v >= G.vexnum)
    {
        printf("错误：顶点序号 %d 超出范围\n", v);
        exit(EXIT_FAILURE);
    }
    return G.vexs[v];
}


// 初始条件：图G存在，v是G中某个顶点
// 操作结果：返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点，则返回-1
int
FirstAdjVex(MGraph G, VertexType v)
{
    int v_index = LocateVex(G, v);
    if(v_index == -1) return -1; // 顶点不存在

    for(int j = 0; j < G.vexnum; j++)
    {
        // 对于图：0表示不相邻，1表示相邻
        // 对于网：INFINITY表示不相邻，其他值表示权值
        if(G.kind == GraphKind::UDG || G.kind == GraphKind::DG)
        {
            if(G.arcs[v_index][j].adj != 0) return j; // 返回第一个邻接顶点的序号
        }
        else
        {
            if(G.arcs[v_index][j].adj != INFINITY) return j; // 返回第一个邻接顶点的序号
        }
    }

    return -1; // 没有邻接顶点
}


// 初始条件：图G存在，v是G中某个顶点，w是v的邻接顶点
// 操作结果：返回v的(相对于w的)下一个邻接顶点的序号，若w是v的最后一个邻接顶点，则返回-1
int
NextAdjVex(MGraph G, VertexType v, VertexType w)
{
    int v_index = LocateVex(G, v);
    if(v_index == -1) return -1; // 顶点v不存在
    int w_index = LocateVex(G, w);
    if(w_index == -1) return -1; // 顶点w不存在

    for(int j = 0; j < G.vexnum; j++)
    {
        // 对于图：0表示不相邻，1表示相邻
        // 对于网：INFINITY表示不相邻，其他值表示权值
        if(G.kind == GraphKind::UDG || G.kind == GraphKind::DG)
        {
            if(G.arcs[v_index][j].adj != 0 && j > w_index) return j; // 返回下一个邻接顶点的序号
        }
        else
        {
            if(G.arcs[v_index][j].adj != INFINITY && j > w_index) return j; // 返回下一个邻接顶点的序号
        }
    }

    return -1; // w是v的最后一个邻接顶点
}


// 初始条件：图G存在。操作结果：销毁图G
void
DestroyGraph(MGraph& G)
{
    // 销毁图G，释放相关资源
    G.vexnum    = 0;
    G.arc_count = 0;
    G.kind      = GraphKind::UDG; // 重置为无向图
    for(int i = 0; i < MAX_VERTEX_NUM; i++)
    {
        G.vexs[i][0] = '\0'; // 清空顶点信息
        for(int j = 0; j < MAX_VERTEX_NUM; j++)
        {
            G.arcs[i][j].adj = INFINITY; // 重置邻接矩阵
        }
    }
}


int visited[MAX_VERTEX_NUM]; // 访问标志数组(全局量)


// 从第v个顶点出发递归地深度优先遍历图G
void
DFS(MGraph G, int v)
{
    visited[v] = 1;   // 标记顶点v为已访问
    visit(G.vexs[v]); // 访问顶点v
    for(int w = 0; w < G.vexnum; w++)
    {
        // 对于图：0表示不相邻，1表示相邻
        // 对于网：INFINITY表示不相邻，其他值表示权值
        if(G.kind == GraphKind::UDG || G.kind == GraphKind::DG)
        {
            if(G.arcs[v][w].adj != 0 && !visited[w]) // 存在边且未访问
            {
                DFS(G, w);                           // 递归访问邻接顶点w
            }
        }
        else
        {
            if(G.arcs[v][w].adj != INFINITY && !visited[w]) // 存在边且未访问
            {
                DFS(G, w);                                  // 递归访问邻接顶点w
            }
        }
    }
}


// 图G存在，从第1个顶点起，深度优先遍历图G，并对每个顶点调用函数visit一次且仅一次
void
DFSTraverse(MGraph G)
{

    // 初始条件：图G存在，vi是顶点的输出函数的指针。
    // 操作结果：从第1个顶点起，深度优先遍历图G，并对每个顶点访问一次且仅一次
    for(int v = 0; v < G.vexnum; v++) visited[v] = 0; // 访问标志数组初始化(未被访问)
    for(int v = 0; v < G.vexnum; v++)
    {
        if(!visited[v]) DFS(G, v); // 对尚未访问的顶点v调用DFS
    }
    printf("\n");
}


// 图G存在，从第1个顶点起，按广度优先非递归遍历图G,并对每个顶点调用函数visit一次且仅一次
void
BFSTraverse(MGraph G)
{
    SqQueue Q;    // 队列
    InitQueue(Q); // 初始化队列
    for(int i = 0; i < G.vexnum; i++)
    {
        visited[i] = 0; // 初始化访问标志数组
    }

    // 遍历所有顶点
    for(int i = 0; i < G.vexnum; i++)
    {
        if(!visited[i])           // 如果顶点i未被访问
        {
            visit(G.vexs[i]);     // 访问顶点i
            visited[i] = 1;       // 标记为已访问
            EnQueue(Q, i);        // 将顶点i入队

            while(!QueueEmpty(Q)) // 当队列不为空
            {
                int v_index;
                DeQueue(Q, v_index); // 出队一个顶点

                // 遍历v_index的所有邻接顶点
                for(int j = FirstAdjVex(G, G.vexs[v_index]); j != -1; j = NextAdjVex(G, G.vexs[v_index], G.vexs[j]))
                {
                    if(!visited[j])       // 如果邻接顶点j未被访问
                    {
                        visit(G.vexs[j]); // 访问邻接顶点j
                        visited[j] = 1;   // 标记为已访问
                        EnQueue(Q, j);    // 将邻接顶点j入队
                    }
                }
            }
        }
    }
}
