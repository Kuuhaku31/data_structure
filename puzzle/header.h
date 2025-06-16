
// puzzle/header.h

#ifndef PUZZLE_HEADER_H
#define PUZZLE_HEADER_H


#define HASH_SIZE 67631 // 哈希表的大小，使用一个较大的质数作为容量


// 3x3 状态数组
struct State
{
    char data[9] = { 0 }; // 3x3 状态数组
};


// 定义操作方向的枚举类型
enum Operate
{
    UP    = 0,
    RIGHT = 1,
    DOWN  = 2,
    LEFT  = 3,
    NONE  = 4,
};


typedef struct Node Node;          // 节点

typedef Node* Node_ptr;            // 节点指针
typedef Node* LinkList;            // 链式队列
typedef Node* StateMap[HASH_SIZE]; // 哈希表，存储状态映射

struct Node
{
    int deep = 0;                      // 最小步数

    State   current_state;             // 当前状态
    State   last_state;                // 上一个状态
    Operate operate = Operate::NONE;   // 操作（移动方向）

    Node_ptr last_list_node = nullptr; // 指向上一个队列节点
    Node_ptr next_list_node = nullptr; // 指向下一个队列节点

    Node_ptr last_map_node = nullptr;  // 指向上一个哈希表节点
    Node_ptr next_map_node = nullptr;  // 指向下一个哈希表节点
};


// 函数声明

// 状态相关

void StateSet(State& state, const char* str);
void StatePrint(const State& state);
bool StateEqual(const State& a, const State& b);      // 检查两个状态是否相等
int  StateFindZero(const State& state);               // 查找状态中 '0' 的位置
void StateSwap(State& state, int index1, int index2); // 交换状态中两个位置的值

// 链式队列相关函数声明

void     LinkListInit(LinkList& list);                               // 初始化队列
void     LinkListPushTail(LinkList& list, Node_ptr res_node);        // 入队操作
void     LinkListPopHead(LinkList& list, Node& dst_node);            // 出队操作
bool     LinkListIsEmpty(const LinkList& list);                      // 检查队列是否为空
Node_ptr LinkListContains(const LinkList& list, const State& state); // 检查队列是否包含某个状态


// 哈希表相关函数声明

void     StateMapInit(StateMap& map);                             // 初始化状态映射
void     StateMapDestroy(StateMap& map);                          // 销毁状态映射
Node_ptr StateMapSearch(const StateMap& map, const State& state); // 查找状态
Node_ptr StateMapInsert(StateMap& map, const Node& node);         // 插入状态，返回新节点指针
unsigned StateMapHash(const State& state);                        // 哈希函数


/*

注意！！！
元素的动态创建和销毁
都由 StateMap 的相关函数来处理
不由 LinkList 的相关函数来处理

*/


// 广度优先搜索函数声明
// 参数说明：
// start - 初始状态
// target - 目标状态
// state_map - 状态映射，用于记录每个状态的信息
// path - 用于存储找到的路径
int BFS(const State& start, const State& target, StateMap& state_map, LinkList& path); // 广度优先搜索


#endif // PUZZLE_HEADER_H
