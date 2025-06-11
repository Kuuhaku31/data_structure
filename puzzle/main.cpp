
// puzzle/main.cpp

#include "header.h"

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
    int steps = BFS(start, target, path);

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
            printState(current->current_state);

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

    return 0;
}
