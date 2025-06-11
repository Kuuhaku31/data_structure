
// puzzle/main.cpp

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
LinkListInit(LinkQueue& q)
{
    q = nullptr; // 初始化为空队列
}

// 删除队列
void
LinkListDelete(LinkQueue& q)
{
    while(q != nullptr)
    {
        LinkListNode* temp = q;

        q = q->next_node; // 移动到下一个节点

        delete temp;      // 删除当前节点
    }
}

// 入队操作
void
LinkListPushTail(LinkQueue& q, const StateArray& state, MoveDirection operate)
{
    // 创建新节点并设置状态
    LinkListNode* new_node = new LinkListNode;
    new_node->state        = state;
    new_node->next_node    = nullptr;
    new_node->last_node    = nullptr;
    new_node->operate      = operate;

    // 如果队列为空，初始化新节点为队头和队尾
    if(q == nullptr)
    {
        new_node->next_node = new_node;
        new_node->last_node = new_node;
        q                   = new_node;
    }
    // 如果队列不为空，将新节点添加到队尾
    else
    {
        LinkListNode* tail  = q->last_node;
        tail->next_node     = new_node;
        new_node->last_node = tail;
        new_node->next_node = q;
        q->last_node        = new_node;
    }
}

// 出队操作
void
LinkListPopHead(LinkQueue& q, StateArray& front_array, MoveDirection& operate)
{
    if(q == nullptr) return;

    LinkListNode* front_node = q;
    front_array              = front_node->state;
    operate                  = front_node->operate;

    // 只有一个节点
    if(q->next_node == q)
    {

        delete front_node;
        q = nullptr;
    }
    // 有多个节点
    else
    {
        LinkListNode* tail = q->last_node;
        LinkListNode* next = q->next_node;
        tail->next_node    = next;
        next->last_node    = tail;
        delete front_node;
        q = next;
    }
}

// 检查队列是否为空
bool
LinkListIsEmpty(const LinkQueue& q)
{
    return q == nullptr; // 如果队列为空，返回 true
}


// BFS + 路径恢复
int
bfs(const StateArray& start, const StateArray target, LinkQueue& path)
{
    printf("开始 BFS 搜索...\n");
    std::unordered_map<StateArray, StateNode> state_info; // 记录每个状态的信息
    LinkQueue                                 queue;      // 队列用于 BFS
    LinkListInit(queue);                                  // 初始化队列

    state_info[start] = { 0, "" };                        // 初始状态信息
    LinkListPushTail(queue, start, MoveDirection::NONE);  // 将初始状态入队

    while(!LinkListIsEmpty(queue))
    {
        StateArray    cur;
        MoveDirection operate;
        LinkListPopHead(queue, cur, operate); // 当前状态出队

        // 如果当前状态是目标状态
        if(cur == target)
        {
            printf("找到目标状态！\n");

            // 从目标状态向前回溯路径
            StateArray s = cur;
            while(s != start)
            {
                LinkListPushTail(path, s, state_info[s].operate); // 将当前状态加入路径
                s = state_info[s].last_state;                     // 复制上一个状态
            }
            LinkListPushTail(path, start, MoveDirection::NONE);   // 将初始状态加入路径

            return state_info[cur].min_steps;                     // 返回最小步数
        }

        // 获取当前状态中 '0' 的位置
        int z = cur.find('0');
        int x = z % 3;
        int y = z / 3;

        // 尝试四个方向移动 '0'
        for(int i = 0; i < 4; ++i)
        {
            MoveDirection dir = static_cast<MoveDirection>(i);

            int nx = 0;
            int ny = 0;

            switch(dir)
            {
            case MoveDirection::UP:
                nx = x;
                ny = y - 1; // 向上移动
                break;
            case MoveDirection::RIGHT:
                nx = x + 1; // 向右移动
                ny = y;
                break;
            case MoveDirection::DOWN:
                nx = x;
                ny = y + 1; // 向下移动
                break;
            case MoveDirection::LEFT:
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
                StateArray next = cur;
                std::swap(next[z], next[nz]);

                // 如果新状态未被访问过
                if(!state_info.count(next))
                {
                    StateNode new_state = { state_info[cur].min_steps + 1, cur, dir };
                    state_info[next]    = new_state;    // 更新新状态的信息
                    LinkListPushTail(queue, next, dir); // 将新状态入队
                }
            }
        }
    }

    return -1; // 无法到达
}

int
main()
{
    StateArray start;
    printf("请输入初始状态（0 表示空格，按行输入共 9 个数字）:\n");
    for(int i = 0; i < 9; ++i)
    {
        char ch;
        scanf(" %c", &ch);
        start += ch;
    }

    LinkQueue path;
    LinkListInit(path);              // 初始化路径队列

    StateArray target = "123456780"; // 目标状态
    printf("目标状态为: %s\n", target.c_str());
    int steps = bfs(start, target, path);

    if(steps == -1)
    {
        printf("无法达到目标状态。\n");
    }
    else
    {
        printf("路径为:\n");
        printf("最少步数为: %d\n\n", steps);

        if(LinkListIsEmpty(path))
        {
            printf("路径为空。\n");
            return 0;
        }

        // 从循环队尾开始打印路径
        int           count   = 0;
        LinkListNode* current = path;
        do
        {
            current = current->last_node; // 向前移动到上一个节点

            printf("移动%d次：\n", count);
            printState(current->state);

            printf("上一个操作：\n");
            MoveDirection dir = current->operate;
            switch(dir)
            {
            case MoveDirection::UP:
                printf("向下划动\n");
                break;
            case MoveDirection::RIGHT:
                printf("向左划动\n");
                break;
            case MoveDirection::DOWN:
                printf("向上划动\n");
                break;
            case MoveDirection::LEFT:
                printf("向右划动\n");
                break;
            case MoveDirection::NONE:
                printf("无操作\n");
                break;
            }

            printf("\n");

            count++;
        } while(current != path); // 循环队列
    }

    return 0;
}
