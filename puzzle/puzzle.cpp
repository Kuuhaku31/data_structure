
// puzzle/puzzle.cpp

#include "header.h"


// 打印 3x3 状态
void
printState(const StateArray& state)
{
    for(int i = 0; i < 9; ++i)
    {
        printf("%c ", state[i]);
        if(i % 3 == 2) printf("\n");
    }
}

// 初始化队列
void
LinkListInit(LinkList& q)
{
    q = nullptr; // 初始化为空队列
}

// 删除队列
void
LinkListDelete(LinkList& q)
{
    while(q != nullptr)
    {
        LinkListNode_ptr temp = q;

        q = q->next_node; // 移动到下一个节点

        delete temp;      // 删除当前节点
    }
}

// 入队操作
void
LinkListPushTail(LinkList& list, LinkListNode& res_node)
{
    // 创建新节点并设置状态
    LinkListNode_ptr new_node = new LinkListNode;

    new_node->deep          = res_node.deep;
    new_node->current_state = res_node.current_state;
    new_node->last_state    = res_node.last_state;
    new_node->operate       = res_node.operate;

    new_node->next_node = nullptr; // 固定初始化为 nullptr
    new_node->last_node = nullptr;


    // 如果队列为空，初始化新节点为队头和队尾
    if(list == nullptr)
    {
        new_node->next_node = new_node;
        new_node->last_node = new_node;
        list                = new_node;
    }
    // 如果队列不为空，将新节点添加到队尾
    else
    {
        LinkListNode_ptr tail = list->last_node;
        tail->next_node       = new_node;
        new_node->last_node   = tail;
        new_node->next_node   = list;
        list->last_node       = new_node;
    }
}

// 出队操作
void
LinkListPopHead(LinkList& list, LinkListNode& dst_node)
{
    if(list == nullptr) return;

    LinkListNode_ptr front_node = list;
    dst_node.deep               = front_node->deep;
    dst_node.current_state      = front_node->current_state;
    dst_node.last_state         = front_node->last_state;
    dst_node.operate            = front_node->operate;
    dst_node.next_node          = front_node->next_node;
    dst_node.last_node          = front_node->last_node;

    // 只有一个节点
    if(list->next_node == list)
    {
        delete front_node;
        list = nullptr;
    }
    // 有多个节点
    else
    {
        LinkListNode_ptr tail = list->last_node;
        LinkListNode_ptr next = list->next_node;
        tail->next_node       = next;
        next->last_node       = tail;

        delete front_node;
        list = next;
    }
}

LinkListNode_ptr
LinkListContains(const LinkList& list, const StateArray& state)
{
    LinkListNode_ptr current = list;
    if(current == nullptr) return nullptr; // 如果队列为空，返回 nullptr
    do
    {
        if(current->current_state == state) return current; // 找到匹配的状态
        current = current->next_node;
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


// BFS + 路径恢复
int
BFS(const StateArray& start_state, const StateArray& target_state, LinkList& path)
{
    printf("开始 BFS 搜索...\n");
    std::unordered_map<StateArray, LinkListNode> searched_nodes; // 记录每个状态的信息
    LinkList                                     node_queue;     // 队列用于 BFS
    LinkListInit(node_queue);                                    // 初始化队列


    LinkListNode start_node;
    start_node.deep          = 0;             // 初始状态步数为 0
    start_node.current_state = start_state;   // 设置初始状态
    start_node.operate       = Operate::NONE; // 初始状态没有操作
    LinkListPushTail(node_queue, start_node); // 将初始状态入队

    searched_nodes[start_state] = start_node; // 记录初始状态的信息

    while(!LinkListIsEmpty(node_queue))
    {
        LinkListNode cur_node;
        LinkListPopHead(node_queue, cur_node); // 当前状态出队

        // 如果当前状态是目标状态
        if(cur_node.current_state == target_state)
        {
            printf("找到目标状态！\n");

            // 从目标状态向前回溯路径
            StateArray s = cur_node.current_state;
            while(s != start_state)
            {
                LinkListNode state_info_node = searched_nodes[s]; // 获取当前状态的信息
                LinkListPushTail(path, state_info_node);          // 将当前状态加入路径
                s = searched_nodes[s].last_state;                 // 复制上一个状态
            }
            LinkListPushTail(path, start_node);                   // 将初始状态加入路径

            return cur_node.deep;                                 // 返回最小步数
        }

        // 获取当前状态中 '0' 的位置
        int z = cur_node.current_state.find('0');
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
                StateArray next = cur_node.current_state;
                std::swap(next[z], next[nz]);

                // 如果新状态未被访问过
                if(!searched_nodes.count(next))
                {
                    LinkListNode new_state;
                    new_state.deep          = searched_nodes[cur_node.current_state].deep + 1; // 更新步数
                    new_state.last_state    = cur_node.current_state;                          // 记录上一个状态
                    new_state.operate       = dir;                                             // 记录操作方向
                    new_state.current_state = next;                                            // 更新新状态

                    searched_nodes[next] = new_state;                                          // 更新新状态的信息
                    LinkListPushTail(node_queue, new_state);                                   // 将新状态入队
                }
            }
        }
    }

    return -1; // 无法到达
}
