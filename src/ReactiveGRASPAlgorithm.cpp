#include "../include/ReactiveGRASPAlgorithm.h"
#include "../include/Config.h"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <numeric>

ReactiveGRASPAlgorithm::ReactiveGRASPAlgorithm(const Graph &g,
                                               int p,
                                               int q,
                                               const std::vector<double> &alphas,
                                               int blockSize,
                                               int totalIterations)
    : graph(g),
      p_param(p),
      q_param(q),
      blockSize(std::max(blockSize, Config::DEFAULT_BLOCK_SIZE_GRASPR)),
      totalIterations(std::max(totalIterations, Config::DEFAULT_REACTIVE_ITERATIONS)),
      alphaValues(alphas.empty() ? Config::DEFAULT_REACTIVE_ALPHAS : alphas),
      alphaProbabilities(alphaValues.size(), 0.0),
      alphaRewardSum(alphaValues.size(), 0.0),
      alphaUsageCount(alphaValues.size(), 0),
      bestAlphaUsed(alphaValues.front()),
      bestMaxColor(INT_MAX),
      averageSolution(0.0)
{
    double initialProb = 1.0 / static_cast<double>(alphaValues.size());
    std::fill(alphaProbabilities.begin(), alphaProbabilities.end(), initialProb);
}

int ReactiveGRASPAlgorithm::findSmallestValidColor(int v, const std::vector<int> &coloring)
{
    int color = 1;
    while (true)
    {
        bool ok = true;

        const auto &nbrs = graph.getNeighbors(v);
        for (int u : nbrs)
        {
            if (coloring[u] == 0)
                continue;
            if (std::abs(color - coloring[u]) < p_param)
            {
                ok = false;
                break;
            }
        }

        if (!ok)
        {
            ++color;
            continue;
        }

        auto dist2 = graph.getVerticesAtDistance2(v);
        for (int u : dist2)
        {
            if (coloring[u] == 0)
                continue;
            if (std::abs(color - coloring[u]) < q_param)
            {
                ok = false;
                break;
            }
        }

        if (ok)
            break;
        ++color;
    }
    return color;
}

std::vector<int> ReactiveGRASPAlgorithm::constructGreedyRandomized(double alpha)
{
    int n = graph.getNumVertices();
    std::vector<int> coloring(n, 0);
    std::vector<bool> colored(n, false);

    for (int count = 0; count < n; ++count)
    {
        std::vector<std::pair<int, int>> candidates;

        for (int v = 0; v < n; ++v)
        {
            if (colored[v])
                continue;
            int minColor = findSmallestValidColor(v, coloring);
            candidates.emplace_back(minColor, v);
        }

        if (candidates.empty())
            break;

        int cMin = INT_MAX, cMax = INT_MIN;
        for (const auto &c : candidates)
        {
            cMin = std::min(cMin, c.first);
            cMax = std::max(cMax, c.first);
        }

        double threshold = cMin + alpha * (cMax - cMin);
        std::vector<std::pair<int, int>> rcl;
        for (const auto &c : candidates)
        {
            if (c.first <= threshold)
            {
                rcl.push_back(c);
            }
        }

        int idx = std::rand() % rcl.size();
        int chosenVertex = rcl[idx].second;
        int chosenColor = rcl[idx].first;

        coloring[chosenVertex] = chosenColor;
        colored[chosenVertex] = true;
    }

    return coloring;
}

void ReactiveGRASPAlgorithm::updateAlphaProbabilities()
{
    std::vector<double> rewards(alphaValues.size(), 0.0);
    for (size_t i = 0; i < alphaValues.size(); ++i)
    {
        if (alphaUsageCount[i] > 0)
        {
            rewards[i] = alphaRewardSum[i] / static_cast<double>(alphaUsageCount[i]);
        }
    }

    double sumRewards = std::accumulate(rewards.begin(), rewards.end(), 0.0);
    if (sumRewards <= 0.0)
    {
        double uniform = 1.0 / static_cast<double>(alphaValues.size());
        std::fill(alphaProbabilities.begin(), alphaProbabilities.end(), uniform);
        return;
    }

    for (size_t i = 0; i < alphaValues.size(); ++i)
    {
        alphaProbabilities[i] = rewards[i] / sumRewards;
    }
}

int ReactiveGRASPAlgorithm::selectAlphaIndex()
{
    double r = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    double cumulative = 0.0;
    for (size_t i = 0; i < alphaProbabilities.size(); ++i)
    {
        cumulative += alphaProbabilities[i];
        if (r <= cumulative)
            return static_cast<int>(i);
    }
    return static_cast<int>(alphaProbabilities.size() - 1);
}

std::vector<int> ReactiveGRASPAlgorithm::solve()
{
    std::vector<int> bestColoring;
    double sumSolutions = 0.0;

    for (int iter = 0; iter < totalIterations; ++iter)
    {
        if (iter > 0 && iter % blockSize == 0)
        {
            updateAlphaProbabilities();
        }

        int alphaIndex = selectAlphaIndex();
        double alpha = alphaValues[alphaIndex];

        std::vector<int> coloring = constructGreedyRandomized(alpha);

        int maxColor = 0;
        for (int c : coloring)
        {
            maxColor = std::max(maxColor, c);
        }

        sumSolutions += maxColor;

        alphaRewardSum[alphaIndex] += 1.0 / static_cast<double>(std::max(1, maxColor));
        alphaUsageCount[alphaIndex] += 1;

        if (maxColor < bestMaxColor)
        {
            bestMaxColor = maxColor;
            bestColoring = coloring;
            bestAlphaUsed = alpha;
        }
    }

    averageSolution = sumSolutions / static_cast<double>(totalIterations);

    return bestColoring;
}
