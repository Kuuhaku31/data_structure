
// puzzle/header.h

#ifndef PUZZLE_HEADER_H
#define PUZZLE_HEADER_H


#define HASH_SIZE 99991                     // 哈希表的大小，使用一个较大的质数作为容量

typedef struct StateNode     StateNode;     // 节点
typedef struct LinkQueueNode LinkQueueNode; // 链式队列节点

typedef StateNode*     StateNode_ptr;       // 节点指针
typedef LinkQueueNode* LinkQueueNode_ptr;   // 链式队列节点指针
typedef LinkQueueNode* LinkQueue;           // 链式队列


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

// 将整数转换为操作方向
Operate int_to_operate(int dir);

// 节点结构体
struct StateNode
{
    StateNode(const State& state);

    int           deep;          // 最小步数
    State         current_state; // 当前状态
    State         last_state;    // 上一个状态
    Operate       operate;       // 操作（移动方向）
    StateNode_ptr next_map_node; // 指向下一个哈希表节点
};

struct LinkQueueNode
{
    LinkQueueNode(StateNode_ptr ptr);

    StateNode_ptr     node; // 指向状态节点
    LinkQueueNode_ptr last; // 指向上一个队列节点
    LinkQueueNode_ptr next; // 指向下一个队列节点
};


struct StateMap
{
    struct
    {
        StateNode_ptr node_ptr;
        int           node_count; // 这个哈希桶中存储的节点数量
    } map[HASH_SIZE];             // 哈希表，存储状态节点指针

    int node_count = 0;           // 当前节点数量
    int leaf_count = 0;           // 当前叶子节点数量
};


/* 函数声明 */

void StateCopy(State& dest, const State& src);                            // 复制状态
void StateSet(State& state, const char* str);                             // 设置状态
bool StateEqual(const State& a, const State& b);                          // 检查两个状态是否相等

void          LinkQueueInit(LinkQueue& list);                             // 初始化队列
void          LinkQueueDestroy(LinkQueue& list);                          // 销毁队列
void          LinkQueuePushTail(LinkQueue& list, StateNode_ptr res_node); // 入队操作
StateNode_ptr LinkQueuePopHead(LinkQueue& list);                          // 出队操作
bool          LinkQueueIsEmpty(const LinkQueue& list);                    // 检查队列是否为空

void          StateMapInit(StateMap& map);                                // 初始化状态映射
void          StateMapDestroy(StateMap& map);                             // 销毁状态映射
StateNode_ptr StateMapSearch(const StateMap& map, const State& state);    // 查找状态
unsigned      StateMapHash(const State& state);                           // 哈希函数
bool          StateMapInsert(StateMap& map, StateNode_ptr& node_ptr, const State& state);


/* Build Tree */

/*
### 构建状态树
使用广度优先搜索（BFS）算法从起始状态（1234567890）到所有可能的目标状态
参数：
  - `start`: 起始状态
  - `state_map`: 状态映射，用于存储已访问的状态
  - `leafs`: 用于存储所有叶子节点（目标状态）
  - `node_count`: 用于记录节点总数
  - `leaf_count`: 用于记录叶子节点总数
*/
void BuildTree(const State& start_state, StateMap& state_map, LinkQueue& leafs, int& leaf_count);

/*
### 利用 BFS 算法构建状态树
参数：
 - `state_map`: 状态映射，用于存储已访问的状态
 - `target_state`: 目标状态
 - `path`: 用于存储找到的路径
*/
void FindPath(const StateMap& state_map, const State& target_state, LinkQueue& path);


#endif // PUZZLE_HEADER_H
