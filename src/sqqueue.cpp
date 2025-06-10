
// sqqueue.cpp

#include "sqqueue.h"

#include <stdio.h>
#include <stdlib.h>


// 初始化
void
InitQueue(SqQueue& Q)
{
    Q.base = (QElemType*)malloc(MAX_QSIZE * sizeof(QElemType));
    if(!Q.base)
        exit(OVERFLOW);
    Q.front = Q.rear = 0;
}


// 销毁循环队列
void
DestroyQueue(SqQueue& Q)
{
    free(Q.base);
    Q.base  = NULL;
    Q.front = Q.rear = 0;
}


// 清空循环队列
void
ClearQueue(SqQueue& Q)
{
    Q.front = Q.rear = 0;
}


// 判断循环队列是否为空
int
QueueEmpty(SqQueue Q)
{
    return Q.front == Q.rear ? TRUE : FALSE;
}


// 返回循环队列的长度
int
QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAX_QSIZE) % MAX_QSIZE;
}


// 获取循环队列的头元素
int
GetHead(SqQueue Q, QElemType& e)
{
    if(Q.front == Q.rear)
        return ERROR;
    e = Q.base[Q.front];
    return OK;
}


// 插入元素e为循环队列Q的新的队尾元素
int
EnQueue(SqQueue& Q, QElemType e)
{
    if((Q.rear + 1) % MAX_QSIZE == Q.front)
        return ERROR;
    Q.base[Q.rear] = e;
    Q.rear         = (Q.rear + 1) % MAX_QSIZE;
    return OK;
}


// 删除循环队列Q的队头元素
int
DeQueue(SqQueue& Q, QElemType& e)
{
    if(Q.front == Q.rear)
        return ERROR;
    e       = Q.base[Q.front];
    Q.front = (Q.front + 1) % MAX_QSIZE;
    return OK;
}


// 从队头到队尾依次对循环队列Q中每个元素调用函数vi()
void
QueueTraverse(SqQueue Q, void (*vi)(QElemType))
{
    int i = Q.front;
    while(i != Q.rear)
    {
        vi(Q.base[i]);
        i = (i + 1) % MAX_QSIZE;
    }
    printf("\n");
}
