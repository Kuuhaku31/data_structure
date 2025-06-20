
// puzzle/main.cpp

#include "header.h"

#include <ctime>
#include <stdio.h>


// 保存解法到文件
void
SavePathToFile(const LinkQueue& path, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if(!file)
    {
        printf("无法打开文件 %s 进行写入。\n", filename);
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
        Operate dir = current->node->operate;
        switch(dir)
        {
        case Operate::ZERO_UP:
            fprintf(file, "向下划动:\n");
            break;
        case Operate::ZERO_RIGHT:
            fprintf(file, "向左划动:\n");
            break;
        case Operate::ZERO_DOWN:
            fprintf(file, "向上划动:\n");
            break;
        case Operate::ZERO_LEFT:
            fprintf(file, "向右划动:\n");
            break;
        case Operate::ZERO_NONE:
            fprintf(file, "无操作:\n");
            break;
        }
        for(int i = 0; i < 9; ++i)
        {
            fprintf(file, "| %c ", current->node->current_state.data[i]);
            if(i % 3 == 2) fprintf(file, "|\n"); // 每三列换行
        }
        fprintf(file, "\n");

        count++;
    } while(current != path.queue_front); // 循环队列

    printf("路径已保存到 %s\n", filename);
    fclose(file);
}


// 保存状态映射到文件
void
SaveMapToFile(const StateMap& state_map, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if(!file)
    {
        printf("无法打开文件 %s 进行写入。\n", filename);
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

    printf("状态映射已保存到 %s\n", filename);
    fclose(file);
}


// 保存叶子节点到文件
void
SaveLeafsToFile(const LinkQueue& leafs_queue, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if(!file)
    {
        printf("无法打开文件 %s 进行写入。\n", filename);
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

    printf("叶子节点信息已保存到 %s\n", filename);
    fclose(file);
}


void
PrintStateMapInfo(const StateMap& state_map)
{
    printf("\n=== 状态映射信息 ===\n");
    printf("总节点数: %d\n", state_map.node_count);
    printf("叶子节点数: %d\n", state_map.leaf_count);

    int zero_count = 0;    // 空的哈希桶的数量

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

    printf("哈希桶总数: %d\n", HASH_SIZE);
    printf("空的哈希桶数量: %d\n", zero_count);
    printf("哈希桶利用率: %.2f%%\n", (static_cast<double>(HASH_SIZE - zero_count) / HASH_SIZE) * 100.0);
    printf("平均每个哈希桶节点数: %.2f\n", average);
    printf("哈希桶节点数方差: %.2f\n", variance);
    printf("=== 状态映射信息结束 ===\n\n");
}


// puzzle.exe < 根状态 > < 目标状态 >
int
main(int argc, char* argv[])
{
    printf("3x3 拼图求解器\n\n");

    bool need_find_path = false; // 是否需要查找路径

    State     root_state;        // 根状态
    State     target_state;      // 目标状态
    LinkQueue path;              // 路径队列
    LinkQueue leafs;             // 叶子节点队列
    StateMap  state_map;         // 映射表

    // LinkQueueInit(path);         // 初始化路径队列
    // LinkQueueInit(leafs);        // 初始化叶子节点队列


    // 处理命令行参数
    {
        switch(argc)
        {
        case 1: // 仅构建映射表，默认 123456780 为根状态
            root_state.StateSet("123456780");
            printf("未指定目标状态，使用默认根状态 123456780\n");
            break;

        case 2: // 仅构建映射表，根据 argv[1] 为根状态
            root_state.StateSet(argv[1]);
            printf("使用根状态: %s\n", argv[1]);
            break;

        default: // 构建映射表，然后根据 argv[2] 查找路径
            root_state.StateSet(argv[1]);
            target_state.StateSet(argv[2]);
            need_find_path = true;
            printf("使用根状态: %s\n", argv[1]);
            printf("目标状态: %s\n", argv[2]);
            break;
        }
    }


    // 开始 BFS 搜索，构建映射表
    {
        printf("开始 BFS 搜索...\n");
        clock_t start_time = clock(); // 记录开始时间

        // BFS(start_state, target_state, state_map, path);
        BuildTree(root_state, state_map, leafs);                                            // 构建状态树

        clock_t end_time     = clock();                                                     // 记录结束时间
        double  elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC; // 计算耗时
        printf("BFS 搜索完成，耗时: %.2f 秒\n", elapsed_time);
    }


    // 处理结果
    {
        PrintStateMapInfo(state_map);              // 打印状态映射信息

        SaveLeafsToFile(leafs, "leafs.txt");       // 保存叶子节点到文件
        SaveMapToFile(state_map, "state_map.txt"); // 保存状态映射到文件
    }


    // 根据映射表找到 path
    if(need_find_path)
    {
        printf("开始查找从根状态到目标状态的路径...\n");
        FindPath(state_map, target_state, path); // 从状态映射中找到路径
        SavePathToFile(path, "path.txt");        // 保存路径到文件
    }


    // 清理资源
    {
        // LinkQueueDestroy(path);  // 销毁路径队列
        // LinkQueueDestroy(leafs); // 销毁叶子节点队列
    }


    return 0;
}
