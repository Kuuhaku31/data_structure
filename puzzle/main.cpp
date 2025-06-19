
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
    LinkQueueNode_ptr current = path;
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
    } while(current != path); // 循环队列

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
        fprintf(file, "哈希桶[%04d] -> ", i);

        // 打印链表
        StateNode_ptr current = state_map[i];
        while(current)
        {
            for(int j = 0; j < 9; ++j) fprintf(file, "%c", current->current_state.data[j]);
            fprintf(file, " -> ");
            current = current->next_map_node; // 移动到下一个节点
        }

        fprintf(file, "NULL\n");
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
    LinkQueueNode_ptr current = leafs_queue;
    do
    {
        // 将当前状态写入文件
        for(int i = 0; i < 9; ++i)
        {
            fprintf(file, "%c", current->node->current_state.data[i]);
        }
        // 保存 deep
        fprintf(file, " %d ", current->node->deep);
        fprintf(file, "\n");

        current = current->last; // 向前移动到上一个节点
    } while(current != leafs_queue); // 循环队列

    printf("叶子节点信息已保存到 %s\n", filename);
    fclose(file);
}


// puzzle.exe < 根状态 > < 目标状态 >
int
main(int argc, char* argv[])
{
    printf("3x3 拼图求解器\n\n");

    bool need_find_path = false; // 是否需要查找路径

    int       node_count = 0;    // 统计节点数量
    int       leaf_count = 0;    // 统计叶子节点数量
    State     root_state;        // 根状态
    State     target_state;      // 目标状态
    LinkQueue path;              // 路径队列
    LinkQueue leafs;             // 叶子节点队列
    StateMap  state_map;         // 映射表

    LinkQueueInit(path);
    LinkQueueInit(leafs);
    StateMapInit(state_map);


    // 处理命令行参数
    {
        switch(argc)
        {
        case 1: // 仅构建映射表，默认 123456780 为根状态
            StateSet(root_state, "123456780");
            printf("未指定目标状态，使用默认根状态 123456780\n");
            break;

        case 2: // 仅构建映射表，根据 argv[1] 为根状态
            StateSet(root_state, argv[1]);
            printf("使用根状态: %s\n", argv[1]);
            break;

        default: // 构建映射表，然后根据 argv[2] 查找路径
            StateSet(root_state, argv[1]);
            StateSet(target_state, argv[2]);
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
        BuildTree(root_state, state_map, leafs, node_count, leaf_count);                    // 构建状态树

        clock_t end_time     = clock();                                                     // 记录结束时间
        double  elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC; // 计算耗时
        printf("BFS 搜索完成，耗时: %.2f 秒\n", elapsed_time);
    }


    // 处理结果
    {
        printf("总节点数: %d\n", node_count);
        printf("叶子节点数: %d\n", leaf_count);
        SaveLeafsToFile(leafs, "leafs.txt");       // 保存叶子节点到文件
        SaveMapToFile(state_map, "state_map.txt"); // 保存状态映射到文件
    }


    // 根据映射表找到 path
    if(need_find_path)
    {
        FindPath(state_map, target_state, path); // 从状态映射中找到路径
        SavePathToFile(path, "path.txt");        // 保存路径到文件
    }

    StateMapDestroy(state_map); // 销毁状态映射

    return 0;
}
