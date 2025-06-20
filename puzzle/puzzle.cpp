
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


StateNode::StateNode(const State& state)
    : deep(0)
    , operate(Operate::ZERO_NONE)
    , next_map_node(nullptr)
{
    StateCopy(current_state, state);
}


LinkQueueNode::LinkQueueNode(StateNode_ptr ptr)
    : node(ptr)
    , last(nullptr)
    , next(nullptr)
{
}


void
StateCopy(State& dest, const State& src)
{
    for(int i = 0; i < 9; ++i) dest.data[i] = src.data[i]; // 逐个复制状态数组中的字符
}


void
StateSet(State& state, const char* str)
{
    for(int i = 0; i < 9; ++i) state.data[i] = str[i];
}


bool
StateEqual(const State& a, const State& b)
{
    for(int i = 0; i < 9; ++i)
    {
        if(a.data[i] != b.data[i]) return false; // 如果有任何一个字符不相等，返回 false
    }
    return true;                                 // 所有字符都相等，返回 true
}


int
StateFindZero(const State& state)
{
    for(int i = 0; i < 9; ++i)
    {
        if(state.data[i] == '0') return i; // 返回 '0' 的索引位置
    }
    return -1;                             // 如果没有找到 '0'，返回 -1
}


LinkQueue::LinkQueue()
{
    this->queue_front = nullptr; // 初始化为空队列
    // this->queue_rear  = nullptr; // 初始化为空队列
    this->size = 0; // 初始化队列大小
}


LinkQueue::~LinkQueue()
{
    // 销毁循环队列
    if(this->queue_front == nullptr) return;         // 如果队列为空，直接返回

    LinkQueueNode_ptr current   = this->queue_front; // 从队头开始遍历
    LinkQueueNode_ptr next_node = nullptr;           // 用于保存下一个节点
    do
    {
        next_node = current->next; // 保存下一个节点
        delete current;            // 删除当前节点
        current = next_node;       // 移动到下一个节点
    } while(current != this->queue_front); // 循环直到回到队头

    this->queue_front = nullptr; // 最后将队列指针置为 nullptr
    // this->queue_rear  = nullptr;
}


void
LinkQueue::LinkQueuePushTail(StateNode_ptr new_state_node)
{
    LinkQueueNode_ptr new_list_node = new LinkQueueNode(new_state_node); // 创建新的链表节点

    // 如果队列为空，初始化新节点为队头和队尾
    if(this->queue_front == nullptr)
    {
        new_list_node->next = new_list_node; // 新节点指向自己，形成循环
        new_list_node->last = new_list_node; // 新节点的上一个节点指向自己
        this->queue_front   = new_list_node; // 将队列指针指向新节点
    }
    // 如果队列不为空，将新节点添加到队尾
    else
    {
        LinkQueueNode_ptr tail  = this->queue_front->last; // 获取队尾节点
        tail->next              = new_list_node;           // 将新节点添加到队尾
        new_list_node->last     = tail;                    // 新节点的上一个节点指向队尾节点
        new_list_node->next     = this->queue_front;       // 新节点的下一个节点指向队头节点
        this->queue_front->last = new_list_node;           // 队头节点的上一个节点指向新节点
    }
}


StateNode_ptr
LinkQueue::LinkQueuePopHead()
{
    if(this->queue_front == nullptr) return nullptr;

    LinkQueueNode_ptr front_node       = this->queue_front;
    StateNode_ptr     front_state_node = this->queue_front->node;

    // 如果队列只有一个节点，直接清空队列
    if(this->queue_front->next == this->queue_front)
    {
        this->queue_front = nullptr;
        // this->queue_rear  = nullptr;
    }
    // 如果队列有多个节点
    else
    {
        this->queue_front       = this->queue_front->next; // 更新队头后移
        this->queue_front->last = front_node->last;        // 更新队头的上一个节点指针
        front_node->last->next  = this->queue_front;       // 更新队尾指针
    }

    delete front_node; // 删除队头节点

    return front_state_node;
}


bool
LinkQueue::LinkQueueIsEmpty() const
{
    return this->queue_front == nullptr; // 如果队列为空，返回 true
}


unsigned
StateMap::StateMapHash(const State& state) const
{
    unsigned hash_value = 0;
    for(char c : state.data)
    {
        hash_value = (hash_value * 114514 + c) % HASH_SIZE; // 使用简单的哈希函数
    }
    return hash_value;
}


StateMap::StateMap()
{
    // 分配指针数组内存
    for(int i = 0; i < HASH_SIZE; ++i)
    {
        this->map[i].node_ptr   = nullptr; // 初始化每个指针为 nullptr
        this->map[i].node_count = 0;       // 初始化每个哈希桶的节点数量为 0
    }
}


StateMap::~StateMap()
{
    StateNode_ptr temp    = nullptr;
    StateNode_ptr current = nullptr;

    // 遍历每个链表，释放节点内存
    for(int i = 0; i < HASH_SIZE; ++i)
    {
        current = this->map[i].node_ptr;
        while(current)
        {
            temp    = current;
            current = current->next_map_node; // 移动到下一个节点
            delete temp;                      // 删除当前节点
        }
    }
}


/*
### 查找状态
如果找到匹配的状态，返回指向该状态节点的指针
如果未找到匹配的状态，返回 `nullptr`
*/
StateNode_ptr
StateMap::StateMapSearch(const State& state) const
{
    unsigned index = StateMapHash(state); // 计算哈希值

    // 遍历链表查找状态
    StateNode_ptr current = this->map[index].node_ptr;
    while(current)
    {
        if(StateEqual(current->current_state, state)) return current; // 找到匹配的状态
        current = current->next_map_node;                             // 移动到下一个节点
    }
    // 如果遍历完链表都没有找到匹配的状态，返回 nullptr
    return nullptr;
}

/*
### 插入状态
如果状态已存在，则返回 `false`
否则插入新状态，`node_count` 加 1 并返回 `true`
并且把新节点指针赋值给 `node_ptr`
*/
bool
StateMap::StateMapInsert(StateNode_ptr& node_ptr, const State& state)
{
    unsigned index = StateMapHash(state); // 计算哈希值

    // 检查是否已存在相同状态
    // 如果已存在相同状态，直接返回
    bool found = false;
    node_ptr   = this->map[index].node_ptr;
    while(node_ptr)
    {
        if(StateEqual(node_ptr->current_state, state)) // 比较当前状态
        {
            found = true;
            break;
        }
        node_ptr = node_ptr->next_map_node;
    }
    if(found) return false;                                    // 如果已存在相同状态，返回 false
    else                                                       // 如果不存在相同状态，则创建新节点并插入到哈希表中
    {
        node_ptr                  = new StateNode(state);      // 创建新节点
        node_ptr->next_map_node   = this->map[index].node_ptr; // 新节点指向当前链表头
        this->map[index].node_ptr = node_ptr;                  // 更新链表头为新节点

        this->map[index].node_count++;                         // 统计节点数量
        this->node_count++;                                    // 更新总节点数量
        return true;
    }
}


void
FindPath(const StateMap& state_map, const State& target_state, LinkQueue& path)
{
    StateNode_ptr target_node = state_map.StateMapSearch(target_state); // 查找目标状态节点
    if(!target_node) return;                                            // 如果目标状态不存在，直接返回

    // 从目标状态向前回溯路径
    StateNode_ptr path_node = target_node; // 从目标节点开始回溯路径
    while(path_node != nullptr)
    {
        path.LinkQueuePushTail(path_node);                           // 将当前节点加入路径
        path_node = state_map.StateMapSearch(path_node->last_state); // 回溯到上一个状态
    }
}


static inline bool
_create_new_state(const State& current_state, State& new_state, Operate dir)
{
    // 计算 '0' 的位置
    // 获取当前状态中 '0' 的位置

    int z = -0;
    for(int i = 0; i < 9; ++i)
    {
        if(current_state.data[i] == '0') z = i; // 返回 '0' 的索引位置
    }

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
        // 交换 '0' 和新位置的值
        char temp          = new_state.data[z];
        new_state.data[z]  = new_state.data[nz];
        new_state.data[nz] = temp;

        return true;
    }
    else return false;
}
void
BuildTree(const State& start_state, StateMap& state_map, LinkQueue& leafs)
{
    LinkQueue node_queue; // 队列用于 BFS
    // LinkQueueInit(node_queue); // 初始化队列

    StateNode_ptr start_node_ptr = nullptr;
    state_map.StateMapInsert(start_node_ptr, start_state); // 插入初始状态到哈希表
    node_queue.LinkQueuePushTail(start_node_ptr);          // 将初始状态入队

    // 开始 BFS 搜索
    while(!node_queue.LinkQueueIsEmpty())
    {
        // 当前状态出队
        StateNode_ptr current_node = node_queue.LinkQueuePopHead();

        // 尝试四个方向移动 '0'
        bool is_leaf = true;
        for(int i = 0; i < 4; ++i)
        {
            // 如果无法创建新状态
            Operate dir = int_to_operate(i);
            State   next_state;
            if(!_create_new_state(current_node->current_state, next_state, dir)) continue;

            // 如果新状态未被访问过
            StateNode_ptr new_map_node = nullptr;
            if(state_map.StateMapInsert(new_map_node, next_state))
            {
                // 创建新节点并设置状态
                new_map_node->deep          = current_node->deep + 1;      // 更新步数
                new_map_node->current_state = next_state;                  // 更新新状态
                new_map_node->last_state    = current_node->current_state; // 记录上一个状态
                new_map_node->operate       = dir;                         // 记录操作方向

                // 将新状态入队
                node_queue.LinkQueuePushTail(new_map_node);

                is_leaf = false; // 只要有一个子节点，就不是叶子节点
            }
        }
        if(is_leaf)
        {
            leafs.LinkQueuePushTail(current_node); // 将当前节点添加到叶子节点队列
            state_map.leaf_count++;                // 更新叶子节点数量
        }
    }

    // BFS 完成后，node_queue 中的节点已全部处理完毕
    // leafs 中包含所有叶子节点，state_map 中包含所有访问过的状态
}
