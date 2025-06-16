
// linkhash.h

#pragma once


typedef int KeyType; // 设关键字域为整型
typedef int DataType;

struct ElemType
{
    KeyType  key;
    DataType data;
};

struct ElemNode
{
    ElemType  elem;
    ElemNode* next_node;
};

struct HashTable
{
    ElemNode** rcd; // （指向指针的指针）存放指针的数组

    int size;       // 哈希表的容量
    int count;      // 当前表中含有的记录个数

    int m;          // 除求余数，通过 m 计算哈希值
};

void      InitHash(HashTable& H, int size);            // 初始化哈希表
void      DestroyHash(HashTable& H);                   // 销毁哈希表
ElemNode* SearchHash(const HashTable& H, KeyType key); // 查找
void      DeleteHash(HashTable& H, ElemType e);        // 删除
void      InsertHash(HashTable& H, ElemType e);        // 插入
void      HashTraverse(const HashTable& H);            // 遍历
unsigned  Hash(KeyType K, int m);                      // 计算哈希函数
