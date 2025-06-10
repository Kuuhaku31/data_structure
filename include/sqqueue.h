
// sqqueue.h

#pragma once


// 函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
typedef int QElemType;

#define MAX_QSIZE 5    // 最大队列长度+1

typedef int QElemType; // 队列元素类型

struct SqQueue
{
    QElemType* base;  // 初始化的动态分配存储空间
    int        front; // 头指针，若队列不空，指向队列头元素
    int        rear;  // 尾指针，若队列不空，指向队列尾元素的下一个位置
};

void InitQueue(SqQueue& Q);                           // 构造一个空循环队列Q
void DestroyQueue(SqQueue& Q);                        // 销毁循环队列Q，Q不再存在
void ClearQueue(SqQueue& Q);                          // 将Q清为空循环队列
int  QueueEmpty(SqQueue Q);                           // 若循环队列Q为空队列，则返回TRUE；否则返回FALSE
int  QueueLength(SqQueue Q);                          // 返回Q的元素个数，即循环队列的长度
int  GetHead(SqQueue Q, QElemType& e);                // 若循环队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
int  EnQueue(SqQueue& Q, QElemType e);                // 插入元素e为循环队列Q的新的队尾元素
int  DeQueue(SqQueue& Q, QElemType& e);               // 若循环队列不空，则删除Q的队头元素，用e返回其值，并返回OK；否则返回ERROR
void QueueTraverse(SqQueue Q, void (*vi)(QElemType)); // 从队头到队尾依次对队列Q中每个元素调用函数vi()
