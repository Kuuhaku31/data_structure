
// puzzle/puzzle.cpp

#include "header.h"


Operate
int_to_operate(int dir)
{
    switch(dir)
    {
    case 0: return Operate::ZERO_UP;
    case 1: return Operate::ZERO_RIGHT;
    case 2: return Operate::ZERO_DOWN;
    case 3: return Operate::ZERO_LEFT;
    default: return Operate::ZERO_NONE; // 默认返回 NONE
    }
}

Node::Node(const State& state)
{
    StateCopy(current_state, state);
}

void
NodeCopy(Node& dest, const Node& src)
{
    dest.deep = src.deep;
    StateCopy(dest.current_state, src.current_state);
    StateCopy(dest.last_state, src.last_state);
    dest.operate = src.operate;

    dest.last_list_node = src.last_list_node; // 复制队列指针
    dest.next_list_node = src.next_list_node; // 复制队列指针

    dest.last_map_node = src.last_map_node;   // 复制哈希表指针
    dest.next_map_node = src.next_map_node;   // 复制哈希表指针
}


void
StateCopy(State& dest, const State& src)
{
    for(int i = 0; i < 9; ++i) dest.data[i] = src.data[i]; // 逐个复制状态数组中的字符
}

// 设置 3x3 状态
void
StateSet(State& state, const char* str)
{
    for(int i = 0; i < 9; ++i) state.data[i] = str[i];
}


// 检查两个状态是否相等
bool
StateEqual(const State& a, const State& b)
{
    for(int i = 0; i < 9; ++i)
    {
        if(a.data[i] != b.data[i]) return false; // 如果有任何一个字符不相等，返回 false
    }
    return true;                                 // 所有字符都相等，返回 true
}


// 查找状态中 '0' 的位置
int
StateFindZero(const State& state)
{
    for(int i = 0; i < 9; ++i)
    {
        if(state.data[i] == '0') return i; // 返回 '0' 的索引位置
    }
    return -1;                             // 如果没有找到 '0'，返回 -1
}


// 交换状态中两个位置的值
void
StateSwap(State& state, int index1, int index2)
{
    char temp          = state.data[index1]; // 临时变量存储 index1 的值
    state.data[index1] = state.data[index2]; // 将 index2 的值赋给 index1
    state.data[index2] = temp;               // 将临时变量的值赋给 index2
}


// 初始化队列
void
LinkQueueInit(LinkQueue& q)
{
    q = nullptr; // 初始化为空队列
}


// 入队操作
void
LinkQueuePushTail(LinkQueue& list, Node_ptr new_node)
{
    // 固定初始化为 nullptr
    new_node->next_list_node = nullptr;
    new_node->last_list_node = nullptr;

    // 如果队列为空，初始化新节点为队头和队尾
    if(list == nullptr)
    {
        new_node->next_list_node = new_node;
        new_node->last_list_node = new_node;
        list                     = new_node;
    }
    // 如果队列不为空，将新节点添加到队尾
    else
    {
        Node_ptr tail            = list->last_list_node;
        tail->next_list_node     = new_node;
        new_node->last_list_node = tail;
        new_node->next_list_node = list;
        list->last_list_node     = new_node;
    }
}


// 出队操作
Node_ptr
LinkQueuePopHead(LinkQueue& list)
{
    if(list == nullptr) return nullptr;

    Node_ptr front_node = list;

    // 如果队列只有一个节点，直接清空队列
    if(list->next_list_node == list)
    {
        list = nullptr;
    }
    // 如果队列有多个节点
    else
    {
        list                                       = list->next_list_node;       // 更新队头后移
        list->last_list_node                       = front_node->last_list_node; // 更新队头的上一个节点指针
        front_node->last_list_node->next_list_node = list;                       // 更新队尾指针
    }

    front_node->next_list_node = nullptr;
    front_node->last_list_node = nullptr;

    return front_node;
}


// 检查队列是否为空
bool
LinkQueueIsEmpty(const LinkQueue& q)
{
    return q == nullptr; // 如果队列为空，返回 true
}


// 计算状态的哈希值
unsigned
StateMapHash(const State& state)
{
    unsigned hash_value = 0;
    for(char c : state.data)
    {
        hash_value = (hash_value * 114514 + c) % HASH_SIZE; // 使用简单的哈希函数
    }
    return hash_value;
}


// 初始化状态映射
void
StateMapInit(StateMap& map)
{
    // 分配指针数组内存
    for(int i = 0; i < HASH_SIZE; ++i)
    {
        map[i] = nullptr; // 初始化每个指针为 nullptr
    }
}


// 销毁状态映射
void
StateMapDestroy(StateMap& map)
{
    // 遍历每个链表，释放节点内存
    for(int i = 0; i < HASH_SIZE; ++i)
    {
        Node_ptr temp    = nullptr;
        Node_ptr current = map[i];
        while(current)
        {
            temp    = current;
            current = current->next_map_node; // 移动到下一个节点
            delete temp;                      // 删除当前节点
        }
    }
}


// 查找状态
Node_ptr
StateMapSearch(const StateMap& map, const State& state)
{
    unsigned index = StateMapHash(state); // 计算哈希值

    // 遍历链表查找状态
    Node_ptr current = map[index];
    while(current)
    {
        if(StateEqual(current->current_state, state)) return current; // 找到匹配的状态
        current = current->next_map_node;                             // 移动到下一个节点
    }
    // 如果遍历完链表都没有找到匹配的状态，返回 nullptr
    return nullptr;
}


// 插入状态
// 返回新节点指针
Node_ptr
StateMapInsert(StateMap& map, const Node& node)
{
    unsigned index = StateMapHash(node.current_state); // 计算哈希值

    // 检查是否已存在相同状态
    // 如果已存在相同状态，直接返回
    bool     found   = false;
    Node_ptr current = map[index];
    while(current)
    {
        if(StateEqual(current->current_state, node.current_state)) // 比较当前状态
        {
            found = true;
            break;
        }
        current = current->next_map_node; // 移动到下一个节点
    }
    if(found) return current;             // 如果已存在相同状态，返回对应节点指针

    // 创建新节点并设置状态
    Node_ptr new_node       = new Node(node.current_state); // 创建新节点
    *new_node               = node;                         // 复制节点信息
    new_node->current_state = node.current_state;           // 设置当前状态

    // 将新节点插入到哈希表中
    new_node->next_map_node = map[index]; // 新节点指向当前链表头
    map[index]              = new_node;   // 更新链表头为新节点

    // 返回新节点指针
    return new_node;
}


// BFS + 路径恢复
void
BFS(const State& start_state, const State& target_state, StateMap& state_map, LinkQueue& path)
{
    LinkQueue node_queue;                                             // 队列用于 BFS
    LinkQueueInit(node_queue);                                        // 初始化队列

    Node start_node(start_state);                                     // 创建初始状态节点
    start_node.current_state = start_state;                           // 设置初始状态
    Node_ptr start_node_ptr  = StateMapInsert(state_map, start_node); // 插入初始状态到哈希表
    LinkQueuePushTail(node_queue, start_node_ptr);                    // 将初始状态入队
    while(!LinkQueueIsEmpty(node_queue))
    {
        // 当前状态出队
        Node_ptr current_node = LinkQueuePopHead(node_queue);

        // 如果当前状态是目标状态
        if(StateEqual(current_node->current_state, target_state))
        {
            // 从目标状态向前回溯路径
            Node_ptr path_node = current_node; // 从当前节点开始回溯路径
            while(path_node != nullptr)
            {
                LinkQueuePushTail(path, path_node);                           // 将当前节点加入路径
                path_node = StateMapSearch(state_map, path_node->last_state); // 回溯到上一个状态
            }

            break;
        }

        // 获取当前状态中 '0' 的位置
        int z = StateFindZero(current_node->current_state);
        int x = z % 3;
        int y = z / 3;

        // 尝试四个方向移动 '0'
        for(int i = 0; i < 4; ++i)
        {
            Operate dir = static_cast<Operate>(i);

            int nx = 0;
            int ny = 0;

            switch(dir)
            {
            case Operate::ZERO_UP:
                nx = x;
                ny = y - 1; // 向上移动
                break;
            case Operate::ZERO_RIGHT:
                nx = x + 1; // 向右移动
                ny = y;
                break;
            case Operate::ZERO_DOWN:
                nx = x;
                ny = y + 1; // 向下移动
                break;
            case Operate::ZERO_LEFT:
                nx = x - 1; // 向左移动
                ny = y;
                break;
            }

            // 检查新位置是否在 3x3 网格内
            if(nx >= 0 && nx < 3 && ny >= 0 && ny < 3)
            {
                // 新位置的索引
                int nz = ny * 3 + nx;

                // 生成新状态
                State next_state = current_node->current_state;
                StateSwap(next_state, z, nz);

                // 如果新状态未被访问过
                if(!StateMapSearch(state_map, next_state))
                {
                    // 创建新节点并设置状态
                    Node next_node(next_state);                            // 创建新节点
                    next_node.deep          = current_node->deep + 1;      // 更新步数
                    next_node.current_state = next_state;                  // 更新新状态
                    next_node.last_state    = current_node->current_state; // 记录上一个状态
                    next_node.operate       = dir;                         // 记录操作方向

                    // 插入新状态到哈希表
                    Node_ptr new_map_node = StateMapInsert(state_map, next_node);

                    // 将新状态入队
                    LinkQueuePushTail(node_queue, new_map_node);
                }
            }
        }
    }
}


bool
StateMapInsert(StateMap& map, Node_ptr& node_ptr, const State& state, int& node_count)
{
    unsigned index = StateMapHash(state); // 计算哈希值

    // 检查是否已存在相同状态
    // 如果已存在相同状态，直接返回
    bool found = false;
    node_ptr   = map[index];
    while(node_ptr)
    {
        if(StateEqual(node_ptr->current_state, state)) // 比较当前状态
        {
            found = true;
            break;
        }
        node_ptr = node_ptr->next_map_node;
    }
    if(found) return false;                        // 如果已存在相同状态，返回 false
    else                                           // 如果不存在相同状态，则创建新节点并插入到哈希表中
    {
        node_ptr                = new Node(state); // 创建新节点
        node_ptr->next_map_node = map[index];      // 新节点指向当前链表头
        map[index]              = node_ptr;        // 更新链表头为新节点

        node_count++;                              // 统计节点数量
        return true;
    }
}


static inline bool
_create_new_state(const State& current_state, State& new_state, Operate dir)
{
    // 计算 '0' 的位置
    // 获取当前状态中 '0' 的位置
    int z = StateFindZero(current_state);
    int x = z % 3;
    int y = z / 3;

    // 根据方向移动 '0'
    switch(dir)
    {
    case Operate::ZERO_UP:
        y -= 1; // 向上移动
        break;
    case Operate::ZERO_RIGHT:
        x += 1; // 向右移动
        break;
    case Operate::ZERO_DOWN:
        y += 1; // 向下移动
        break;
    case Operate::ZERO_LEFT:
        x -= 1; // 向左移动
        break;
    }

    // 检查新位置是否在 3x3 网格内
    if(x >= 0 && x < 3 && y >= 0 && y < 3)
    {
        // 新位置的索引
        int nz = y * 3 + x;

        // 生成新状态
        StateCopy(new_state, current_state); // 复制当前状态
        StateSwap(new_state, z, nz);         // 交换 '0' 和新位置的值

        return true;
    }
    else return false;
}
void
BuildTree(const State& start_state, StateMap& state_map, LinkQueue& leafs, int& node_count, int& leaf_count)
{
    LinkQueue node_queue;      // 队列用于 BFS
    LinkQueueInit(node_queue); // 初始化队列

    Node_ptr start_node_ptr = nullptr;
    StateMapInsert(state_map, start_node_ptr, start_state, node_count); // 插入初始状态到哈希表
    LinkQueuePushTail(node_queue, start_node_ptr);                      // 将初始状态入队

    // 开始 BFS 搜索
    while(!LinkQueueIsEmpty(node_queue))
    {
        // 当前状态出队
        Node_ptr current_node = LinkQueuePopHead(node_queue);

        // 尝试四个方向移动 '0'
        bool is_leaf = true; // 标记是否为叶子节点
        for(int i = 0; i < 4; ++i)
        {
            // 如果无法创建新状态
            Operate dir = int_to_operate(i);
            State   next_state;
            if(!_create_new_state(current_node->current_state, next_state, dir)) continue;

            // 如果新状态未被访问过
            Node_ptr new_map_node = nullptr;
            if(StateMapInsert(state_map, new_map_node, next_state, node_count))
            {
                // 创建新节点并设置状态
                new_map_node->deep          = current_node->deep + 1;      // 更新步数
                new_map_node->current_state = next_state;                  // 更新新状态
                new_map_node->last_state    = current_node->current_state; // 记录上一个状态
                new_map_node->operate       = dir;                         // 记录操作方向

                // 将新状态入队
                LinkQueuePushTail(node_queue, new_map_node);

                is_leaf = false; // 只要有一个子节点，就不是叶子节点
            }
        }
        if(is_leaf)                                 // 如果当前节点没有子节点，则将其视为叶子节点
        {
            LinkQueuePushTail(leafs, current_node); // 将当前节点添加到叶子节点队列
            leaf_count++;                           // 统计叶子节点数量
        }
    }

    // BFS 完成后，node_queue 中的节点已全部处理完毕
    // leafs 中包含所有叶子节点，state_map 中包含所有访问过的
}
