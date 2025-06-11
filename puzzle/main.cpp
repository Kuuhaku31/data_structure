
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
    printf("\n");
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
LinkListPushTail(LinkQueue& q, const StateArray& state)
{
    // 创建新节点并设置状态
    LinkListNode* new_node = new LinkListNode;
    new_node->state        = state;
    new_node->next_node = new_node->last_node = nullptr;

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
LinkListPopHead(LinkQueue& q, StateArray& front_array)
{
    if(q == nullptr) return;

    LinkListNode* front_node = q;
    front_array              = front_node->state;

    if(q->next_node == q)
    {
        // 只有一个节点
        delete front_node;
        q = nullptr;
    }
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


const StateArray TARGET = "123456780";

// 定义移动方向
const int move_vector[4][2] = {
    { +0, -1 }, // 上
    { +1, +0 }, // 右
    { +0, +1 }, // 下
    { -1, +0 }  // 左
};

// BFS + 路径恢复
int
bfs(const StateArray& start, LinkQueue& path)
{
    printf("开始 BFS 搜索...\n");
    std::unordered_map<StateArray, StateNode> state_info; // 记录每个状态的信息
    LinkQueue                                 q;          // 队列用于 BFS
    LinkListInit(q);                                      // 初始化队列

    state_info[start] = { 0, "" };                        // 初始状态信息
    LinkListPushTail(q, start);                           // 将初始状态入队

    while(!LinkListIsEmpty(q))
    {
        StateArray cur;
        LinkListPopHead(q, cur); // 当前状态出队

        // 如果当前状态是目标状态
        if(cur == TARGET)
        {
            printf("找到目标状态！\n");

            // 从目标状态向前回溯路径
            StateArray s = TARGET;
            while(s != start)
            {
                LinkListPushTail(path, s);    // 将当前状态加入路径
                s = state_info[s].last_state; // 复制上一个状态
            }
            LinkListPushTail(path, start);

            return state_info[cur].min_steps; // 返回最小步数
        }

        // 获取当前状态中 '0' 的位置
        int z = cur.find('0');
        int x = z % 3;
        int y = z / 3;

        // 尝试四个方向移动 '0'
        for(int i = 0; i < 4; ++i)
        {
            int nx = x + move_vector[i][0];
            int ny = y + move_vector[i][1];

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
                    state_info[next] = { state_info[cur].min_steps + 1, cur }; // 更新新状态的信息
                    LinkListPushTail(q, next);                                 // 将新状态入队
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
    LinkListInit(path); // 初始化路径队列

    int steps = bfs(start, path);

    if(steps == -1)
    {
        printf("无法达到目标状态。\n");
    }
    else
    {
        printf("路径为:\n");
        printf("最少步数为: %d\n", steps);

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

            count++;
        } while(current != path); // 循环队列
    }

    return 0;
}
