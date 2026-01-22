#include "../include/ResultLogger.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <sys/stat.h>

ResultLogger::ResultLogger(const std::string &csvFilename)
    : filename(csvFilename), headerWritten(false)
{

    // Verifica se o arquivo existe para decidir se escreve o cabeçalho
    headerWritten = fileExists(filename);

    // Se o arquivo não existe, cria o cabeçalho
    if (!headerWritten)
    {
        writeHeader();
    }
}

bool ResultLogger::fileExists(const std::string &filename)
{
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

void ResultLogger::writeHeader()
{
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível abrir o arquivo " << filename << std::endl;
        return;
    }

    file << "data_hora,instancia,p,q,algoritmo,alpha,iteracoes,tamanho_bloco,"
         << "semente,tempo_execucao_s,melhor_solucao,melhor_alpha,media_solucoes" << std::endl;

    file.close();
    headerWritten = true;
}

std::string ResultLogger::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool ResultLogger::logResult(const std::string &instance,
                             const std::string &algorithm,
                             int p, int q,
                             double alpha,
                             int iterations,
                             int blockSize,
                             unsigned int seed,
                             double executionTime,
                             int bestSolution,
                             double bestAlpha,
                             double averageSolution)
{
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Erro: não foi possível abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string timestamp = getCurrentTimestamp();

    file << std::fixed << std::setprecision(6);

    file << timestamp << ","
         << instance << ","
         << p << ","
         << q << ","
         << algorithm << ",";

    // Alpha: relevante apenas para GRASP (não reativo)
    if (algorithm == "grasp")
        file << alpha;
    file << ",";

    // Iterações: relevante para GRASP e Reativo
    if (algorithm == "grasp" || algorithm == "reactive")
        file << iterations;
    file << ",";

    // Tamanho do bloco: relevante apenas para Reativo
    if (algorithm == "reactive")
        file << blockSize;
    file << ",";

    file << seed << ","
         << executionTime << ","
         << bestSolution << ",";

    // Melhor alpha: relevante apenas para Reativo
    if (algorithm == "reactive" && bestAlpha >= 0)
        file << bestAlpha;
    file << ",";

    // Média das soluções: relevante para GRASP e Reativo
    if ((algorithm == "grasp" || algorithm == "reactive") && averageSolution >= 0)
        file << std::setprecision(2) << averageSolution;

    file << std::endl;

    file.close();

    std::cout << "✓ Resultado registrado em " << filename << std::endl;
    return true;
}
