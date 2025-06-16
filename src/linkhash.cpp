
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
    // 确保哈希表大小大于0
    H.size  = size;
    H.count = 0;
    H.m     = size;
    H.rcd   = new ElemNode*[size];

    // 初始化指针数组，每个元素指向 nullptr
    for(int i = 0; i < size; ++i)
    {
        H.rcd[i] = nullptr;
    }
}

// 销毁哈希表
void
DestroyHash(HashTable& H)
{
    // 遍历每个链表，释放节点内存
    for(int i = 0; i < H.size; ++i)
    {
        ElemNode* tmp = nullptr;
        ElemNode* p   = H.rcd[i];
        while(p)
        {
            tmp = p;
            p   = p->next_node;
            delete tmp;
        }
    }

    // 释放指针数组内存
    delete[] H.rcd;
    H.rcd  = nullptr;
    H.size = H.count = H.m = 0;
}

// 查找
ElemNode*
SearchHash(const HashTable& H, KeyType key)
{
    // 计算哈希值
    int index = Hash(key, H.m);

    // 遍历对应链表查找
    ElemNode* p = H.rcd[index];
    while(p)
    {
        if(p->elem.key == key) return p;
        p = p->next_node;
    }
    return nullptr;
}

// 插入
void
InsertHash(HashTable& H, ElemType e)
{
    int index = Hash(e.key, H.m);

    // 检查是否已存在相同的键
    bool      found = false;
    ElemNode* p     = H.rcd[index];
    while(p)
    {
        if(p->elem.key == e.key && p->elem.data == e.data)
        {
            found = true; // 已存在相同键
            break;
        }
        p = p->next_node;
    }

    // 如果不存在相同键，则插入新节点
    if(!found)
    {
        ElemNode* newNode  = new ElemNode{ e, nullptr };
        newNode->next_node = H.rcd[index];
        H.rcd[index]       = newNode;
        H.count++;
    }
}

// 删除
void
DeleteHash(HashTable& H, ElemType e)
{
    int index = Hash(e.key, H.m);

    ElemNode* prev = nullptr;      // 前一个节点指针
    ElemNode* p    = H.rcd[index]; // 当前节点指针
    while(p)
    {
        if(p->elem.key == e.key)
        {
            if(prev) prev->next_node = p->next_node; // 如果不是头节点，前一个节点指向下一个节点
            else H.rcd[index] = p->next_node;        // 如果是头节点，直接更新头指针

            delete p;
            H.count--;
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
