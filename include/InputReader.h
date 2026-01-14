#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "Graph.h"
#include <string>

class InputReader
{
public:
    /**
     * @brief Lê um grafo de um arquivo
     *
     * Formato esperado:
     * Linha 1: número_de_vértices número_de_arestas
     * Linhas seguintes: vértice_u vértice_v (uma aresta por linha)
     *
     * @param filename Nome do arquivo
     * @param graph Referência ao grafo que será preenchido
     * @return true se a leitura foi bem-sucedida, false caso contrário
     */
    static bool readGraph(const std::string &filename, Graph &graph);

    /**
     * @brief Lê um grafo no formato DIMACS
     *
     * Formato DIMACS:
     * c linhas de comentário começam com 'c'
     * p edge n m (n = número de vértices, m = número de arestas)
     * e u v (aresta entre u e v)
     *
     * @param filename Nome do arquivo
     * @param graph Referência ao grafo que será preenchido
     * @return true se a leitura foi bem-sucedida, false caso contrário
     */
    static bool readGraphDIMACS(const std::string &filename, Graph &graph);

    /**
     * @brief Lê um grafo no formato de lista de adjacência
     *
     * Formato:
     * Linha 1: número_de_vértices
     * Linhas seguintes: vértice: vizinho1 vizinho2 ... vizinhoN
     *
     * @param filename Nome do arquivo
     * @param graph Referência ao grafo que será preenchido
     * @return true se a leitura foi bem-sucedida, false caso contrário
     */
    static bool readGraphAdjList(const std::string &filename, Graph &graph);

    /**
     * @brief Detecta automaticamente o formato e lê o grafo
     * @param filename Nome do arquivo
     * @param graph Referência ao grafo que será preenchido
     * @return true se a leitura foi bem-sucedida, false caso contrário
     */
    static bool readGraphAuto(const std::string &filename, Graph &graph);
};

#endif
