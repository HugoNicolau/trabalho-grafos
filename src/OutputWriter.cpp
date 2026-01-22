#include "../include/OutputWriter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <set>

void OutputWriter::printSolution(const std::vector<int> &coloring, int maxColor)
{
    std::cout << "\n========== SOLUÇÃO L(p,q)-COLORING ==========" << std::endl;
    std::cout << "Maior cor utilizada: " << maxColor << std::endl;
    std::cout << "Número de vértices: " << coloring.size() << std::endl;
    std::cout << "\nColoração dos vértices:" << std::endl;

    for (size_t i = 0; i < coloring.size(); i++)
    {
        std::cout << "Vértice " << i << ": cor " << coloring[i] << std::endl;
    }

    std::cout << "============================================\n"
              << std::endl;
}

bool OutputWriter::writeSolution(const std::string &filename,
                                 const std::vector<int> &coloring,
                                 int maxColor)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível criar o arquivo " << filename << std::endl;
        return false;
    }

    file << "# Solução L(p,q)-Coloring" << std::endl;
    file << "# Maior cor utilizada: " << maxColor << std::endl;
    file << "# Número de vértices: " << coloring.size() << std::endl;
    file << std::endl;

    for (size_t i = 0; i < coloring.size(); i++)
    {
        file << i << " " << coloring[i] << std::endl;
    }

    file.close();
    std::cout << "Solução salva em: " << filename << std::endl;
    return true;
}

std::string OutputWriter::generateGraphVisualization(const Graph &graph,
                                                     const std::vector<int> &coloring)
{
    std::ostringstream oss;

    int n = graph.getNumVertices();

    // Adicionar arestas com nomes dos vértices incluindo a cor
    // Formato: v0(cor0) v1(cor1) - o nome do nó já mostra a cor
    std::set<std::pair<int, int>> addedEdges;

    for (int u = 0; u < n; u++)
    {
        const auto &neighbors = graph.getNeighbors(u);
        for (int v : neighbors)
        {
            if (!graph.isDirected())
            {
                int minV = std::min(u, v);
                int maxV = std::max(u, v);
                if (addedEdges.find({minV, maxV}) != addedEdges.end())
                {
                    continue;
                }
                addedEdges.insert({minV, maxV});
            }
            
            // Nome do nó inclui vértice e cor: v0(5)
            std::string uName = std::to_string(u) + "(" + std::to_string(coloring[u]) + ")";
            std::string vName = std::to_string(v) + "(" + std::to_string(coloring[v]) + ")";
            oss << uName << " " << vName << "\n";
        }
    }

    return oss.str();
}

bool OutputWriter::writeGraphVisualization(const std::string &filename,
                                           const Graph &graph,
                                           const std::vector<int> &coloring)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível criar o arquivo " << filename << std::endl;
        return false;
    }

    std::string visualization = generateGraphVisualization(graph, coloring);
    file << visualization;

    file.close();
    std::cout << "Visualização salva em: " << filename << std::endl;
    std::cout << "Copie e cole o conteúdo em http://csacademy.com/app/grapheditor/" << std::endl;
    return true;
}

bool OutputWriter::validateColoring(const Graph &graph,
                                    const std::vector<int> &coloring,
                                    int p, int q)
{
    int n = graph.getNumVertices();

    if ((int)coloring.size() != n)
    {
        std::cerr << "Erro: tamanho da coloração não corresponde ao número de vértices" << std::endl;
        return false;
    }

    bool valid = true;

    // Verificar restrição para vértices adjacentes (distância 1)
    for (int u = 0; u < n; u++)
    {
        const auto &neighbors = graph.getNeighbors(u);
        for (int v : neighbors)
        {
            int colorDiff = std::abs(coloring[u] - coloring[v]);
            if (colorDiff < p)
            {
                std::cerr << "Violação (distância 1): vértices " << u << " e " << v
                          << " têm diferença de cor " << colorDiff << " < " << p << std::endl;
                valid = false;
            }
        }
    }

    // Verificar restrição para vértices a distância 2
    for (int u = 0; u < n; u++)
    {
        auto distance2Vertices = graph.getVerticesAtDistance2(u);
        for (int v : distance2Vertices)
        {
            int colorDiff = std::abs(coloring[u] - coloring[v]);
            if (colorDiff < q)
            {
                std::cerr << "Violação (distância 2): vértices " << u << " e " << v
                          << " têm diferença de cor " << colorDiff << " < " << q << std::endl;
                valid = false;
            }
        }
    }

    if (valid)
    {
        std::cout << "✓ Coloração válida!" << std::endl;
    }
    else
    {
        std::cout << "✗ Coloração inválida!" << std::endl;
    }

    return valid;
}
