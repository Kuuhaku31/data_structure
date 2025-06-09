
// main.cpp

#include "bitree.h"

int
main()
{
    BiTree T;
    CreateBiTree(T);
    printf("该树的高度为：%d\n", BiTreeDepth(T));
    printf("节点的数目为: %d\n", NodeCount(T));
    printf("叶子节点的数目为: %d\n", LeafNodeCount(T));
    DestroyBiTree(T);
    return 0;
}
