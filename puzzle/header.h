
// puzzle/header.h

#ifndef PUZZLE_HEADER_H
#define PUZZLE_HEADER_H


#define HASH_SIZE 99991            // 哈希表的大小，使用一个较大的质数作为容量

typedef struct Node Node;          // 节点

typedef Node* Node_ptr;            // 节点指针
typedef Node* LinkQueue;           // 链式队列
typedef Node* StateMap[HASH_SIZE]; // 哈希表，存储状态映射

// 3x3 状态数组
struct State
{
    char data[9] = { 0 }; // 3x3 状态数组
};

// 定义操作方向的枚举类型
enum Operate
{
    ZERO_UP    = 0,
    ZERO_RIGHT = 1,
    ZERO_DOWN  = 2,
    ZERO_LEFT  = 3,
    ZERO_NONE  = 4,
};

Operate int_to_operate(int dir);

// 节点结构体
struct Node
{
    Node(const State& state);

    int deep = 0;                         // 最小步数

    State   current_state;                // 当前状态
    State   last_state;                   // 上一个状态
    Operate operate = Operate::ZERO_NONE; // 操作（移动方向）

    Node_ptr last_list_node = nullptr;    // 指向上一个队列节点
    Node_ptr next_list_node = nullptr;    // 指向下一个队列节点

    Node_ptr last_map_node = nullptr;     // 指向上一个哈希表节点
    Node_ptr next_map_node = nullptr;     // 指向下一个哈希表节点
};


/* 函数声明 */
void     NodeCopy(Node& dest, const Node& src);                                          // 复制节点
void     StateCopy(State& dest, const State& src);                                       // 复制状态
void     StateSet(State& state, const char* str);                                        // 设置状态
bool     StateEqual(const State& a, const State& b);                                     // 检查两个状态是否相等
int      StateFindZero(const State& state);                                              // 查找状态中 '0' 的位置
void     StateSwap(State& state, int index1, int index2);                                // 交换状态中两个位置的值
void     LinkQueueInit(LinkQueue& list);                                                 // 初始化队列
void     LinkQueuePushTail(LinkQueue& list, Node_ptr res_node);                          // 入队操作
Node_ptr LinkQueuePopHead(LinkQueue& list);                                              // 出队操作
bool     LinkQueueIsEmpty(const LinkQueue& list);                                        // 检查队列是否为空
void     StateMapInit(StateMap& map);                                                    // 初始化状态映射
void     StateMapDestroy(StateMap& map);                                                 // 销毁状态映射
Node_ptr StateMapSearch(const StateMap& map, const State& state);                        // 查找状态
Node_ptr StateMapInsert(StateMap& map, const Node& node);                                // 插入状态，返回新节点指针
unsigned StateMapHash(const State& state);                                               // 哈希函数

void BFS(const State& start, const State& target, StateMap& state_map, LinkQueue& path); // 广度优先搜索


/* Build Tree */

/*
### 构建状态树
使用广度优先搜索（BFS）算法从起始状态（1234567890）到所有可能的目标状态
参数：
  - `start`: 起始状态
  - `state_map`: 状态映射，用于存储已访问的状态
  - `leafs`: 用于存储所有叶子节点（目标状态）
    - `node_count`: 用于记录节点总数
*/
void BuildTree(const State& start_state, StateMap& state_map, LinkQueue& leafs, int& node_count, int& leaf_count);

// 尝试插入状态
// 如果状态已存在，则返回 false
// 否则插入新状态并返回 true，并且把新节点指针赋值给 node_ptr
bool StateMapInsert(StateMap& map, Node_ptr& node_ptr, const State& state, int& node_count);


#endif // PUZZLE_HEADER_H
