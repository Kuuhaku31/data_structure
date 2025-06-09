
// main.cpp

#include "algraph.h"


int
main()
{
    ALGraph    g;
    VertexType v1, v2;
    int        k;
    CreateGraphF(g); // 利用数据文件创建图
    Display(g);      // 输出图
    printf("请输入顶点的值: ");
    scanf("%s", v1);
    printf("输出图G中顶点%s的所有邻接顶点: ", v1);
    k = FirstAdjVex(g, v1);
    while(k != -1)
    {
        strcpy(v2, g.vertices[k].data);
        visit(v2);
        k = NextAdjVex(g, v1, v2);
    }
    printf("\n");
    return 0;
}
