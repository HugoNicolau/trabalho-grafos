#include "Graph.h"
#include "InputReader.h"
#include "OutputWriter.h"
#include "ResultLogger.h"
#include "Config.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>

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
        seed = static_cast<unsigned int>(std::time(nullptr));
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
    int blockSize = Config::DEFAULT_BLOCK_SIZE;

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
        }
        else if (arg == "--block" && i + 1 < argc)
        {
            blockSize = std::atoi(argv[++i]);
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

    std::cout << "\n[INFO] Executando coloração de exemplo..." << std::endl;
    std::cout << "[AVISO] Os algoritmos guloso, GRASP e GRASP reativo ainda não foram implementados." << std::endl;
    std::cout << "[AVISO] Usando coloração trivial apenas para demonstrar o sistema de I/O.\n"
              << std::endl;

    std::vector<int> coloring = exampleColoring(graph, p, q);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    int maxColor = 0;
    for (int color : coloring)
    {
        maxColor = std::max(maxColor, color);
    }

    std::cout << "Tempo de execução: " << elapsed.count() << " segundos" << std::endl;
    std::cout << "Maior cor utilizada: " << maxColor << std::endl;

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
    logger.logResult(
        inputFile,
        algorithm,
        p, q,
        alpha,
        iterations,
        blockSize,
        seed,
        elapsed.count(),
        maxColor);

    std::cout << "\n========================================" << std::endl;
    std::cout << "   Execução concluída com sucesso!" << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    return 0;
}
