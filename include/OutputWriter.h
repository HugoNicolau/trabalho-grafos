#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include "Graph.h"
#include <string>
#include <vector>
#include <map>

class OutputWriter
{
public:
    /**
     * @brief Escreve a solução na tela
     * @param coloring Mapeamento vértice -> cor
     * @param maxColor Maior cor utilizada
     */
    static void printSolution(const std::vector<int> &coloring, int maxColor);

    /**
     * @brief Escreve a solução em arquivo
     * @param filename Nome do arquivo de saída
     * @param coloring Mapeamento vértice -> cor
     * @param maxColor Maior cor utilizada
     * @return true se a escrita foi bem-sucedida, false caso contrário
     */
    static bool writeSolution(const std::string &filename,
                              const std::vector<int> &coloring,
                              int maxColor);

    /**
     * @brief Gera representação do grafo colorido para visualização
     * Formato compatível com http://csacademy.com/app/grapheditor/
     *
     * @param graph Grafo original
     * @param coloring Mapeamento vértice -> cor
     * @return String com representação do grafo
     */
    static std::string generateGraphVisualization(const Graph &graph,
                                                  const std::vector<int> &coloring);

    /**
     * @brief Escreve visualização do grafo em arquivo
     * @param filename Nome do arquivo de saída
     * @param graph Grafo original
     * @param coloring Mapeamento vértice -> cor
     * @return true se a escrita foi bem-sucedida, false caso contrário
     */
    static bool writeGraphVisualization(const std::string &filename,
                                        const Graph &graph,
                                        const std::vector<int> &coloring);

    /**
     * @brief Valida se a coloração é válida para L(p,q)-coloring
     * @param graph Grafo
     * @param coloring Coloração
     * @param p Parâmetro p
     * @param q Parâmetro q
     * @return true se válida, false caso contrário
     */
    static bool validateColoring(const Graph &graph,
                                 const std::vector<int> &coloring,
                                 int p, int q);
};

#endif
