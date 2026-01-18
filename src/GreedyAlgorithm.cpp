#include "../include/GreedyAlgorithm.h"
#include "../include/Graph.h"
#include <algorithm>
#include <queue>
#include <unordered_set>

GreedyAlgorithm::GreedyAlgorithm(const Graph &g, int p, int q)
    : graph(g), p_param(p), q_param(q)
{
}

// Heurística gulosa simples:
// - Ordena vértices por grau decrescente
// - Atribui a cada vértice a menor cor positiva que satisfaça as
//   restrições L(p,q) em relação a vértices já coloridos
std::vector<int> GreedyAlgorithm::solve()
{
    int n = graph.getNumVertices();
    std::vector<int> coloring(n, 0); // 0 = não colorido

    // Ordenar vértices por grau decrescente (empata por id)
    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [this](int a, int b) {
        int da = graph.getDegree(a);
        int db = graph.getDegree(b);
        if (da != db) return da > db;
        return a < b;
    });

    for (int v : order)
    {
        int color = 1;
        while (true)
        {
            bool ok = true;

            // Verificar vizinhos (distância 1)
            const auto &nbrs = graph.getNeighbors(v);
            for (int u : nbrs)
            {
                if (coloring[u] == 0) continue;
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
                if (coloring[u] == 0) continue;
                if (std::abs(color - coloring[u]) < q_param)
                {
                    ok = false;
                    break;
                }
            }

            if (ok)
            {
                coloring[v] = color;
                break;
            }

            ++color;
        }
    }

    return coloring;
}
