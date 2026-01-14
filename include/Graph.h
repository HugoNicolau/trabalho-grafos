#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

class Graph
{
private:
    int numVertices;
    bool directed;
    std::vector<std::vector<int>> adjList;
    std::vector<int> vertexWeights;
    std::unordered_map<std::string, int> edgeWeights;

    std::string getEdgeKey(int u, int v) const;

public:
    /**
     * @brief Construtor
     * @param n Número de vértices
     * @param isDirected Se o grafo é direcionado
     */
    Graph(int n = 0, bool isDirected = false);

    /**
     * @brief Adiciona uma aresta ao grafo
     * @param u Vértice origem
     * @param v Vértice destino
     * @param weight Peso da aresta (opcional)
     */
    void addEdge(int u, int v, int weight = 1);

    /**
     * @brief Define o peso de um vértice
     * @param v Vértice
     * @param weight Peso
     */
    void setVertexWeight(int v, int weight);

    /**
     * @brief Obtém o número de vértices
     */
    int getNumVertices() const { return numVertices; }

    /**
     * @brief Obtém os vizinhos de um vértice
     * @param v Vértice
     * @return Lista de vizinhos
     */
    const std::vector<int> &getNeighbors(int v) const;

    /**
     * @brief Calcula a distância entre dois vértices usando BFS
     * @param u Vértice origem
     * @param v Vértice destino
     * @return Distância (ou -1 se não conectados)
     */
    int distance(int u, int v) const;

    /**
     * @brief Obtém vértices a distância 2 de um dado vértice
     * @param v Vértice
     * @return Conjunto de vértices a distância 2
     */
    std::unordered_set<int> getVerticesAtDistance2(int v) const;

    /**
     * @brief Verifica se o grafo é direcionado
     */
    bool isDirected() const { return directed; }

    /**
     * @brief Obtém o grau de um vértice
     * @param v Vértice
     */
    int getDegree(int v) const;

    /**
     * @brief Limpa o grafo
     */
    void clear();

    /**
     * @brief Valida se um vértice está no intervalo válido
     * @param v Vértice a validar
     * @return true se válido, false caso contrário
     */
    bool isValidVertex(int v) const { return v >= 0 && v < numVertices; }
};

#endif
