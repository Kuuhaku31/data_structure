
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define OK 1
#define ERROR 0

#define MAX_TREE_SIZE 100

typedef char TElemType;

typedef struct BiTNode
{
    TElemType       data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;


/* 函数声明 */

void visit(TElemType s);                                     // 访问结点的函数
void input(TElemType& s);                                    // 输入结点数据的函数
void CreateBiTree(BiTree& T);                                // 创建二叉树的函数
void DestroyBiTree(BiTree& T);                               // 销毁二叉树的函数
int  BiTreeEmpty(BiTree T);                                  // 判断二叉树是否为空
void ProOrderTraverse(BiTree T, void (*Visit)(TElemType));   // 先序遍历二叉树
void InOrderTraverse(BiTree T, void (*Visit)(TElemType));    // 中序遍历二叉树
void PostOrderTraverse(BiTree T, void (*Visit)(TElemType));  // 后序遍历二叉树
int  BiTreeDepth(BiTree T);                                  // 计算该二叉树的深度，返回深度值
int  NodeCount(BiTree T);                                    // 计算该二叉树的总的节点个数，返回节点个数
int  LeafNodeCount(BiTree T);                                // 算该二叉树的叶子节点个数，返回叶子节点个数
void LevelOrderTraverse(BiTree T, void (*Visit)(TElemType)); // 层序遍历二叉树
void InOrderTraverse2(BiTree T, void (*Visit)(TElemType));   // 中序非递归遍历二叉树


void exchange(BiTree T);  // 实现二叉树左右子树的交换（非递归法）
void exchange_(BiTree T); // 实现二叉树左右子树的交换（递归法）
