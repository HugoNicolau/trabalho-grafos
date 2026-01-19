#pragma once

#include "Graph.h"
#include <vector>

class GRASPAlgorithm {
public:
    GRASPAlgorithm(const Graph &g, int p, int q, double alpha, int iterations);
    std::vector<int> solve();

private:
    const Graph &graph;
    int p_param;
    int q_param;
    double alpha;
    int iterations;

    std::vector<int> constructGreedyRandomized();
    int findSmallestValidColor(int v, const std::vector<int> &coloring);
};
