
// bitree.cpp

#include "bitree.h"
#include "linkqueue.h"
#include "sqstack.h"

#include <stdio.h>
#include <stdlib.h>


TElemType Nil = '#';


void
visit(TElemType s)
{
    printf("%c,", s);
}


void
input(TElemType& s)
{
    scanf("%c", &s);
}


void
_CreateBiTree(BiTree& T, int& elem_index, char* input_buffer)
{
    if(input_buffer[elem_index] == '\0') return; // 如果到达输入缓冲区末尾，返回

    TElemType elem = input_buffer[elem_index++]; // 从输入缓冲区获取一个元素
    if(elem == Nil) T = nullptr;                 // 如果是空结点，设置当前结点为空
    else
    {
        T = new BiTNode;     // 创建新结点

        T->data   = elem;    // 设置当前结点的数据
        T->lchild = nullptr; // 初始化左子树为空
        T->rchild = nullptr; // 初始化右子树为空

        // 递归创建左子树和右子树
        _CreateBiTree(T->lchild, elem_index, input_buffer);
        _CreateBiTree(T->rchild, elem_index, input_buffer);
    }
}
// 按先序次序输入二叉树中结点的值
// 构造二叉链表表示的二叉树T。变量Nil表示空(子)树。
void
CreateBiTree(BiTree& T)
{
    char input_buffer[1024] = { 0 };            // 输入缓冲区
    int  elem_index         = 0;                // 用于跟踪输入缓冲区中的位置

    elem_index = 0;                             // 重置输入缓冲区索引
    scanf("%s", input_buffer);                  // 从标准输入读取字符串到输入缓冲区
    _CreateBiTree(T, elem_index, input_buffer); // 从输入缓冲区开始创建二叉树
}


// 初始条件：二叉树T存在。操作结果：销毁二叉树T
void
DestroyBiTree(BiTree& T)
{
    if(T)                         // 如果T不为空
    {
        DestroyBiTree(T->lchild); // 递归销毁左子树
        DestroyBiTree(T->rchild); // 递归销毁右子树
        delete T;                 // 销毁当前结点
        T = nullptr;              // 将T置为空
    }
}


// 销毁二叉树T
// 初始条件：二叉树T存在。操作结果：若T为空二叉树，则返回TRUE，否则FALSE
int
BiTreeEmpty(BiTree T)
{
    if(T) return 0;
    else return 1;
}

// 先序遍历二叉树T的递归算法，对每个数据元素调用函数Visit
void
ProOrderTraverse(BiTree T, void (*Visit)(TElemType))
{
    if(T)
    {
        Visit(T->data);                     // 访问根结点
        ProOrderTraverse(T->lchild, Visit); // 先序遍历左子树
        ProOrderTraverse(T->rchild, Visit); // 先序遍历右子树
    }
}


// 中序遍历二叉树T的递归算法，对每个数据元素调用函数Visit
void
InOrderTraverse(BiTree T, void (*Visit)(TElemType))
{
    if(T)
    {
        InOrderTraverse(T->lchild, Visit); // 中序遍历左子树
        Visit(T->data);                    // 访问根结点
        InOrderTraverse(T->rchild, Visit); // 中序遍历右子树
    }
}

// 后序递归遍历T，对每个结点调用函数Visit一次且仅一次
void
PostOrderTraverse(BiTree T, void (*Visit)(TElemType))
{
    if(T)
    {
        PostOrderTraverse(T->lchild, Visit); // 后序遍历左子树
        PostOrderTraverse(T->rchild, Visit); // 后序遍历右子树
        Visit(T->data);                      // 访问根结点
    }
}


// 初始条件：二叉树T存在。操作结果：返回T的深度
int
BiTreeDepth(BiTree T)
{
    if(T == nullptr) return 0; // 空树的深度为0
    else
    {
        int leftDepth  = BiTreeDepth(T->lchild);                      // 左子树深度
        int rightDepth = BiTreeDepth(T->rchild);                      // 右子树深度
        return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1; // 返回最大深度加1
    }
}

// 初始条件：二叉树T存在。操作结果：返回T的结点数
int
NodeCount(BiTree T)
{
    if(T == nullptr) return 0; // 空树没有结点
    else
    {
        int leftCount  = NodeCount(T->lchild); // 左子树结点数
        int rightCount = NodeCount(T->rchild); // 右子树结点数
        return leftCount + rightCount + 1;     // 返回左子树、右子树结点数加1（当前结点）
    }
}

// 初始条件：二叉树T存在。操作结果：返回T的叶子结点数
int
LeafNodeCount(BiTree T)
{
    if(T == nullptr) return 0;                                  // 空树没有叶子结点
    if(T->lchild == nullptr && T->rchild == nullptr) return 1;  // 当前结点是叶子结点
    return LeafNodeCount(T->lchild) + LeafNodeCount(T->rchild); // 返回左右子树叶子结点数之和
}


// 实现二叉树左右子树的交换（递归法）
void
exchange_(BiTree T)
{
    if(T == nullptr) return; // 如果当前结点为空，直接返回

    // 递归交换左子树和右子树
    exchange_(T->lchild);
    exchange_(T->rchild);

    // 交换当前结点的左右子树
    BiTree temp = T->lchild;
    T->lchild   = T->rchild;
    T->rchild   = temp;
}


// 在进行层次遍历的时候，设置一个队列结构，首先将根节点指针入队列，遍历从二叉树的根节点开始：
// 从队头取出一个元素，访问该元素所指向的节点；
// 若该元素所指节点的左右孩子节点非空，则将该元素所指节点的左孩子指针和右孩子指针顺序入队。
// 此过程不断进行，当队列为空时，二叉树的层次遍历结束。
// 初始条件：二叉树T存在，Visit是对结点操作的应用函数
// 操作结果：层序递归遍历T(利用队列)，对每个结点调用函数Visit一次且仅一次
void
LevelOrderTraverse(BiTree T, void (*Visit)(TElemType))
{
    LinkQueue Q;
    InitQueue(Q);
    BiTree p = T;

    if(p != nullptr) EnQueue(Q, p); // 根节点入队

    while(!QueueEmpty(Q))
    {
        DeQueue(Q, p);
        Visit(p->data);
        if(p->lchild != nullptr) EnQueue(Q, p->lchild);
        if(p->rchild != nullptr) EnQueue(Q, p->rchild);
    }

    DestroyQueue(Q);
}


// 实现二叉树左右子树的交换（栈实现）
void
exchange(BiTree T)
{
    if(T == nullptr) return; // 如果当前结点为空，直接返回

    SqStack S;
    InitStack(S); // 初始化栈

    Push(S, T);   // 将根结点入栈

    while(!StackEmpty(S))
    {
        BiTree p;
        Pop(S, p); // 弹出栈顶结点

        // 交换当前结点的左右子树
        BiTree temp = p->lchild;
        p->lchild   = p->rchild;
        p->rchild   = temp;

        // 如果左子树不为空，将左子树入栈
        if(p->lchild != nullptr) Push(S, p->lchild);
        // 如果右子树不为空，将右子树入栈
        if(p->rchild != nullptr) Push(S, p->rchild);
    }

    DestroyStack(S); // 销毁栈
}


// 采用二叉链表存储结构，Visit是对数据元素操作的应用函数。
// 中序遍历二叉树T的非递归算法，对每个数据元素调用函数Visit
void
InOrderTraverse2(BiTree T, void (*Visit)(TElemType))
{
    SqStack S;
    InitStack(S); // 初始化栈
    BiTree p = T; // 指向当前结点

    while(p != nullptr || !StackEmpty(S))
    {
        while(p != nullptr) // 先访问左子树
        {
            Push(S, p);     // 将当前结点入栈
            p = p->lchild;  // 移动到左子结点
        }

        if(!StackEmpty(S))  // 如果栈不为空
        {
            Pop(S, p);      // 弹出栈顶结点
            Visit(p->data); // 访问当前结点
            p = p->rchild;  // 移动到右子结点
        }
    }

    DestroyStack(S); // 销毁栈
}
