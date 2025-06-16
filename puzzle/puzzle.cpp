
// puzzle/puzzle.cpp

#include "header.h"

#include <stdio.h>


// 设置 3x3 状态
void
StateSet(State& state, const char* str)
{
    for(int i = 0; i < 9; ++i) state.data[i] = str[i];
}

// 打印 3x3 状态
void
StatePrint(const State& state)
{
    for(int i = 0; i < 9; ++i)
    {
        printf("%c ", state.data[i]);
        if(i % 3 == 2) printf("\n");
    }
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
LinkListInit(LinkList& q)
{
    q = nullptr; // 初始化为空队列
}


// 入队操作
void
LinkListPushTail(LinkList& list, Node_ptr new_node)
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
void
LinkListPopHead(LinkList& list, Node& dst_node)
{
    if(list == nullptr) return;

    Node_ptr front_node     = list;
    dst_node.deep           = front_node->deep;
    dst_node.current_state  = front_node->current_state;
    dst_node.last_state     = front_node->last_state;
    dst_node.operate        = front_node->operate;
    dst_node.next_list_node = front_node->next_list_node;
    dst_node.last_list_node = front_node->last_list_node;

    // 只有一个节点
    if(list->next_list_node == list)
    {
        // delete front_node;
        list = nullptr;
    }
    // 有多个节点
    else
    {
        Node_ptr tail        = list->last_list_node;
        Node_ptr next        = list->next_list_node;
        tail->next_list_node = next;
        next->last_list_node = tail;
        list                 = next;

        // delete front_node;
        front_node->next_list_node = nullptr; // 清空前驱指针
        front_node->last_list_node = nullptr; // 清空后继指针
    }
}


Node_ptr
LinkListContains(const LinkList& list, const State& state)
{
    Node_ptr current = list;
    if(current == nullptr) return nullptr; // 如果队列为空，返回 nullptr
    do
    {
        if(StateEqual(current->current_state, state)) return current; // 使用 StateEqual 检查状态是否相等
        current = current->next_list_node;
    } while(current != list);

    // 如果遍历完队列都没有找到匹配的状态，返回 nullptr
    return nullptr;
}


// 检查队列是否为空
bool
LinkListIsEmpty(const LinkList& q)
{
    return q == nullptr; // 如果队列为空，返回 true
}


// 哈希表的哈希函数

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
    Node_ptr new_node       = new Node;
    *new_node               = node;               // 复制节点信息
    new_node->current_state = node.current_state; // 设置当前状态

    // 将新节点插入到哈希表中
    new_node->next_map_node = map[index]; // 新节点指向当前链表头
    map[index]              = new_node;   // 更新链表头为新节点

    // 返回新节点指针
    return new_node;
}


// BFS + 路径恢复
void
BFS(const State& start_state, const State& target_state, StateMap& state_map, LinkList& path)
{
    LinkList node_queue;      // 队列用于 BFS
    LinkListInit(node_queue); // 初始化队列

    Node start_node;
    start_node.current_state = start_state;                           // 设置初始状态
    Node_ptr start_node_ptr  = StateMapInsert(state_map, start_node); // 插入初始状态到哈希表
    LinkListPushTail(node_queue, start_node_ptr);                     // 将初始状态入队


    while(!LinkListIsEmpty(node_queue))
    {
        Node cur_node;
        LinkListPopHead(node_queue, cur_node); // 当前状态出队

        // 如果当前状态是目标状态
        if(StateEqual(cur_node.current_state, target_state))
        {
            printf("找到目标状态！\n");

            // 从目标状态向前回溯路径
            State s = cur_node.current_state;
            while(!StateEqual(s, start_state))
            {
                Node_ptr state_info_node = StateMapSearch(state_map, s); // 查找当前状态的信息
                LinkListPushTail(path, state_info_node);                 // 将当前状态加入路径
                s = state_info_node->last_state;                         // 复制上一个状态
            }
            LinkListPushTail(path, start_node_ptr);                      // 将初始状态加入路径

            break;
        }

        // 获取当前状态中 '0' 的位置
        int z = StateFindZero(cur_node.current_state);
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
            case Operate::UP:
                nx = x;
                ny = y - 1; // 向上移动
                break;
            case Operate::RIGHT:
                nx = x + 1; // 向右移动
                ny = y;
                break;
            case Operate::DOWN:
                nx = x;
                ny = y + 1; // 向下移动
                break;
            case Operate::LEFT:
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
                State next_state = cur_node.current_state;
                StateSwap(next_state, z, nz);

                // 如果新状态未被访问过
                if(!StateMapSearch(state_map, next_state))
                {
                    // 创建新节点并设置状态
                    Node next_node;
                    next_node.deep          = cur_node.deep + 1;      // 更新步数
                    next_node.current_state = next_state;             // 更新新状态
                    next_node.last_state    = cur_node.current_state; // 记录上一个状态
                    next_node.operate       = dir;                    // 记录操作方向

                    // 插入新状态到哈希表
                    Node_ptr new_map_node = StateMapInsert(state_map, next_node);

                    // 将新状态入队
                    LinkListPushTail(node_queue, new_map_node);
                }
            }
        }
    }
}
