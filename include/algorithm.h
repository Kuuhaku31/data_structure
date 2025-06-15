
// algorithm.h

#pragma once

#include "mgraph.h"


// 计算图G中从顶点v0到其他顶点的最短路径
void ShortestPath_DIJ(MGraph G, int v0, int dist[], int path[]);
