
// hash.cpp

#include "hash.h"


int m = 0; // 全局变量, H(key) = key % m


// 哈希函数的基本操作函数
// 操作结果：构造一个空的哈希表
void
InitHashTable(HashTable& H)
{                              // 操作结果：构造一个空的哈希表
    int i;
    H.count = 0;               // 当前元素个数为0
    scanf("%d", &H.sizeindex); // 哈希表存储容量
    H.elem = (ElemType*)malloc(H.sizeindex * sizeof(ElemType));
    for(i = 0; i < H.sizeindex; i++)
        H.elem[i].key = NULL_KEY; // 未填记录的标志
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
    p = (p + 1) % m;
}


// 在开放定址哈希表H中查找关键码为K的元素，若查找成功，以 index 指示待查数据
// 元素在表中位置，并返回 SUCCESS；否则，以 index 指示插入位置，并返回 UNSUCCESS
// count 用以计冲突次数，其初值置零，供建表插入时参考。
int
SearchHash(HashTable H, KeyType K, int& index, int& count)
{
    int start = Hash(K);
    index     = start;
    count     = 1;
    while(count <= H.sizeindex)
    {
        if(H.elem[index].key == K) return SUCCESS;               // 查找成功
        else if(H.elem[index].key == NULL_KEY) return UNSUCCESS; // 查找失败，未找到
        else
        {
            collision(index, count); // 线性探测再散列
            count++;
        }
    }
    return UNSUCCESS; // 冲突次数过大，返回UNSUCCESS
}


// 查找不成功时插入数据元素e到开放定址哈希表H中，并返回OK；
// 若冲突次数过大，则重建哈希表，算法9.18
int
InsertHash(HashTable& H, ElemType e)
{
    int index, count;
    int res = SearchHash(H, e.key, index, count);
    if(res == SUCCESS) // 查找成功，元素已存在
    {
        return SUCCESS;
    }
    else if(res == UNSUCCESS) // 查找不成功，插入位置
    {
        H.elem[index] = e;    // 插入数据元素
        H.count++;
        return SUCCESS;
    }
    return UNSUCCESS;
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

int
Find(HashTable H, KeyType K, int& index)
{
    int count = 1;
    int start = Hash(K);
    index     = start;
    while(count <= H.sizeindex)
    {
        if(H.elem[index].key == K)
        {
            printf("哈希地址为%d，第%d次查找成功\n", index, count);
            return SUCCESS;
        }
        else if(H.elem[index].key == NULL_KEY)
        {
            printf("哈希地址为%d，第%d次查找为空\n", index, count);
            return UNSUCCESS;
        }
        else
        {
            printf("哈希地址为%d，第%d次冲突\n", index, count);
            collision(index, count);
            count++;
        }
    }
    return UNSUCCESS;
}

void
print(int p)
{
    printf("%d\t", p);
}
