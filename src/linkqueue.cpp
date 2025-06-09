
// linkqueue.cpp

#include "linkqueue.h"

void
print(QElemType i)
{
    printf("%d ", i);
}

void
InitQueue(LinkQueue& Q)
{
    Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
    if(!Q.front)
        exit(OVERFLOW);
    Q.front->next = NULL;
}

void
DestroyQueue(LinkQueue& Q)
{
    while(Q.front)
    {
        QueuePtr p = Q.front;
        Q.front    = Q.front->next;
        free(p);
    }
    Q.rear = NULL;
}

void
ClearQueue(LinkQueue& Q)
{
    DestroyQueue(Q);
    InitQueue(Q);
}

int
QueueEmpty(LinkQueue Q)
{
    return Q.front == Q.rear ? TRUE : FALSE;
}

int
QueueLength(LinkQueue Q)
{
    int      length = 0;
    QueuePtr p      = Q.front->next;
    while(p)
    {
        length++;
        p = p->next;
    }
    return length;
}

int
GetHead(LinkQueue Q, QElemType& e)
{
    if(Q.front == Q.rear)
        return ERROR;
    e = Q.front->next->data;
    return OK;
}

int
EnQueue(LinkQueue& Q, QElemType e)
{
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if(!p)
        exit(OVERFLOW);
    p->data      = e;
    p->next      = NULL;
    Q.rear->next = p;
    Q.rear       = p;
    return OK;
}

int
DeQueue(LinkQueue& Q, QElemType& e)
{
    if(Q.front == Q.rear)
        return ERROR;
    QueuePtr p    = Q.front->next;
    e             = p->data;
    Q.front->next = p->next;
    if(Q.rear == p)
        Q.rear = Q.front;
    free(p);
    return OK;
}

void
QueueTraverse(LinkQueue Q, void (*vi)(QElemType))
{
    QueuePtr p = Q.front->next;
    while(p)
    {
        vi(p->data);
        p = p->next;
    }
    printf("\n");
}
