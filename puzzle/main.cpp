
// puzzle/main.cpp

#include <algorithm>
// #include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;


// 循环链式队列节点
typedef struct LinkQueueNode
{
    string         state;     // 队列中的状态数组
    LinkQueueNode* last_node; // 指向上一个节点
    LinkQueueNode* next_node; // 指向下一个节点
} LinkQueueNode;

typedef LinkQueueNode* LinkQueue; // 队列类型定义为指向 LinkQueueNode 的指针

void LinkQueueInit(LinkQueue& q);
void LinkQueueDelete(LinkQueue& q);
void LinkQueueEnqueue(LinkQueue& q, const string& state);
void LinkQueueDequeue(LinkQueue& q, string& front_array);
bool LinkQueueIsEmpty(const LinkQueue& q);

// 初始化队列
void
LinkQueueInit(LinkQueue& q)
{
    q = nullptr; // 初始化为空队列
}

// 删除队列
void
LinkQueueDelete(LinkQueue& q)
{
    while(q != nullptr)
    {
        LinkQueueNode* temp = q;

        q = q->next_node; // 移动到下一个节点

        delete temp;      // 删除当前节点
    }
}

// 入队操作
void
LinkQueueEnqueue(LinkQueue& q, const string& state)
{
    // 创建新节点并设置状态
    LinkQueueNode* new_node = new LinkQueueNode;
    new_node->state         = state;
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
        LinkQueueNode* tail = q->last_node;
        tail->next_node     = new_node;
        new_node->last_node = tail;
        new_node->next_node = q;
        q->last_node        = new_node;
    }
}

// 出队操作
void
LinkQueueDequeue(LinkQueue& q, string& front_array)
{
    if(q == nullptr) return;

    LinkQueueNode* front_node = q;
    front_array               = front_node->state;

    if(q->next_node == q)
    {
        // 只有一个节点
        delete front_node;
        q = nullptr;
    }
    else
    {
        LinkQueueNode* tail = q->last_node;
        LinkQueueNode* next = q->next_node;
        tail->next_node     = next;
        next->last_node     = tail;
        delete front_node;
        q = next;
    }
}

// 检查队列是否为空
bool
LinkQueueIsEmpty(const LinkQueue& q)
{
    return q == nullptr; // 如果队列为空，返回 true
}


const string TARGET = "123456780";

// 定义移动方向
const int move_vector[4][2] = {
    { +0, -1 }, // 上
    { +1, +0 }, // 右
    { +0, +1 }, // 下
    { -1, +0 }  // 左
};

// 打印 3x3 状态
void
printState(const string& state)
{
    for(int i = 0; i < 9; ++i)
    {
        printf("%c ", state[i]);
        if(i % 3 == 2) printf("\n");
    }
    printf("\n");
}


struct StateNode
{
    int    min_steps;  // 最小步数
    string last_state; // 上一个状态
};


// BFS + 路径恢复
int
bfs(const string& start, vector<string>& path)
{
    printf("开始 BFS 搜索...\n");
    unordered_map<string, StateNode> state_info; // 记录每个状态的信息
    LinkQueue                        q;          // 队列用于 BFS
    LinkQueueInit(q);                            // 初始化队列

    state_info[start] = { 0, "" };               // 初始状态信息
    LinkQueueEnqueue(q, start);                  // 将初始状态入队

    while(!LinkQueueIsEmpty(q))
    {
        string cur;
        LinkQueueDequeue(q, cur); // 当前状态出队

        // 如果当前状态是目标状态
        if(cur == TARGET)
        {
            // 从目标状态向前回溯路径
            string s = TARGET;
            while(s != start)
            {
                path.push_back(s);
                s = state_info[s].last_state; // 复制上一个状态
            }
            path.push_back(start);
            reverse(path.begin(), path.end()); // 反转路径，使其从起始状态到目标状态
            return state_info[cur].min_steps;
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
                string next = cur;
                swap(next[z], next[nz]);

                // 如果新状态未被访问过
                if(!state_info.count(next))
                {
                    state_info[next] = { state_info[cur].min_steps + 1, cur }; // 更新新状态的信息
                    LinkQueueEnqueue(q, next);                                 // 将新状态入队
                }
            }
        }
    }

    return -1; // 无法到达
}

int
main()
{
    string start;
    printf("请输入初始状态（0 表示空格，按行输入共 9 个数字）:\n");
    for(int i = 0; i < 9; ++i)
    {
        char ch;
        scanf(" %c", &ch);
        start += ch;
    }

    vector<string> path;

    int steps = bfs(start, path);

    if(steps == -1)
    {
        printf("无法达到目标状态。\n");
    }
    else
    {
        printf("最少步数为: %d\n", steps);
        printf("路径如下（共 %d 步）:\n\n", path.size());
        for(int i = 0; i < path.size(); ++i)
        {
            printf("Step %d:\n", i);
            printState(path[i]);
        }
    }

    return 0;
}
