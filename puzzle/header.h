
// puzzle/header.h

#ifndef PUZZLE_HEADER_H
#define PUZZLE_HEADER_H

#include <string>
#include <unordered_map>


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

} LinkListNode, *LinkListNode_ptr, *LinkList;

typedef std::unordered_map<StateArray, LinkListNode> StateMap; // 定义状态映射，用于记录每个状态的信息


// 函数声明

void             LinkListInit(LinkList& list);                                    // 初始化队列
void             LinkListDelete(LinkList& list);                                  // 删除队列
void             LinkListPushTail(LinkList& list, LinkListNode& res_node);        // 入队操作
void             LinkListPopHead(LinkList& list, LinkListNode& dst_node);         // 出队操作
bool             LinkListIsEmpty(const LinkList& list);                           // 检查队列是否为空
LinkListNode_ptr LinkListContains(const LinkList& list, const StateArray& state); // 检查队列是否包含某个状态

void printState(const StateArray& state);

int BFS(const StateArray& start, const StateArray& target, LinkList& path); // 广度优先搜索


#endif // PUZZLE_HEADER_H
