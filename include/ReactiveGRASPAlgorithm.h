#pragma once

#include "Graph.h"
#include <vector>

class ReactiveGRASPAlgorithm
{
public:
    ReactiveGRASPAlgorithm(const Graph &g,
                           int p,
                           int q,
                           const std::vector<double> &alphas,
                           int blockSize,
                           int totalIterations);

    std::vector<int> solve();
    double getBestAlphaUsed() const { return bestAlphaUsed; }
    double getAverageSolution() const { return averageSolution; }
    int getBestMaxColor() const { return bestMaxColor; }

private:
    const Graph &graph;
    int p_param;
    int q_param;
    int blockSize;
    int totalIterations;

    std::vector<double> alphaValues;
    std::vector<double> alphaProbabilities;
    std::vector<double> alphaRewardSum;
    std::vector<int> alphaUsageCount;

    double bestAlphaUsed;
    int bestMaxColor;
    double averageSolution;

    std::vector<int> constructGreedyRandomized(double alpha);
    int findSmallestValidColor(int v, const std::vector<int> &coloring);
    void updateAlphaProbabilities();
    int selectAlphaIndex();
};
