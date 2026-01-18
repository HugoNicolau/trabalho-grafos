#pragma once

#include "Graph.h"
#include <vector>

class GreedyAlgorithm {
public:
    GreedyAlgorithm(const Graph &g, int p, int q);
    std::vector<int> solve();

private:
    const Graph &graph;
    int p_param;
    int q_param;
};
