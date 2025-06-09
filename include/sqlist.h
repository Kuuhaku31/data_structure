
// sqlist.h

#ifndef SQLIST_H
#define SQLIST_H

#include <stdio.h>

#define INIT_SIZE 5  // 顺序表存储空间初始分配大小
#define INCREMENT 10 // 顺序表存储空间分配增量

/* 定义ElemType为int类型 */
typedef int ElemType;

/*****ElemType类型元素的基本操作*****/
void input(ElemType& s);
void output(ElemType s);
int  equals(ElemType a, ElemType b);

/* 顺序表类型定义 */
typedef struct
{
    ElemType* elem;     // 存储空间基地址
    int       length;   // 当前长度
    int       listsize; // 当前分配的存储容量
} SqList;

/*****顺序表的基本操作*****/
void InitList(SqList& L);
int  ListInsert(SqList& L, int i, ElemType e);
void ListTraverse(SqList L, void (*vi)(ElemType));
int  MergeList(SqList La, SqList Lb, SqList& Lc);


#endif // SQLIST_H
