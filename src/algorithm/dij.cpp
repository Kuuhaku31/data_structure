
// dij.cpp

#include "algorithm.h"


// 计算图G中从顶点v0到其他顶点的最短路径
// 迪杰斯特拉算法（Dijkstra's Algorithm）
// G: 图的邻接矩阵表示
// v0: 起点顶点的索引
// dist: 存储从起点到各顶点的最短距离
// path: 存储从起点到各顶点的最短路径，path[i]表示到达顶点i的前驱顶点
void
ShortestPath_DIJ(MGraph G, int v0, int dist[], int path[])
{
    int visited[MAX_VERTEX_NUM] = { 0 }; // 访问标志数组
    for(int i = 0; i < G.vexnum; i++)
    {
        dist[i] = G.arcs[v0][i].adj; // 初始化距离
        path[i] = -1;                // 初始化路径，-1表示没有路径
    }
    dist[v0]    = 0;                 // 起点到自身的距离为0
    visited[v0] = 1;                 // 标记起点已访问

    // 从起点开始，逐步找到最短路径
    // 迭代G.vexnum - 1次，因为起点已经访问过
    for(int i = 1; i < G.vexnum; i++)
    {
        int min_dist = INFINITY;
        int u        = -1; //

        // 找到未访问的顶点中距离起点最近的顶点
        for(int j = 0; j < G.vexnum; j++)
        {
            if(!visited[j] && dist[j] < min_dist)
            {
                min_dist = dist[j];
                u        = j;
            }
        }

        if(u == -1) break; // 所有顶点都已访问或不可达

        visited[u] = 1;    // 标记u已访问

        // 更新与u相邻的顶点的距离
        for(int v = 0; v < G.vexnum; v++)
        {
            if(G.arcs[u][v].adj != INFINITY && !visited[v])
            {
                if(dist[u] + G.arcs[u][v].adj < dist[v])
                {
                    dist[v] = dist[u] + G.arcs[u][v].adj;
                    path[v] = u; // 更新路径
                }
            }
        }
    }
}


int
main()
{
    MGraph G;
    CreateGraphF(G);          // 假设CreateGraphF函数已经定义并实现
    Display(G);               // 显示图的邻接矩阵

    int dist[MAX_VERTEX_NUM]; // 存储最短路径长度
    int path[MAX_VERTEX_NUM]; // 存储最短路径

    int start_vertex = 6;     // 假设从顶点0开始计算最短路径
    ShortestPath_DIJ(G, start_vertex, dist, path);

    // 输出结果
    for(int i = 0; i < G.vexnum; i++)
    {
        printf("从%s到%s的最短路径长度为: %d，路径为: ", G.vexs[start_vertex], G.vexs[i], dist[i]);
        printf("%s ", G.vexs[start_vertex]);
        int p = path[i];
        while(p != -1)
        {
            printf("%s ", G.vexs[p]);
            p = path[p];
        }
        printf("%s ", G.vexs[i]);
        printf("\n");
    }

    return 0;
}
