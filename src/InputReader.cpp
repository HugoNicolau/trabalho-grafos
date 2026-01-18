#include "../include/InputReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>

//todo: verificação de edges para normalização se 0-based ou 1-based
bool normalizeEdges(
    std::vector<std::pair<int, int>> &edges,
    int numVertices
) {
    int minV = INT_MAX;
    int maxV = INT_MIN;

    for (auto &[u, v] : edges) {
        minV = std::min(minV, std::min(u, v));
        maxV = std::max(maxV, std::max(u, v));
    }

    // Caso 1: 0-based
    if (minV == 0 && maxV == numVertices - 1) {
        return true;
    }

    // Caso 2: 1-based
    if (minV == 1 && maxV == numVertices) {
        for (auto &[u, v] : edges) {
            u--;
            v--;
        }
        return true;
    }

    // Caso inválido
    std::cerr << "Erro: índices de vértices fora do padrão esperado\n";
    return false;
}

bool InputReader::readGraph(const std::string &filename, Graph &graph)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string line;
    int numVertices = 0, numEdges = 0;

    // Pular linhas de comentário e ler cabeçalho
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        if (iss >> numVertices >> numEdges)
            break;
    }

    if (numVertices <= 0 || numEdges < 0)
    {
        std::cerr << "Erro: valores inválidos no arquivo" << std::endl;
        return false;
    }

    graph = Graph(numVertices, false); // Assumindo grafo não direcionado

    int u, v;
    int edgesRead = 0;

    while (edgesRead < numEdges && std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        if (!(iss >> u >> v))
        {
            std::cerr << "Erro: formato inválido na linha" << std::endl;
            return false;
        }

        // Converter para índice baseado em 0 se os vértices começarem em 1
        if (u >= numVertices || v >= numVertices)
        {
            u--;
            v--;
        }

        try
        {
            graph.addEdge(u, v);
            edgesRead++;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erro ao adicionar aresta (" << u << ", " << v << "): "
                      << e.what() << std::endl;
            return false;
        }
    }

    file.close();
    std::cout << "Grafo carregado: " << numVertices << " vértices, "
              << edgesRead << " arestas" << std::endl;
    return true;
}

bool InputReader::readGraphDIMACS(const std::string &filename, Graph &graph)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string line;
    int numVertices = 0, numEdges = 0;
    bool headerFound = false;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'c')
        {
            continue;
        }
        else if (type == 'p')
        {
            std::string format;
            iss >> format >> numVertices >> numEdges;

            if (numVertices <= 0 || numEdges < 0)
            {
                std::cerr << "Erro: valores inválidos no cabeçalho" << std::endl;
                return false;
            }

            graph = Graph(numVertices, false);
            headerFound = true;
        }
        else if (type == 'e')
        {
            if (!headerFound)
            {
                std::cerr << "Erro: aresta encontrada antes do cabeçalho" << std::endl;
                return false;
            }

            int u, v;
            iss >> u >> v;

            // DIMACS usa índices baseados em 1
            u--;
            v--;

            try
            {
                graph.addEdge(u, v);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erro ao adicionar aresta (" << u << ", " << v << "): "
                          << e.what() << std::endl;
                return false;
            }
        }
    }

    file.close();

    if (!headerFound)
    {
        std::cerr << "Erro: cabeçalho do formato DIMACS não encontrado" << std::endl;
        return false;
    }

    std::cout << "Grafo DIMACS carregado: " << numVertices << " vértices, "
              << numEdges << " arestas" << std::endl;
    return true;
}

bool InputReader::readGraphAdjList(const std::string &filename, Graph &graph)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível abrir o arquivo " << filename << std::endl;
        return false;
    }

    int numVertices;
    file >> numVertices;

    if (numVertices <= 0)
    {
        std::cerr << "Erro: número de vértices inválido" << std::endl;
        return false;
    }

    graph = Graph(numVertices, false);
    file.ignore();

    std::string line;
    int edgeCount = 0;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        int vertex;
        char colon;

        iss >> vertex >> colon;

        if (colon != ':')
        {
            std::cerr << "Erro: formato inválido (esperado ':')" << std::endl;
            return false;
        }

        int neighbor;
        while (iss >> neighbor)
        {
            try
            {
                graph.addEdge(vertex, neighbor);
                edgeCount++;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erro ao adicionar aresta (" << vertex << ", "
                          << neighbor << "): " << e.what() << std::endl;
                return false;
            }
        }
    }

    file.close();
    std::cout << "Grafo (lista de adjacência) carregado: " << numVertices
              << " vértices, " << edgeCount << " arestas" << std::endl;
    return true;
}

bool InputReader::readGraphAuto(const std::string &filename, Graph &graph)
{
    // Detectar formato baseado na extensão
    if (filename.find(".dimacs") != std::string::npos ||
        filename.find(".col") != std::string::npos)
    {
        return readGraphDIMACS(filename, graph);
    }

    // Tenta detectar o formato baseado no conteúdo
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string firstLine;
    while (std::getline(file, firstLine))
    {
        if (firstLine.empty() || firstLine[0] == '#')
            continue;
        break;
    }
    file.close();

    if (firstLine.empty())
    {
        std::cerr << "Erro: arquivo vazio ou só com comentários" << std::endl;
        return false;
    }

    // Detectar formato DIMACS
    if (firstLine[0] == 'c' || firstLine[0] == 'p')
    {
        return readGraphDIMACS(filename, graph);
    }

    // Detectar formato de lista de adjacência
    if (firstLine.find(':') != std::string::npos)
    {
        return readGraphAdjList(filename, graph);
    }

    // Formato padrão (número de vértices e arestas)
    return readGraph(filename, graph);
}
