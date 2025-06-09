
// sqstack.cpp

#include "sqstack.h"


void
input(SElemType& s)
{
    scanf("%d", &s);
}

void
output(SElemType s)
{
    printf("%d ", s);
}

void
InitStack(SqStack& S)
{
    S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if(!S.base)
        exit(OVERFLOW);
    S.top       = S.base;
    S.stacksize = STACK_INIT_SIZE;
}

void
DestroyStack(SqStack& S)
{
    free(S.base);
    S.base = S.top = NULL;
    S.stacksize    = 0;
}

void
ClearStack(SqStack& S)
{
    S.top = S.base;
}

int
StackEmpty(SqStack S)
{
    return S.top == S.base ? TRUE : FALSE;
}

int
StackLength(SqStack S)
{
    return S.top - S.base;
}

int
GetTop(SqStack S, SElemType& e)
{
    if(S.top == S.base)
        return ERROR;
    e = *(S.top - 1);
    return OK;
}

void
Push(SqStack& S, SElemType e)
{

    if(S.top - S.base >= S.stacksize)
    {
        S.base = (SElemType*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if(!S.base)
            exit(OVERFLOW);
        S.top        = S.base + S.stacksize;
        S.stacksize += STACKINCREMENT;
    }
    *S.top++ = e;
}

int
Pop(SqStack& S, SElemType& e)
{

    if(S.top == S.base)
        return ERROR;
    e = *--S.top;
    return OK;
}

void
StackTraverse(SqStack S, void (*visit)(SElemType))
{

    SElemType* p = S.base;
    while(p != S.top)
    {
        visit(*p);
        p++;
    }
    printf("\n");
}

void
conversion(unsigned n)
{
    SqStack S;
    InitStack(S);

    while(n != 0)
    {
        Push(S, n % 16);
        n /= 16;
    }

    while(!StackEmpty(S))
    {
        int top;
        Pop(S, top);
        if(top < 10)
            printf("%d", top);
        else
            printf("%c", 'A' + top - 10);
    }

    DestroyStack(S);
    printf("\n");
}
