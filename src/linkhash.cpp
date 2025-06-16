
// linkhash.cpp

#include "linkhash.h"

#include <stdio.h>


using namespace std;

// 哈希函数：除留余数法
unsigned
Hash(KeyType K, int m)
{
    return K % m;
}

// 初始化哈希表
void
InitHash(HashTable& H, int size)
{
    H.size  = size;
    H.count = 0;
    H.m     = size;
    H.rcd   = new ElemNode*[size];
    for(int i = 0; i < size; ++i)
    {
        H.rcd[i] = nullptr;
    }
}

// 销毁哈希表
void
DestroyHash(HashTable& H)
{
    for(int i = 0; i < H.size; ++i)
    {
        ElemNode* p = H.rcd[i];
        while(p)
        {
            ElemNode* tmp = p;
            p             = p->next_node;
            delete tmp;
        }
    }
    delete[] H.rcd;
    H.rcd  = nullptr;
    H.size = H.count = H.m = 0;
}

// 查找
ElemNode*
SearchHash(const HashTable& H, KeyType key)
{
    int       index = Hash(key, H.m);
    ElemNode* p     = H.rcd[index];
    while(p)
    {
        if(p->elem.key == key)
            return p;
        p = p->next_node;
    }
    return nullptr;
}

// 插入
void
InsertHash(HashTable& H, ElemType e)
{
    int       index    = Hash(e.key, H.m);
    ElemNode* newNode  = new ElemNode{ e, nullptr };
    newNode->next_node = H.rcd[index];
    H.rcd[index]       = newNode;
    ++H.count;
}

// 删除
void
DeleteHash(HashTable& H, ElemType e)
{
    int       index = Hash(e.key, H.m);
    ElemNode* p     = H.rcd[index];
    ElemNode* prev  = nullptr;
    while(p)
    {
        if(p->elem.key == e.key)
        {
            if(prev)
                prev->next_node = p->next_node;
            else
                H.rcd[index] = p->next_node;
            delete p;
            --H.count;
            return;
        }
        prev = p;
        p    = p->next_node;
    }
}

// 遍历
void
HashTraverse(const HashTable& H)
{
    for(int i = 0; i < H.size; ++i)
    {
        printf("索引[%02d] -> ", i);
        ElemNode* p = H.rcd[i];
        while(p)
        {
            printf("[%d:%d] -> ", p->elem.key, p->elem.data);
            p = p->next_node;
        }
        printf("NULL\n");
    }
}
