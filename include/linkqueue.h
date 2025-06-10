
// linkqueue.h

#pragma once

#include <stdio.h>
#include <stdlib.h>


using namespace std;

// 函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int QElemType;
// typedef BiTNode* QElemType;

typedef struct QNode
{
    QElemType data;
    QNode*    next;
}* QueuePtr;

struct LinkQueue
{
    QueuePtr front, rear; // 队头、队尾指针
};

void InitQueue(LinkQueue& Q);                           // 构造一个空队列Q
void DestroyQueue(LinkQueue& Q);                        // 销毁队列Q，Q不再存在
void ClearQueue(LinkQueue& Q);                          // 将Q清为空队列
int  QueueEmpty(LinkQueue Q);                           // 若队列Q为空队列，则返回TRUE；否则返回FALSE
int  QueueLength(LinkQueue Q);                          // 返回Q的元素个数，即队列的长度
int  GetHead(LinkQueue Q, QElemType& e);                // 若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
int  EnQueue(LinkQueue& Q, QElemType e);                // 插入元素e为Q的新的队尾元素
int  DeQueue(LinkQueue& Q, QElemType& e);               // 若队列不空，则删除Q的队头元素，用e返回其值，并返回OK；否则返回ERROR
void QueueTraverse(LinkQueue Q, void (*vi)(QElemType)); // 从队头到队尾依次对队列