#include "../include/Graph.h"
#include <queue>
#include <algorithm>
#include <stdexcept>

Graph::Graph(int n, bool isDirected)
    : numVertices(n), directed(isDirected)
{
    adjList.resize(n);
    vertexWeights.resize(n, 0);
}

std::string Graph::getEdgeKey(int u, int v) const
{
    if (!directed && u > v)
    {
        std::swap(u, v);
    }
    return std::to_string(u) + "_" + std::to_string(v);
}

void Graph::addEdge(int u, int v, int weight)
{
    if (!isValidVertex(u) || !isValidVertex(v))
    {
        throw std::out_of_range("Vértice fora do intervalo válido");
    }

    adjList[u].push_back(v);
    if (!directed)
    {
        adjList[v].push_back(u);
    }

    edgeWeights[getEdgeKey(u, v)] = weight;
}

void Graph::setVertexWeight(int v, int weight)
{
    if (!isValidVertex(v))
    {
        throw std::out_of_range("Vértice fora do intervalo válido");
    }
    vertexWeights[v] = weight;
}

const std::vector<int> &Graph::getNeighbors(int v) const
{
    if (!isValidVertex(v))
    {
        throw std::out_of_range("Vértice fora do intervalo válido");
    }
    return adjList[v];
}

int Graph::distance(int u, int v) const
{
    if (!isValidVertex(u) || !isValidVertex(v))
    {
        return -1;
    }

    if (u == v)
        return 0;

    std::vector<int> dist(numVertices, -1);
    std::queue<int> q;

    dist[u] = 0;
    q.push(u);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        for (int neighbor : adjList[current])
        {
            if (dist[neighbor] == -1)
            {
                dist[neighbor] = dist[current] + 1;
                if (neighbor == v)
                {
                    return dist[v];
                }
                q.push(neighbor);
            }
        }
    }

    return dist[v];
}

std::unordered_set<int> Graph::getVerticesAtDistance2(int v) const
{
    std::unordered_set<int> result;

    if (!isValidVertex(v))
    {
        return result;
    }

    std::unordered_set<int> distance1;
    distance1.insert(v);

    // Coletar vizinhos diretos (distância 1)
    for (int neighbor1 : adjList[v])
    {
        distance1.insert(neighbor1);
    }

    // Coletar vizinhos dos vizinhos (distância 2), excluindo distância 0 e 1
    for (int neighbor1 : adjList[v])
    {
        for (int neighbor2 : adjList[neighbor1])
        {
            if (distance1.find(neighbor2) == distance1.end())
            {
                result.insert(neighbor2);
            }
        }
    }

    return result;
}

int Graph::getDegree(int v) const
{
    if (v < 0 || v >= numVertices)
    {
        return 0;
    }
    return adjList[v].size();
}

void Graph::clear()
{
    adjList.clear();
    vertexWeights.clear();
    edgeWeights.clear();
    numVertices = 0;
}
