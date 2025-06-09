
// sqstack.h

#pragma once

#include <stdio.h>
#include <stdlib.h>

// 函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

#define STACK_INIT_SIZE 100 // 存储空间初始分配量
#define STACKINCREMENT 10   // 存储空间分配增量


typedef BiTNode* SElemType;
typedef int      SElemType; // 定义栈元素类型为整型


/* 顺序栈类型定义 */
typedef struct
{
    SElemType* base;      // 栈的基址即栈底指针
    SElemType* top;       // 栈顶指针
    int        stacksize; // 当前分配的空间
} SqStack;


/*****SElemType类型元素的基本操作*****/

void input(SElemType& s);
void output(SElemType s);

void InitStack(SqStack& S);                              // 构造一个空栈S
void DestroyStack(SqStack& S);                           // 销毁栈S，S不再存在
void ClearStack(SqStack& S);                             // 把S置为空栈
int  StackEmpty(SqStack S);                              // 若栈S为空栈，则返回TRUE，否则返回FALSE
int  StackLength(SqStack S);                             // 返回S的元素个数，即栈的长度
int  GetTop(SqStack S, SElemType& e);                    // 若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR
void Push(SqStack& S, SElemType e);                      // 插入元素e为新的栈顶元素
int  Pop(SqStack& S, SElemType& e);                      // 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
void StackTraverse(SqStack S, void (*visit)(SElemType)); // 从栈底到栈顶依次对栈中每个元素调用函数visit()

void conversion(unsigned n);                             // 利用顺序栈将一个非负的十进制整数N转换为对应的十六进制数。非负的十进制整数N从键盘输入，转换结果从屏幕输出。