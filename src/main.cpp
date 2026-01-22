#include "Graph.h"
#include "InputReader.h"
#include "OutputWriter.h"
#include "ResultLogger.h"
#include "GreedyAlgorithm.h"
#include "GRASPAlgorithm.h"
#include "ReactiveGRASPAlgorithm.h"
#include "Config.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

/**
 * @brief Gera e imprime a semente de randomização
 * @param customSeed Semente customizada (0 para gerar baseada em tempo)
 * @return Semente utilizada
 */
unsigned int initializeRandomSeed(unsigned int customSeed = 0)
{
    unsigned int seed;

    if (customSeed != 0)
    {
        seed = customSeed;
        std::cout << "Usando semente customizada: " << seed << std::endl;
    }
    else
    {
        // Gera semente baseada em tempo com alta precisão (microssegundos) + PID
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        seed = static_cast<unsigned int>(micros ^ getpid());
        std::cout << "Semente gerada (timestamp): " << seed << std::endl;
    }

    std::srand(seed);
    return seed;
}

/**
 * Esta é apenas uma coloração trivial para testar o sistema de I/O
 */
std::vector<int> exampleColoring(const Graph &graph, int p, int q)
{
    int n = graph.getNumVertices();
    std::vector<int> coloring(n);

    int spacing = std::max(p, q);
    for (int i = 0; i < n; i++)
    {
        coloring[i] = i * spacing;
    }

    return coloring;
}

void printUsage(const char *programName)
{
    std::cout << "\nUso: " << programName << " [opções]" << std::endl;
    std::cout << "\nOpções:" << std::endl;
    std::cout << "  -i <arquivo>     Arquivo de entrada (instância do grafo)" << std::endl;
    std::cout << "  -o <arquivo>     Arquivo de saída para solução" << std::endl;
    std::cout << "  -v <arquivo>     Arquivo de saída para visualização" << std::endl;
    std::cout << "  -p <valor>       Parâmetro p do L(p,q)-coloring (padrão: 2)" << std::endl;
    std::cout << "  -q <valor>       Parâmetro q do L(p,q)-coloring (padrão: 1)" << std::endl;
    std::cout << "  -s <semente>     Semente de randomização (0 = automática)" << std::endl;
    std::cout << "  -a <algoritmo>   Algoritmo: greedy|grasp|reactive (padrão: greedy)" << std::endl;
    std::cout << "  --alpha <valor>  Valor de alpha para GRASP (0.0-1.0)" << std::endl;
    std::cout << "  --iter <n>       Número de iterações para GRASP" << std::endl;
    std::cout << "  --block <n>      Tamanho do bloco para GRASP reativo" << std::endl;
    std::cout << "  -h, --help       Mostra esta mensagem" << std::endl;
    std::cout << "\nExemplo:" << std::endl;
    std::cout << "  " << programName << " -i instances/grafo1.txt -p 2 -q 1 -o output/solucao.txt" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "========================================" << std::endl;
    std::cout << "   L(p,q)-Coloring Solver" << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    std::string inputFile = "";
    std::string outputFile = "";
    std::string vizFile = "";
    int p = Config::DEFAULT_P;
    int q = Config::DEFAULT_Q;
    unsigned int customSeed = 0;
    std::string algorithm = Config::ALGORITHM_GREEDY;
    double alpha = Config::DEFAULT_ALPHA;
    int iterations = Config::DEFAULT_ITERATIONS;
    int blockSize = Config::DEFAULT_BLOCK_SIZE_GRASPR;
    bool iterationsSet = false;
    bool blockSizeSet = false;

    // Parse de argumentos da linha de comando
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (arg == "-i" && i + 1 < argc)
        {
            inputFile = argv[++i];
        }
        else if (arg == "-o" && i + 1 < argc)
        {
            outputFile = argv[++i];
        }
        else if (arg == "-v" && i + 1 < argc)
        {
            vizFile = argv[++i];
        }
        else if (arg == "-p" && i + 1 < argc)
        {
            p = std::atoi(argv[++i]);
        }
        else if (arg == "-q" && i + 1 < argc)
        {
            q = std::atoi(argv[++i]);
        }
        else if (arg == "-s" && i + 1 < argc)
        {
            customSeed = std::atoi(argv[++i]);
        }
        else if (arg == "-a" && i + 1 < argc)
        {
            algorithm = argv[++i];
        }
        else if (arg == "--alpha" && i + 1 < argc)
        {
            alpha = std::atof(argv[++i]);
        }
        else if (arg == "--iter" && i + 1 < argc)
        {
            iterations = std::atoi(argv[++i]);
            iterationsSet = true;
        }
        else if (arg == "--block" && i + 1 < argc)
        {
            blockSize = std::atoi(argv[++i]);
            blockSizeSet = true;
        }
    }

    if (algorithm == Config::ALGORITHM_REACTIVE)
    {
        if (!iterationsSet)
        {
            iterations = Config::DEFAULT_REACTIVE_ITERATIONS;
        }
        if (!blockSizeSet)
        {
            blockSize = Config::DEFAULT_BLOCK_SIZE_GRASPR;
        }
    }

    if (inputFile.empty())
    {
        std::cerr << "Erro: arquivo de entrada não especificado!" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    unsigned int seed = initializeRandomSeed(customSeed);

    std::cout << "\nCarregando grafo de: " << inputFile << std::endl;
    Graph graph;

    if (!InputReader::readGraphAuto(inputFile, graph))
    {
        std::cerr << "Erro ao carregar o grafo!" << std::endl;
        return 1;
    }

    std::cout << "\nParâmetros do problema:" << std::endl;
    std::cout << "  p = " << p << " (diferença mínima para vértices adjacentes)" << std::endl;
    std::cout << "  q = " << q << " (diferença mínima para vértices a distância 2)" << std::endl;
    std::cout << "  Algoritmo: " << algorithm << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<int> coloring;
    double bestAlphaUsed = -1.0;
    double averageSolution = -1.0;

    if (algorithm == Config::ALGORITHM_GREEDY)
    {
        std::cout << "[INFO] Executando algoritmo guloso (greedy)..." << std::endl;
        GreedyAlgorithm greedy(graph, p, q);
        coloring = greedy.solve();
    }
    else if (algorithm == Config::ALGORITHM_GRASP)
    {
        std::cout << "[INFO] Executando GRASP (alpha=" << alpha << ", iter=" << iterations << ")..." << std::endl;
        GRASPAlgorithm grasp(graph, p, q, alpha, iterations);
        coloring = grasp.solve();
        averageSolution = grasp.getAverageSolution();
    }
    else if (algorithm == Config::ALGORITHM_REACTIVE)
    {
        std::cout << "[INFO] Executando GRASP reativo (alphas=";
        for (size_t i = 0; i < Config::DEFAULT_REACTIVE_ALPHAS.size(); ++i)
        {
            std::cout << Config::DEFAULT_REACTIVE_ALPHAS[i];
            if (i + 1 < Config::DEFAULT_REACTIVE_ALPHAS.size())
                std::cout << ",";
        }
        std::cout << ", bloco=" << blockSize << ", iter=" << iterations << ")..." << std::endl;

        ReactiveGRASPAlgorithm reactive(graph, p, q,
                                        Config::DEFAULT_REACTIVE_ALPHAS,
                                        blockSize,
                                        iterations);
        coloring = reactive.solve();
        bestAlphaUsed = reactive.getBestAlphaUsed();
        averageSolution = reactive.getAverageSolution();
    }
    else
    {
        std::cout << "[AVISO] Algoritmo solicitado não implementado; usando coloração de exemplo." << std::endl;
        coloring = exampleColoring(graph, p, q);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    int maxColor = 0;
    for (int color : coloring)
    {
        maxColor = std::max(maxColor, color);
    }

    std::cout << "Tempo de execução: " << elapsed.count() << " segundos" << std::endl;
    std::cout << "Maior cor utilizada: " << maxColor << std::endl;
    if (averageSolution >= 0)
    {
        std::cout << "Média das soluções: " << averageSolution << std::endl;
    }
    if (bestAlphaUsed >= 0)
    {
        std::cout << "Melhor alpha usado: " << bestAlphaUsed << std::endl;
    }

    std::cout << "\nValidando solução..." << std::endl;
    OutputWriter::validateColoring(graph, coloring, p, q);

    // Exibir solução na tela
    if (outputFile.empty() && vizFile.empty())
    {
        OutputWriter::printSolution(coloring, maxColor);
    }

    // Salvar solução em arquivo
    if (!outputFile.empty())
    {
        OutputWriter::writeSolution(outputFile, coloring, maxColor);
    }

    // Salvar visualização
    if (!vizFile.empty())
    {
        OutputWriter::writeGraphVisualization(vizFile, graph, coloring);
    }

    ResultLogger logger(Config::DEFAULT_CSV_FILE);
    double logAlpha = -1.0;
    int logIterations = 0;
    int logBlockSize = 0;
    double logBestAlpha = -1.0;
    double logAverageSolution = -1.0;
    if (algorithm == Config::ALGORITHM_GRASP)
    {
        logAlpha = alpha;
        logIterations = iterations;
        logAverageSolution = averageSolution;
    }
    else if (algorithm == Config::ALGORITHM_REACTIVE)
    {
        logIterations = iterations;
        logBlockSize = blockSize;
        logAverageSolution = averageSolution;
        logBestAlpha = bestAlphaUsed;
    }
    logger.logResult(
        inputFile,
        algorithm,
        p, q,
        logAlpha,
        logIterations,
        logBlockSize,
        seed,
        elapsed.count(),
        maxColor,
        logBestAlpha,
        logAverageSolution);

    std::cout << "\n========================================" << std::endl;
    std::cout << "   Execução concluída com sucesso!" << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    return 0;
}
