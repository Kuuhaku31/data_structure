
// puzzle/main.cpp

#include "header.h"

#include <ctime>
#include <stdio.h>


// 保存解法到文件
void
SavePathToFile(const LinkList& path, const char* filename)
{
    if(path == nullptr) return;

    FILE* file = fopen(filename, "w");
    if(!file)
    {
        printf("无法打开文件 %s 进行写入。\n", filename);
        return;
    }

    // 处理循环队列
    // 从循环队尾开始打印路径
    int   count   = 0;
    Node* current = path;
    do
    {
        current = current->last_list_node; // 向前移动到上一个节点

        // 将当前状态写入文件
        fprintf(file, "第 %d 步:\n", current->deep);
        Operate dir = current->operate;
        switch(dir)
        {
        case Operate::UP:
            fprintf(file, "向下划动:\n");
            break;
        case Operate::RIGHT:
            fprintf(file, "向左划动:\n");
            break;
        case Operate::DOWN:
            fprintf(file, "向上划动:\n");
            break;
        case Operate::LEFT:
            fprintf(file, "向右划动:\n");
            break;
        case Operate::NONE:
            fprintf(file, "无操作:\n");
            break;
        }
        for(int i = 0; i < 9; ++i)
        {
            fprintf(file, "| %c ", current->current_state.data[i]);
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
        Node_ptr current = state_map[i];
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


// puzzle.exe < 初始状态 > < 目标状态（默认为 123456780） >
int
main(int argc, char* argv[])
{
    int      deep = -1;
    State    start_state;
    State    target_state;
    LinkList path;
    StateMap state_map;
    LinkListInit(path);      // 初始化路径队列
    StateMapInit(state_map); // 初始化状态映射

    // 设置初始状态和目标状态
    {
        if(argc < 2)
        {
            printf("请提供初始状态字符串（例如: 123456780）\n");
            return 1;
        }
        StateSet(start_state, argv[1]);                   // 设置初始状态
        if(argc < 3) StateSet(target_state, "123456780"); // 设置默认目标状态
        else StateSet(target_state, argv[2]);             // 设置目标状态
    }

    // 开始 BFS 搜索
    {
        printf("开始 BFS 搜索...\n");
        clock_t start_time = clock(); // 记录开始时间

        deep = BFS(start_state, target_state, state_map, path);

        clock_t end_time     = clock();                                                     // 记录结束时间
        double  elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC; // 计算耗时
        printf("BFS 搜索完成，耗时: %.2f 秒\n", elapsed_time);
    }

    // 处理结果
    {
        if(deep == -1) printf("无法达到目标状态\n");
        else
        {
            printf("最少步数为: %d\n\n", deep);
            SavePathToFile(path, "path.txt");      // 保存路径到文件
        }
        SaveMapToFile(state_map, "state_map.txt"); // 保存状态映射到文件
    }

    StateMapDestroy(state_map); // 销毁状态映射

    return 0;
}
