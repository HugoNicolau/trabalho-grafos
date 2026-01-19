#include "../include/GRASPAlgorithm.h"
#include <algorithm>
#include <cstdlib>
#include <climits>

GRASPAlgorithm::GRASPAlgorithm(const Graph &g, int p, int q, double alpha, int iterations)
    : graph(g), p_param(p), q_param(q), alpha(alpha), iterations(iterations < 30 ? 30 : iterations)
{
}

// Encontra a menor cor válida para o vértice v considerando a coloração atual
int GRASPAlgorithm::findSmallestValidColor(int v, const std::vector<int> &coloring)
{
    int color = 1;
    while (true)
    {
        bool ok = true;

        // Verificar vizinhos (distância 1)
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

        // Verificar distância 2
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

// solução usando heurística gulosa randomizada
std::vector<int> GRASPAlgorithm::constructGreedyRandomized()
{
    int n = graph.getNumVertices();
    std::vector<int> coloring(n, 0);
    std::vector<bool> colored(n, false);

    for (int count = 0; count < n; ++count)
    {
        // Para cada vértice não colorido, calcular a menor cor válida
        std::vector<std::pair<int, int>> candidates; // (cor_minima, vertice)

        for (int v = 0; v < n; ++v)
        {
            if (colored[v])
                continue;
            int minColor = findSmallestValidColor(v, coloring);
            candidates.emplace_back(minColor, v);
        }

        if (candidates.empty())
            break;

        // Encontrar cMin e cMax entre os candidatos
        int cMin = INT_MAX, cMax = INT_MIN;
        for (const auto &c : candidates)
        {
            cMin = std::min(cMin, c.first);
            cMax = std::max(cMax, c.first);
        }

        // Construir RCL (Restricted Candidate List)
        // Um candidato entra na RCL se cor <= cMin + alpha * (cMax - cMin)
        double threshold = cMin + alpha * (cMax - cMin);
        std::vector<std::pair<int, int>> rcl;
        for (const auto &c : candidates)
        {
            if (c.first <= threshold)
            {
                rcl.push_back(c);
            }
        }

        // Escolher aleatoriamente um candidato da RCL
        int idx = std::rand() % rcl.size();
        int chosenVertex = rcl[idx].second;
        int chosenColor = rcl[idx].first;

        coloring[chosenVertex] = chosenColor;
        colored[chosenVertex] = true;
    }

    return coloring;
}

// Executa o GRASP por várias iterações e retorna a melhor solução
std::vector<int> GRASPAlgorithm::solve()
{
    std::vector<int> bestColoring;
    int bestMaxColor = INT_MAX;

    for (int iter = 0; iter < iterations; ++iter)
    {
        std::vector<int> coloring = constructGreedyRandomized();

        // Calcular a maior cor usada
        int maxColor = 0;
        for (int c : coloring)
        {
            maxColor = std::max(maxColor, c);
        }

        // Atualizar melhor solução se encontrou uma melhor
        if (maxColor < bestMaxColor)
        {
            bestMaxColor = maxColor;
            bestColoring = coloring;
        }
    }

    return bestColoring;
}
