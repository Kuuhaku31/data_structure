
// puzzle/header.h

#ifndef PUZZLE_HEADER_H
#define PUZZLE_HEADER_H

#include <string>
#include <unordered_map>


typedef std::string StateArray; // 定义一个状态数组为字符串类型，方便处理

// 循环链式队列节点
// 队列类型定义为指向 LinkQueueNode 的指针
typedef struct LinkListNode
{
    StateArray    state;     // 队列中的状态数组
    LinkListNode* last_node; // 指向上一个节点
    LinkListNode* next_node; // 指向下一个节点

} LinkListNode, *LinkQueue;


typedef struct StateNode
{
    int        min_steps;   // 最小步数
    StateArray last_state;  // 上一个状态
    char       change_code; // 记录状态变化的代码（移动方向）

} StateNode;

// 定义移动方向
enum MoveDirection
{
    UP    = 0,
    RIGHT = 1,
    DOWN  = 2,
    LEFT  = 3,
};

void LinkListInit(LinkQueue& list);                              // 初始化队列
void LinkListDelete(LinkQueue& list);                            // 删除队列
void LinkListPushTail(LinkQueue& list, const StateArray& state); // 压入列表尾部
void LinkListPopHead(LinkQueue& list, StateArray& front_array);  // 弹出列表头部
bool LinkListIsEmpty(const LinkQueue& list);                     // 检查队列是否为空

void printState(const StateArray& state);


#endif // PUZZLE_HEADER_H
