
// puzzle/header.h

#ifndef PUZZLE_HEADER_H
#define PUZZLE_HEADER_H

#include <string>
// #include <unordered_map>


typedef std::string StateArray; // 定义一个状态数组为字符串类型，方便处理

// 定义操作方向的枚举类型
enum Operate
{
    UP    = 0,
    RIGHT = 1,
    DOWN  = 2,
    LEFT  = 3,
    NONE  = 4,
};

// 循环链式队列节点
// 队列类型定义为指向 LinkQueueNode 的指针
typedef struct LinkListNode
{
    int deep = 0;                             // 最小步数

    StateArray current_state = "";            // 当前状态
    StateArray last_state    = "";            // 上一个状态
    Operate    operate       = Operate::NONE; // 操作（移动方向）

    LinkListNode* last_node = nullptr;        // 指向上一个节点
    LinkListNode* next_node = nullptr;        // 指向下一个节点

    LinkListNode* last_map_node = nullptr;    // 指向上一个哈希表节点
    LinkListNode* next_map_node = nullptr;    // 指向下一个哈希表节点

} LinkListNode, *LinkListNode_ptr, *LinkList;

// typedef std::unordered_map<StateArray, LinkListNode> StateMap; // 定义状态映射，用于记录每个状态的信息


// 哈希表
struct StateMap
{
    LinkListNode** rcd; // （指向指针的指针）存放指针的数组

    int size;           // 哈希表的容量
    int count;          // 当前表中含有的记录个数

    int m;              // 除求余数，通过 m 计算哈希值
};


// 函数声明

void             LinkListInit(LinkList& list);                                    // 初始化队列
void             LinkListPushTail(LinkList& list, LinkListNode_ptr res_node);     // 入队操作
void             LinkListPopHead(LinkList& list, LinkListNode& dst_node);         // 出队操作
bool             LinkListIsEmpty(const LinkList& list);                           // 检查队列是否为空
LinkListNode_ptr LinkListContains(const LinkList& list, const StateArray& state); // 检查队列是否包含某个状态


// 哈希表相关函数声明

void             StateMapInit(StateMap& map, int size);                        // 初始化状态映射
void             StateMapDestroy(StateMap& map);                               // 销毁状态映射
LinkListNode_ptr StateMapSearch(const StateMap& map, const StateArray& state); // 查找状态
LinkListNode_ptr StateMapInsert(StateMap& map, const LinkListNode& node);      // 插入状态，返回新节点指针
unsigned         StateMapHash(const StateArray& state, int m);                 // 哈希函数


/*

注意！！！
元素的动态创建和销毁
都由 StateMap 的相关函数来处理
不由 LinkList 的相关函数来处理

*/


void printState(const StateArray& state);

int BFS(const StateArray& start, const StateArray& target, LinkList& path); // 广度优先搜索


#endif // PUZZLE_HEADER_H
