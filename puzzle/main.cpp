
// puzzle/main.cpp

#include "header.h"

#include <stdio.h>

int
main(int argc, char* argv[])
{
    // State start  = "213540786";
    // State target = "123456780"; // 目标状态


    State start;
    State target;
    StateSet(start, "213540786");  // 初始状态
    StateSet(target, "123456780"); // 目标状态
    printf("初始状态为:\n");
    StatePrint(start);
    printf("目标状态为:\n");
    StatePrint(target);

    // if(argc > 1)
    // {
    //     start = argv[1]; // 从命令行参数获取初始状态
    //     printf("初始状态为: %s\n", start.c_str());
    // }
    // else
    // {
    //     printf("请输入初始状态（0 表示空格，按行输入共 9 个数字）:\n");
    //     for(int i = 0; i < 9; ++i)
    //     {
    //         char ch;
    //         scanf(" %c", &ch);
    //         start += ch;
    //     }
    // }

    LinkList path;
    StateMap state_map;
    LinkListInit(path);      // 初始化路径队列
    StateMapInit(state_map); // 初始化状态映射

    int steps = BFS(start, target, state_map, path);

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
        int   count   = 0;
        Node* current = path;
        do
        {
            current = current->last_list_node; // 向前移动到上一个节点

            printf("移动%d次：\n", count);
            StatePrint(current->current_state);

            printf("上一个操作：\n");
            Operate dir = current->operate;
            switch(dir)
            {
            case Operate::UP:
                printf("向下划动\n");
                break;
            case Operate::RIGHT:
                printf("向左划动\n");
                break;
            case Operate::DOWN:
                printf("向上划动\n");
                break;
            case Operate::LEFT:
                printf("向右划动\n");
                break;
            case Operate::NONE:
                printf("无操作\n");
                break;
            }

            printf("\n");

            count++;
        } while(current != path); // 循环队列
    }

    // 销毁状态映射
    StateMapDestroy(state_map);

    return 0;
}
