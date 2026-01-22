#ifndef RESULT_LOGGER_H
#define RESULT_LOGGER_H

#include <string>
#include <chrono>

class ResultLogger
{
private:
    std::string filename;
    bool headerWritten;

    /**
     * @brief Verifica se o arquivo existe
     */
    bool fileExists(const std::string &filename);

    /**
     * @brief Escreve o cabeçalho do CSV
     */
    void writeHeader();

public:
    /**
     * @brief Construtor
     * @param csvFilename Nome do arquivo CSV
     */
    ResultLogger(const std::string &csvFilename = "results/resultados.csv");

    /**
     * @brief Registra um resultado de execução
     *
     * @param instance Nome da instância
     * @param algorithm Nome do algoritmo executado
     * @param p Parâmetro p do L(p,q)-coloring
     * @param q Parâmetro q do L(p,q)-coloring
     * @param alpha Parâmetro alpha (para GRASP)
     * @param iterations Número de iterações
     * @param blockSize Tamanho do bloco (para GRASP reativo)
     * @param seed Semente de randomização
     * @param executionTime Tempo de execução em segundos
     * @param bestSolution Valor da melhor solução (maior cor)
     * @param bestAlpha Alpha que forneceu melhor solução (para reativo)
     * @param averageSolution Média das soluções nas iterações
     * @return true se o registro foi bem-sucedido, false caso contrário
     */
    bool logResult(const std::string &instance,
                   const std::string &algorithm,
                   int p, int q,
                   double alpha,
                   int iterations,
                   int blockSize,
                   unsigned int seed,
                   double executionTime,
                   int bestSolution,
                   double bestAlpha = -1.0,
                   double averageSolution = -1.0);

    /**
     * @brief Obtém timestamp atual formatado
     */
    static std::string getCurrentTimestamp();
};

#endif
