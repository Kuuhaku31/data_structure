
// hash.cpp

#include "hash.h"


int m = 0; // 全局变量, H(key) = key % m


// 哈希函数的基本操作函数
// 操作结果：构造一个空的哈希表
void
InitHashTable(HashTable& H)
{
    // 输入哈希表存储容量
    scanf("%d", &H.sizeindex);

    H.count = 0;
    H.elem  = (ElemType*)malloc(H.sizeindex * sizeof(ElemType));
    for(int i = 0; i < H.sizeindex; i++) H.elem[i].key = NULL_KEY; // 未填记录的标志
}


// 初始条件：哈希表H存在。操作结果：销毁哈希表H
void
DestroyHashTable(HashTable& H)
{
    free(H.elem);
    H.elem      = NULL;
    H.count     = 0;
    H.sizeindex = 0;
}


// 一个简单的哈希函数(m为全局变量)
unsigned
Hash(KeyType K)
{
    return K % m; // 哈希函数
}

// 线性探测再散列
// 开放定址法处理冲突
void
collision(int& p, int d)
{
    p = (p + d) % m;
}


// 在开放定址哈希表H中查找关键码为K的元素，若查找成功，以 index 指示待查数据
// 元素在表中位置，并返回 SUCCESS；否则，以 index 指示插入位置，并返回 UNSUCCESS
// count 用以计冲突次数，其初值置零，供建表插入时参考。
int
SearchHash(HashTable H, KeyType K, int& index, int& count)
{
    index = Hash(K);
    count = 0;
    while(H.elem[index].key != NULL_KEY)
    {
        if(H.elem[index].key == K) return SUCCESS;
        index = (index + 1) % m; // 线性探测再散列
        count++;                 // 冲突次数加1
    }
    return UNSUCCESS;
}


// 查找不成功时插入数据元素e到开放定址哈希表H中，并返回OK；
// 若冲突次数过大，则重建哈希表，算法9.18
int
InsertHash(HashTable& H, ElemType e)
{
    int index; // 哈希地址
    int count; // 冲突次数

    // 在哈希表H中查找关键码为e.key的元素
    if(SearchHash(H, e.key, index, count) == SUCCESS)
    {
        return DUPLICATE; // 重复
    }

    // 冲突次数过大
    if(count >= H.sizeindex)
    {
        return UNSUCCESS;
    }

    // 插入数据元素e到哈希表H中
    H.elem[index] = e; // 插入数据元素e到哈希表H中
    H.count++;
    return SUCCESS;
}


// 按哈希地址的顺序遍历哈希表
void
TraverseHash(HashTable H, void (*Vi)(int))
{
    printf("哈希地址0～%d\n", H.sizeindex - 1);
    for(int i = 0; i < H.sizeindex; i++)
    {
        // if(H.elem[i].key!=NULL_KEY) // 有数据
        Vi(i);
    }
    printf("\n");
    for(int i = 0; i < H.sizeindex; i++)
    {
        // if(H.elem[i].key!=NULL_KEY) // 有数据
        Vi(H.elem[i].key);
    }
    printf("\n");
}


void
print(int p)
{
    printf("%d\t", p);
}
