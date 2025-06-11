# pizzle/all.py


import itertools
import json
from collections import deque

GOAL = "123456780"
MOVES = {
    0: [1, 3],
    1: [0, 2, 4],
    2: [1, 5],
    3: [0, 4, 6],
    4: [1, 3, 5, 7],
    5: [2, 4, 8],
    6: [3, 7],
    7: [4, 6, 8],
    8: [5, 7],
}


def is_solvable(puzzle):
    """判断一个八数码状态是否有解（逆序对为偶数）"""
    inv_count = 0
    nums = [c for c in puzzle if c != "0"]
    for i in range(len(nums)):
        for j in range(i + 1, len(nums)):
            if nums[i] > nums[j]:
                inv_count += 1
    return inv_count % 2 == 0


def bfs(start):
    """BFS 计算从 start 到目标的最短路径"""
    queue = deque()
    visited = {}
    queue.append((start, []))
    visited[start] = True

    while queue:
        state, path = queue.popleft()
        if state == GOAL:
            return path + [state]

        zero = state.index("0")
        for move in MOVES[zero]:
            state_list = list(state)
            state_list[zero], state_list[move] = state_list[move], state_list[zero]
            new_state = "".join(state_list)
            if new_state not in visited:
                visited[new_state] = True
                queue.append((new_state, path + [state]))

    return []  # 不可达


def main():
    all_states = itertools.permutations("012345678")
    solutions = {}

    for i, state_tuple in enumerate(all_states):
        state = "".join(state_tuple)
        if not is_solvable(state):
            continue  # 跳过无解状态

        path = bfs(state)
        solutions[state] = path

        if i % 1000 == 0:
            print(f"Processed {i} states...")

    # 保存为 JSON 文件
    with open("solutions.json", "w") as f:
        json.dump(solutions, f)

    print("保存完成，共保存状态数：", len(solutions))


if __name__ == "__main__":
    main()
