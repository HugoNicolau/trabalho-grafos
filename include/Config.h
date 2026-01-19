#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config
{
    // Valores padrão para L(p,q)-coloring
    constexpr int DEFAULT_P = 2;
    constexpr int DEFAULT_Q = 1;

    // Valores padrão para GRASP
    constexpr double DEFAULT_ALPHA = 0.3;
    constexpr int DEFAULT_ITERATIONS = 100;
    constexpr int DEFAULT_BLOCK_SIZE = 10;

    // Limites de validação
    constexpr double MIN_ALPHA = 0.0;
    constexpr double MAX_ALPHA = 1.0;
    constexpr int MIN_ITERATIONS = 1;
    constexpr int MIN_BLOCK_SIZE = 1;

    // Diretórios padrão
    const std::string DEFAULT_RESULTS_DIR = "results/";
    const std::string DEFAULT_OUTPUT_DIR = "output/";
    const std::string DEFAULT_INSTANCES_DIR = "instances/";
    const std::string DEFAULT_CSV_FILE = "results/resultados.csv";

    // Algoritmos disponíveis
    const std::string ALGORITHM_GREEDY = "greedy";
    const std::string ALGORITHM_GRASP = "grasp";
    const std::string ALGORITHM_REACTIVE = "reactive";

    // Parâmetros padrão para GRASP reativo
    constexpr int DEFAULT_BLOCK_SIZE_GRASPR = 30;
    constexpr int DEFAULT_REACTIVE_ITERATIONS = 300;
    const std::vector<double> DEFAULT_REACTIVE_ALPHAS = {0.1, 0.3, 0.5};

}

#endif
