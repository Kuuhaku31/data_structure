
// sqlist.cpp

#include "sqlist.h"


void
input(ElemType& s)
{
    scanf("%d", &s);
}

void
output(ElemType s)
{
    printf("%d ", s);
}

int
equals(ElemType a, ElemType b)
{
    if(a == b) return 1;
    else return 0;
}

// 操作结果：构造一个空的顺序线性表
void
InitList(SqList& L)
{
    if(L.elem) delete[] L.elem;       // 如果之前有分配的存储空间，先释放

    L.elem = new ElemType[INIT_SIZE]; // 分配初始存储空间

    if(!L.elem) return;               // 存储分配失败
    L.length   = 0;                   // 空表长度为0
    L.listsize = INIT_SIZE;           // 初始存储容量
}

// 初始条件：顺序线性表L已存在，1 <= i <= ListLength(L) + 1
// 操作结果：在L中第i个位置之前插入新的数据元素elem，L的长度加 1
int
ListInsert(SqList& sqlist, int index, ElemType elem)
{
    if(index < 1 || index > sqlist.length + 1) return 0; // i值不合法
    if(sqlist.length >= sqlist.listsize)                 // 当前存储空间已满，增加分配
    {
        // 分配新的存储空间
        ElemType* newbase = new ElemType[sqlist.listsize + INCREMENT];

        sqlist.elem      = newbase;   // 新基址
        sqlist.listsize += INCREMENT; // 增加存储容量
    }

    ElemType* q = sqlist.elem + index - 1;                                         // q 为插入位置
    for(ElemType* p = sqlist.elem + sqlist.length - 1; p >= q; --p) *(p + 1) = *p; // 插入位置及之后的元素右移

    *q = elem;                                                                     // 插入elem
    sqlist.length++;                                                               // 表长增1

    return 1;
}


// 初始条件：顺序线性表L已存在
// 操作结果：依次对L的每个数据元素调用函数vi()输出
void
ListTraverse(SqList L, void (*vi)(ElemType))
{
    ElemType* p = L.elem;
    for(int i = 1; i <= L.length; i++) vi(*p++);
    printf("\n");
}


// 已知顺序线性表La和Lb的元素按值非递减排列
// 归并La和Lb得到新的顺序线性表Lc，Lc的元素也按值非递减排列
int
MergeList(SqList La, SqList Lb, SqList& Lc)
{
    InitList(Lc);
    int i = 0, j = 0, k = 0;

    Lc.listsize = La.length + Lb.length;
    Lc.elem     = new ElemType[Lc.listsize]; // 分配存储空间
    if(!Lc.elem) return 0;

    while(i < La.length && j < Lb.length)
    {
        if(La.elem[i] <= Lb.elem[j]) Lc.elem[k++] = La.elem[i++];
        else Lc.elem[k++] = Lb.elem[j++];
    }

    while(i < La.length) Lc.elem[k++] = La.elem[i++];

    while(j < Lb.length) Lc.elem[k++] = Lb.elem[j++];

    Lc.length = k;
    return 1;
}
