
// hash.h

#pragma once

#include <stdio.h>
#include <stdlib.h>


#define NULL_KEY -1 // -1为无记录标志
#define SUCCESS 1
#define UNSUCCESS 0
#define DUPLICATE -1
#define N 100 // 数据元素个数


typedef int KeyType; // 设关键字域为整型


struct ElemType // 数据元素类型
{
    KeyType key;
};


struct HashTable
{
    ElemType* elem;      // 数据元素存储基址，动态分配数组
    int       count;     // 当前数据元素个数
    int       sizeindex; // sizeindex为哈希表表长
};


// 哈希函数的基本操作函数

void     InitHashTable(HashTable& H);                        // 构造一个空的哈希表
void     DestroyHashTable(HashTable& H);                     // 哈希表H存在。操作结果：销毁哈希表H
unsigned Hash(KeyType K);                                    // 哈希函数
void     collision(int& p, int d);                           // 线性探测再散列
int      SearchHash(HashTable H, KeyType K, int& p, int& c); // 在开放定址哈希表H中查找关键码为K的元素
int      InsertHash(HashTable& H, ElemType e);               // 查找不成功时插入数据元素e到开放定址哈希表H中
void     TraverseHash(HashTable H, void (*Vi)(int));         // 按哈希地址的顺序遍历哈希表
void     print(int p);                                       // 打印哈希地址或关键码
int      Find(HashTable H, KeyType K, int& p);               // 查找关键码为K的元素，并返回其哈希地址
