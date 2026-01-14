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
         << "semente,tempo_execucao_s,melhor_solucao" << std::endl;

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
                             int bestSolution)
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
         << algorithm << ","
         << alpha << ","
         << iterations << ","
         << blockSize << ","
         << seed << ","
         << executionTime << ","
         << bestSolution << std::endl;

    file.close();

    std::cout << "✓ Resultado registrado em " << filename << std::endl;
    return true;
}
