
// puzzle/main.cpp

#include "header.h"

#include <ctime>
#include <stdio.h>
#include <string>


std::string
state_to_str(const State& state)
{
    std::string str;
    for(int i = 0; i < 9; ++i) str += state.data[i];
    return str;
}


/*
解析成形如：
| 1 | 5 | 2 |
| 0 | 4 | 3 |
| 7 | 8 | 6 |
*/
std::string
prase_state(const State& state)
{
    std::string res = "";
    for(int i = 0; i < 9; ++i)
    {
        res += "| ";
        res += state.data[i];
        res += " ";
        if(i % 3 == 2) res += "|\n";
    }
    return res;
}


void
print_state(const State& state)
{
    printf("\033[1;32;47m");
    for(int i = 0; i < 9; ++i)
    {
        printf("| %c ", state.data[i]);
        if(i % 3 == 2) printf("|\033[0m\n\033[1;32;47m");
    }
    printf("\033[0m\n");
}


std::string
prase_operate_dir(Operate dir)
{
    switch(dir)
    {
    case Operate::ZERO_UP: return "向下滑动";
    case Operate::ZERO_RIGHT: return "向左划动";
    case Operate::ZERO_DOWN: return "向上划动";
    case Operate::ZERO_LEFT: return "向右划动";
    case Operate::ZERO_NONE: return "无操作";
    default: return "未知操作";
    }
}


// 保存解法到文件
void
SavePathToFile(const LinkQueue& path, const char* filename)
{
    printf("\033[1;32m[文件保存]: \033[0m");

    if(path.size <= 0)
    {
        printf("\033[1;33m路径队列为空，无法保存。\n\033[0m");
        return;
    }

    FILE* file = fopen(filename, "w");
    if(!file)
    {
        printf("\033[1;31m无法打开文件 %s 进行写入。\n\033[0m", filename);
        return;
    }

    // 处理循环队列
    // 从循环队尾开始打印路径
    int               count   = 0;
    LinkQueueNode_ptr current = path.queue_front;
    do
    {
        current = current->last; // 向前移动到上一个节点

        // 将当前状态写入文件
        fprintf(file, "第 %d 步:\n", current->node->deep);

        std::string dir_str = prase_operate_dir(current->node->operate);
        fprintf(file, (dir_str + ":\n").c_str());
        std::string res = prase_state(current->node->current_state);
        fprintf(file, res.c_str());

        fprintf(file, "\n");

        count++;
    } while(current != path.queue_front); // 循环队列

    fclose(file);
    printf("已将 路径信息 保存到 %s\n", filename);
}


// 保存状态映射到文件
void
SaveMapToFile(const StateMap& state_map, const char* filename)
{
    printf("\033[1;32m[文件保存]: \033[0m");

    if(state_map.node_count <= 0)
    {
        printf("\033[1;33m状态映射表为空，无法保存。\n\033[0m");
        return;
    }

    FILE* file = fopen(filename, "w");
    if(!file)
    {
        printf("\033[1;31m无法打开文件 %s 进行写入。\n\033[0m", filename);
        return;
    }

    // 打印哈希表的每个桶
    for(int i = 0; i < HASH_SIZE; ++i)
    {
        fprintf(file, "哈希桶[%04d] [count: %d] -> ", i, state_map.map[i].node_count);

        // 打印链表
        StateNode_ptr current = state_map.map[i].node_ptr;
        while(current)
        {
            fprintf(file, "[");
            for(int j = 0; j < 9; ++j) fprintf(file, "%c", current->current_state.data[j]);
            fprintf(file, " from: ");
            for(int j = 0; j < 9; ++j) fprintf(file, "%c", current->last_state.data[j]);
            fprintf(file, "] -> ");
            current = current->next_map_node; // 移动到下一个节点
        }

        fprintf(file, "[NULL]\n");
    }

    fclose(file);
    printf("已将 状态映射信息 保存到 %s\n", filename);
}


// 保存叶子节点到文件
void
SaveLeafsToFile(const LinkQueue& leafs_queue, const char* filename)
{
    printf("\033[1;32m[文件保存]: \033[0m");

    if(leafs_queue.size <= 0)
    {
        printf("\033[1;33m叶子节点队列为空，无法保存。\n\033[0m");
        return;
    }

    FILE* file = fopen(filename, "w");
    if(!file)
    {
        printf("\033[1;31m无法打开文件 %s 进行写入。\n\033[0m", filename);
        return;
    }

    // 处理循环队列
    LinkQueueNode_ptr current = leafs_queue.queue_front;
    do
    {
        // 将当前状态写入文件
        fprintf(file, "state: ");
        for(int i = 0; i < 9; ++i)
        {
            fprintf(file, "%c", current->node->current_state.data[i]);
        }
        // 保存 deep
        fprintf(file, " deep: %d ", current->node->deep);
        fprintf(file, "\n");

        current = current->last; // 向前移动到上一个节点
    } while(current != leafs_queue.queue_front); // 循环队列

    fclose(file);
    printf("已将 叶子节点信息 保存到 %s\n", filename);
}


void
PrintStateMapInfo(const StateMap& state_map)
{
    int zero_count = 0;    // 空的哈希桶的数量
    int max_deep   = 0;    // 最大深度

    double average  = 0.0; // 平均每个哈希桶的节点数
    double variance = 0.0; // 方差
    for(int i = 0; i < HASH_SIZE; i++)
    {
        if(state_map.map[i].node_count > 0)
        {
            average += state_map.map[i].node_count;
        }
        else
        {
            zero_count++; // 统计空的哈希桶数量
        }

        // 遍历每个哈希桶，找到最大深度
        StateNode_ptr current = state_map.map[i].node_ptr;
        while(current)
        {
            if(current->deep > max_deep)
            {
                max_deep = current->deep;
            }
            current = current->next_map_node;
        }
    }
    average /= HASH_SIZE;

    for(int i = 0; i < HASH_SIZE; i++)
    {
        if(state_map.map[i].node_count > 0)
        {
            double diff  = state_map.map[i].node_count - average;
            variance    += diff * diff;
        }
    }
    variance /= HASH_SIZE;

    std::string title = "\033[1;34m========= 状态映射信息 =========\033[0m";
    printf("\n%s", title.c_str());
    printf("\n\033[46;4m哈希桶总数             | %-10d  |\033[0m", HASH_SIZE);
    printf("\n\033[46;4m空的哈希桶数量         | %-10d  |\033[0m", zero_count);
    printf("\n\033[46;4m哈希桶利用率           | %-10.2f%% |\033[0m", (static_cast<double>(HASH_SIZE - zero_count) / HASH_SIZE) * 100.0);
    printf("\n\033[46;4m平均每个哈希桶节点数   | %-10.2f  |\033[0m", average);
    printf("\n\033[46;4m哈希桶节点数方差       | %-10.2f  |\033[0m", variance);
    printf("\n\033[46;4m总共的可能局面数（9!） | %-10d  |\033[0m", 362880); // 9! = 362880
    printf("\n\033[46;4m总节点数               | %-10d  |\033[0m", state_map.node_count);
    printf("\n\033[46;4m叶子节点数             | %-10d  |\033[0m", state_map.leaf_count);
    printf("\n\033[46;4m最大深度               | %-10d  |\033[0m", max_deep);
    printf("\n");
    printf("%s\n\n", title.c_str());
}


// 打印启动状态
void
PrintStartupInfo(const State& root_state, const State& target_state)
{
    std::string title = "\033[1;34m=========== 启动状态 ===========\033[0m";
    printf("\n%s\n", title.c_str());

    printf("根状态:\n");
    print_state(root_state);

    if(target_state.data[0] == 0) printf("目标状态: 未设置\n");
    else
    {
        printf("目标状态:\n");
        print_state(target_state);
    }

    printf("%s\n\n", title.c_str());
}


// 从 args.txt 文件中读取启动参数
void
LoadArgsFromFile(const char* filename, State& root_state, State& target_state)
{
    printf("\033[1;32m[文件读取]: \033[0m");

    FILE* file = fopen(filename, "r");
    if(!file)
    {
        printf("\033[1;31m无法打开文件 %s 进行读取，使用默认根状态 123456780\n\033[0m", filename);
        root_state.StateSet("123456780"); // 设置默认根状态
        return;
    }

    char line[256];
    if(fgets(line, sizeof(line), file)) root_state.StateSet(line);
    if(fgets(line, sizeof(line), file)) target_state.StateSet(line);

    fclose(file);
    printf("已从 %s 读取参数\n", filename);
}


// puzzle.exe < 根状态 > < 目标状态 >
int
main(int argc, char* argv[])
{
    printf("\n\033[1;32m3x3 拼图求解器\033[0m\n");


    State     root_state;   // 根状态
    State     target_state; // 目标状态
    LinkQueue path;         // 路径队列
    LinkQueue leafs;        // 叶子节点队列
    StateMap  state_map;    // 映射表


    // 处理命令行参数
    {
        switch(argc)
        {
        case 2: // 仅构建映射表，根据 argv[1] 为根状态
            root_state.StateSet(argv[1]);

            break;

        case 3: // 构建映射表，然后根据 argv[2] 查找路径
            root_state.StateSet(argv[1]);
            target_state.StateSet(argv[2]);

            break;

        default: // 从 args.txt 文件中读取启动参数
            LoadArgsFromFile("args.txt", root_state, target_state);

            break;
        }

        PrintStartupInfo(root_state, target_state); // 打印启动状态
    }


    // 开始 BFS 搜索，构建映射表
    {
        printf("\033[33mBFS 搜索开始...\033[0m\n");
        clock_t start_time = clock(); // 记录开始时间

        // BFS(start_state, target_state, state_map, path);
        BuildTree(root_state, state_map, leafs);                                            // 构建状态树

        clock_t end_time     = clock();                                                     // 记录结束时间
        double  elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC; // 计算耗时
        printf("\033[33mBFS 搜索完成: \033[0m耗时: %.2f 秒 ", elapsed_time);

        // 根据映射表找到 path
        if(target_state.data[0] == 0) printf("目标状态未设置，跳过路径查找。\n");
        else
        {
            FindPath(state_map, target_state, path); // 从状态映射中找到路径

            if(path.size <= 0) printf("不存在从根状态到目标状态的路径。\n");
            else printf("找到路径，路径长度为 %d\n", path.size - 1);
        }
    }


    // 处理结果
    {
        PrintStateMapInfo(state_map);              // 打印状态映射信息

        SaveLeafsToFile(leafs, "leafs.txt");       // 保存叶子节点到文件
        SaveMapToFile(state_map, "state_map.txt"); // 保存状态映射到文件
        SavePathToFile(path, "path.txt");          // 保存路径到文件
    }


    return 0;
}
