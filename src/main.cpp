
// main.cpp

#include "graph.h"


int
main()
{
    MGraph     g;
    VertexType v1, v2;
    CreateGraphF(g); // 利用数据文件创建邻接矩阵表示的图
    Display(g);      // 输出图
    int i, j, k, n;
    printf("请输入顶点的值: ");
    scanf("%s", v1);
    printf("输出图G中顶点%s的所有邻接顶点: ", v1);
    k = FirstAdjVex(g, v1);
    while(k != -1)
    {
        strcpy(v2, g.vertices[k]);
        visit(v2);
        k = NextAdjVex(g, v1, v2);
    }
    printf("\n");
    return 0;
}
