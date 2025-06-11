
// puzzle/header.h

#ifndef PUZZLE_HEADER_H
#define PUZZLE_HEADER_H

#include <string>
#include <unordered_map>


typedef std::string StateArray; // 定义一个状态数组为字符串类型，方便处理

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
    int deep = -1;                            // 最小步数

    StateArray current_state = "";            // 当前状态
    StateArray last_state    = "";            // 上一个状态
    Operate    operate       = Operate::NONE; // 操作（移动方向）

    LinkListNode* last_node = nullptr;        // 指向上一个节点
    LinkListNode* next_node = nullptr;        // 指向下一个节点

} LinkListNode, *LinkQueue;


// 定义移动方向

void LinkListInit(LinkQueue& list);                                    // 初始化队列
void LinkListDelete(LinkQueue& list);                                  // 删除队列
void LinkListPushTail(LinkQueue& list, LinkListNode& res_node);        // 入队操作
void LinkListPopHead(LinkQueue& list, LinkListNode& dst_node);         // 出队操作
bool LinkListIsEmpty(const LinkQueue& list);                           // 检查队列是否为空
bool LinkListContains(const LinkQueue& list, const StateArray& state); // 检查队列是否包含某个状态

void printState(const StateArray& state);

int BFS(const StateArray& start, const StateArray& target, LinkQueue& path); // 广度优先搜索


#endif // PUZZLE_HEADER_H
