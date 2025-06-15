
// hashlink.cpp

#include <stdio.h>
#include <stdlib.h>

#define N 20         // 数据元素个数

typedef int KeyType; // 设关键字域为整型

struct ElemType
{
    KeyType key;
};

typedef struct Node
{
    ElemType     r;
    struct Node* next;
} Node, *Node_ptr;

typedef struct HashTable
{
    Node** rcd;   // （指向指针的指针）存放指针的数组
    int    size;  // 哈希表的容量
    int    count; // 当前表中含有的记录个数
} HashTable;

int      InitHash(HashTable& H, int size);                   // 初始化哈希表
int      DestroyHash(HashTable& H);                          // 销毁哈希表
Node_ptr SearchHash(const HashTable& H, KeyType key);        // 查找
int      InsertHash(HashTable& H, ElemType e);               // 插入
int      DeleteHash(HashTable& H, KeyType key, ElemType& e); // 删除
void     HashTraverse(HashTable H);                          // 遍历
unsigned Hash(KeyType K, int m);

int
main()
{
    HashTable h;
    KeyType   k;
    ElemType  r[N] = { 0 }, e;
    // int       m, n, i, j;
    // scanf("%d", &m); // H(key) = key % m
    // scanf("%d", &n); // 数据的个数
    // InitHash(h, m);
    // for(i = 0; i < n; i++)
    // {
    //     scanf("%d", &r[i].key);
    // }
    // for(i = 0; i < n; i++)
    // { // 插入前N-1个记录
    //     InsertHash(h, r[i]);
    // }
    // printf("采用链地址法得到的哈希表为：\n");
    // HashTraverse(h);
    // // printf("请输入要删除的整数值：\n");
    // scanf("%d", &k);
    // if(DeleteHash(h, k, e))
    // {
    //     printf("删除元素后哈希表为：\n");
    //     HashTraverse(h);
    // }
    // else
    //     printf("删除元素失败。\n");

    // DestroyHash(h);
    printf("欢迎使用哈希表程序！\n");
    scanf("输入表的大小：%d", &h.size);
    InitHash(h, h.size);

    bool flag = true;
    do
    {
        printf("输入要插入的整数值（输入负数结束）：");
        scanf("%d", &k);
        if(k < 0) // 输入负数结束
        {
            flag = false;
            break;
        }
        r[0].key = k; // 将输入的整数值存入r[0]
        if(InsertHash(h, r[0]))
        {
            printf("插入成功！当前哈希表为：\n");
            HashTraverse(h);
        }
        else
        {
            printf("插入失败，可能是重复的键。\n");
        }

    } while(flag);
}

// 一个简单的哈希函数
unsigned
Hash(KeyType K, int m)
{
    return K % m;
}

int
InitHash(HashTable& H, int size)
{
    H.rcd = (Node**)malloc(size * sizeof(Node*));
    if(H.rcd != NULL)
    {
        for(int i = 0; i < size; i++)
        {
            H.rcd[i] = NULL;
        }
        H.size  = size;
        H.count = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

int
DestroyHash(HashTable& H)
{
    if(H.rcd != NULL)
    {
        Node *np, *nt;
        for(int i = 0; i < H.size; i++)
        {
            np = H.rcd[i];
            while(np != NULL)
            {
                nt = np;
                np = np->next;
                free(nt);
            }
        }
        H.size  = 0;
        H.count = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

Node_ptr
SearchHash(const HashTable& H, KeyType key)
{
    Node_ptr np = H.rcd[Hash(key, H.size)];
    while(np)
    {
        if(key == np->r.key) return np;
        np = np->next;
    }
    return NULL;
}

int
InsertHash(HashTable& H, ElemType e)
{
    Node_ptr* np = &H.rcd[Hash(e.key, H.size)];

    // 如果该键不存在
    // 则直接插入
    if(*np != NULL)
    {
        // 如果该键已存在
        // 插入到链表的末尾
        while(*np != NULL)
        {
            if((*np)->r.key == e.key) return 0; // 重复
            np = &((*np)->next);
        }
    }

    *np         = new Node;
    (*np)->r    = e;
    (*np)->next = NULL;

    H.count++;

    return 1;
}

int
DeleteHash(HashTable& H, KeyType key, ElemType& e)
{
    /*********BEGIN*********/
    int   idx  = Hash(key, H.size);
    Node* p    = H.rcd[idx];
    Node* prev = NULL;
    while(p != NULL)
    {
        if(p->r.key == key)
        {
            // 找到要删除的节点
            e = p->r;
            if(prev == NULL)
            {
                // 删除的是链表头
                H.rcd[idx] = p->next;
            }
            else
            {
                prev->next = p->next;
            }
            free(p);
            H.count--;
            return 1;
        }
        prev = p;
        p    = p->next;
    }
    return 0; // 未找到
    /**********END**********/
}

void
HashTraverse(HashTable H)
{
    if(H.rcd != NULL)
    {
        Node *np, *nq;
        for(int i = 0; i < H.size; i++)
        {
            printf("%d:  ", i);
            if(H.rcd[i])
            {
                printf("%d  ", H.rcd[i]->r.key);
                Node* curr = H.rcd[i]->next;
                while(curr)
                {
                    printf("-> %d  ", curr->r.key);
                    curr = curr->next;
                }
                printf("\n");
            }
            else
                printf("-\n");
        }
    }
}