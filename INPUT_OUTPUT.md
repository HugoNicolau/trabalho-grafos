# Documentação Técnica - Sistema de Input/Output para L(p,q)-Coloring

## 📋 Visão Geral

Este documento descreve a arquitetura e organização do sistema de Input/Output implementado para o problema de L(p,q)-coloring.

---

## 🏗️ Estrutura do Projeto

```
trabalho-grafos/
├── include/              # Headers (.h)
│   ├── Config.h         # Constantes e configurações do projeto
│   ├── Graph.h          # TAD Grafo
│   ├── InputReader.h    # Leitura de instâncias
│   ├── OutputWriter.h   # Escrita de soluções e validação
│   └── ResultLogger.h   # Logging de resultados em CSV
│
├── src/                 # Implementações (.cpp)
│   ├── Graph.cpp
│   ├── InputReader.cpp
│   ├── OutputWriter.cpp
│   ├── ResultLogger.cpp
│   └── main.cpp         # Ponto de entrada do programa
│
├── instances/
│   └── dimacs/          # 41 grafos benchmark DIMACS
│
├── output/              # Arquivos de saída (soluções, visualizações)
├── results/             # Logs CSV de execuções
│
├── Makefile             # Sistema de build
├── download_instances.sh # Script para baixar instâncias DIMACS
├── run_tests.sh         # Script de testes automatizados
└── INSTANCES.md         # Documentação das instâncias disponíveis
```

---

## 📁 Descrição Detalhada dos Arquivos

### 1. **include/Config.h**

**Propósito:** Centralizar todas as constantes e configurações do projeto.

**Conteúdo:**

- `DEFAULT_P`, `DEFAULT_Q` - Valores padrão para L(p,q)-coloring
- `DEFAULT_ALPHA`, `DEFAULT_ITERATIONS`, `DEFAULT_BLOCK_SIZE` - Parâmetros para GRASP
- Diretórios padrão (`results/`, `output/`, etc.)
- Nomes dos algoritmos disponíveis

**Como usar:**

```cpp
#include "Config.h"
int p = Config::DEFAULT_P;
std::string csvFile = Config::DEFAULT_CSV_FILE;
```

**Modificações futuras:**

- Adicionar novas constantes conforme necessário
- **Não requer recompilação de outros módulos** ao alterar valores

---

### 2. **include/Graph.h + src/Graph.cpp**

**Propósito:** Tipo Abstrato de Dados para representação de grafos.

**Estrutura de Dados:**

- `std::vector<std::vector<int>> adjList` - Lista de adjacência
- `std::vector<int> vertexWeights` - Pesos dos vértices (se necessário)
- `std::unordered_map<std::string, int> edgeWeights` - Pesos das arestas

**Métodos Principais:**

| Método                              | Descrição                   | Uso                       |
| ----------------------------------- | --------------------------- | ------------------------- |
| `Graph(int n, bool isDirected)`     | Construtor                  | Cria grafo com n vértices |
| `addEdge(int u, int v, int weight)` | Adiciona aresta             | Constrói o grafo          |
| `getNumVertices()`                  | Retorna número de vértices  | Para iterações            |
| `getNeighbors(int v)`               | Retorna vizinhos de v       | **Crítico para L(p,q)**   |
| `getVerticesAtDistance2(int v)`     | Vértices a distância 2 de v | **Crítico para L(p,q)**   |
| `getDegree(int v)`                  | Grau do vértice             | Útil para heurísticas     |
| `isValidVertex(int v)`              | Valida índice de vértice    | Verificação de segurança  |

**Exemplo de uso em algoritmos:**

```cpp
// Obter vizinhos diretos (distância 1)
const auto& neighbors = graph.getNeighbors(vertex);
for (int neighbor : neighbors) {
    // Verificar restrição p: |f(u) - f(v)| >= p
}

// Obter vértices a distância 2
auto distance2 = graph.getVerticesAtDistance2(vertex);
for (int v : distance2) {
    // Verificar restrição q: |f(u) - f(v)| >= q
}
```

**Complexidades:**

- `getNeighbors()`: O(1) - retorna referência
- `getVerticesAtDistance2()`: O(Σdeg(vizinhos))
- `addEdge()`: O(1) amortizado

---

### 3. **include/InputReader.h + src/InputReader.cpp**

**Propósito:** Ler instâncias de grafos em múltiplos formatos.

**Métodos Públicos:**

| Método               | Formato Suportado       | Uso                 |
| -------------------- | ----------------------- | ------------------- |
| `readGraph()`        | Padrão (n m + arestas)  | Formato simples     |
| `readGraphDIMACS()`  | DIMACS (c/p/e)          | Benchmarks oficiais |
| `readGraphAdjList()` | Lista de adjacência     | Formato alternativo |
| `readGraphAuto()`    | **Detecção automática** | **Recomendado**     |

**Detecção Automática:**

1. Verifica extensão do arquivo (`.dimacs`, `.col`)
2. Lê primeira linha não-comentário
3. Detecta formato baseado em conteúdo

**Formato DIMACS:**

```
c Comentários começam com 'c'
c ...
p edge <vértices> <arestas>
e <u> <v>
e ...
```

**Exemplo de uso:**

```cpp
Graph graph;
if (!InputReader::readGraphAuto(filename, graph)) {
    std::cerr << "Erro ao carregar grafo!" << std::endl;
    return false;
}
// Grafo está pronto para uso
```

**Características:**

- Suporta comentários (`#` ou `c`)
- Converte índices (base 1 → base 0 automaticamente)
- Tratamento robusto de erros

---

### 4. **include/OutputWriter.h + src/OutputWriter.cpp**

**Propósito:** Escrever soluções e validar colorações.

**Métodos Principais:**

| Método                         | Descrição                 | Saída                             |
| ------------------------------ | ------------------------- | --------------------------------- |
| `printSolution()`              | Exibe solução na tela     | stdout                            |
| `writeSolution()`              | Salva solução em arquivo  | Arquivo texto                     |
| `generateGraphVisualization()` | Gera representação visual | String                            |
| `writeGraphVisualization()`    | Salva visualização        | Arquivo compatível com CS Academy |
| `validateColoring()`           | **Valida solução**        | **bool (válida ou não)**          |

**Validação de Coloração:**

```cpp
bool OutputWriter::validateColoring(
    const Graph& graph,
    const std::vector<int>& coloring,
    int p, int q
)
```

**O que valida:**

1. Tamanho da coloração = número de vértices
2. **Restrição distância 1:** `|f(u) - f(v)| >= p` para vizinhos
3. **Restrição distância 2:** `|f(u) - f(v)| >= q` para vértices a distância 2

**Exemplo de uso:**

```cpp
std::vector<int> coloring = myAlgorithm.solve();
int maxColor = *std::max_element(coloring.begin(), coloring.end());

// Validar
if (OutputWriter::validateColoring(graph, coloring, p, q)) {
    std::cout << "Solução válida!" << std::endl;
    OutputWriter::writeSolution("output/sol.txt", coloring, maxColor);
}
```

**Visualização:**

- Formato compatível com http://csacademy.com/app/grapheditor/
- Cada vértice rotulado com sua cor: `0[2]` (vértice 0, cor 2)

---

### 5. **include/ResultLogger.h + src/ResultLogger.cpp**

**Propósito:** Registrar resultados de execuções em CSV para análise posterior.

**Estrutura do CSV:**

```
data_hora,instancia,p,q,algoritmo,alpha,iteracoes,tamanho_bloco,semente,tempo_execucao_s,melhor_solucao
```

**Uso:**

```cpp
ResultLogger logger(Config::DEFAULT_CSV_FILE);
logger.logResult(
    inputFile,           // Nome da instância
    algorithm,           // "greedy", "grasp", "reactive"
    p, q,               // Parâmetros do problema
    alpha,              // Parâmetro GRASP
    iterations,         // Número de iterações
    blockSize,          // Tamanho do bloco (GRASP reativo)
    seed,               // Semente de randomização
    executionTime,      // Tempo em segundos
    maxColor            // Melhor solução (maior cor)
);
```

**Características:**

- Cria arquivo automaticamente se não existir
- Adiciona cabeçalho na primeira execução
- Timestamp automático
- Permite análise estatística posterior (Excel, Python, etc.)

---

### 6. **src/main.cpp**

**Propósito:** Ponto de entrada do programa. Orquestra todo o fluxo.

**Fluxo de Execução:**

```
1. Parse de argumentos da linha de comando
   ↓
2. Inicialização de semente de randomização
   ↓
3. Leitura do grafo (InputReader)
   ↓
4. Execução do algoritmo ← **AQUI VOCÊ VAI IMPLEMENTAR**
   ↓
5. Validação da solução (OutputWriter)
   ↓
6. Escrita de saídas (arquivos)
   ↓
7. Logging em CSV (ResultLogger)
```

**Seção Crítica para Implementação (linhas ~170-180):**

```cpp
// ATUAL (exemplo trivial):
std::vector<int> coloring = exampleColoring(graph, p, q);

// FUTURO (com seus algoritmos):
std::vector<int> coloring;
if (algorithm == Config::ALGORITHM_GREEDY) {
    GreedyAlgorithm greedy(graph, p, q);
    auto solution = greedy.solve();
    coloring = solution.coloring;

} else if (algorithm == Config::ALGORITHM_GRASP) {
    GRASPAlgorithm grasp(graph, p, q, alpha, iterations);
    auto solution = grasp.solve();
    coloring = solution.coloring;

} else if (algorithm == Config::ALGORITHM_REACTIVE) {
    ReactiveGRASPAlgorithm reactive(graph, p, q, iterations, blockSize);
    auto solution = reactive.solve();
    coloring = solution.coloring;
}
```

**Parâmetros de Linha de Comando:**

```bash
-i <arquivo>      # Instância (OBRIGATÓRIO)
-p <valor>        # Parâmetro p (padrão: 2)
-q <valor>        # Parâmetro q (padrão: 1)
-o <arquivo>      # Arquivo de saída
-v <arquivo>      # Arquivo de visualização
-s <semente>      # Semente de randomização
-a <algoritmo>    # greedy|grasp|reactive
--alpha <valor>   # Alpha para GRASP
--iter <n>        # Iterações
--block <n>       # Tamanho do bloco (reativo)
```

---

## 🔧 Como Compilar e Executar

### Compilação

```bash
# Compilar tudo
make

# Limpar e recompilar
make clean && make

# Limpar tudo (incluindo resultados)
make distclean
```

### Execução Básica

```bash
# Executar com grafo DIMACS
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -p 2 -q 1

# Com saída em arquivo
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -p 2 -q 1 -o output/solucao.txt

# Com visualização
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -p 2 -q 1 -v output/viz.txt

# Com semente específica (reprodutibilidade)
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -s 1234567890
```

### Testes Automatizados

```bash
# Executar suite de testes
./run_tests.sh

# Apenas compilar e testar
make test
```

---

## 🎯 Arquitetura para Implementação de Algoritmos

**1. Funções auxiliares disponíveis:**

```cpp
// Já implementadas em Graph.h
const std::vector<int>& getNeighbors(int v);  // Vizinhos diretos
std::unordered_set<int> getVerticesAtDistance2(int v);  // Distância 2
int getDegree(int v);  // Grau do vértice
bool isValidVertex(int v);  // Validação
```

**2. Validação automática:**

```cpp
// Após calcular coloração:
bool isValid = OutputWriter::validateColoring(graph, coloring, p, q);
```

## 🔍 Debugging e Validação

### Verificar se coloração é válida:

```cpp
if (!OutputWriter::validateColoring(graph, coloring, p, q)) {
    std::cerr << "ERRO: Coloração inválida!" << std::endl;
    // Violações são impressas automaticamente
}
```

### Executar com output detalhado:

```bash
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col 2>&1 | tee debug.log
```

### Verificar logs CSV:

```bash
tail -f results/resultados.csv
```

---

## ⚙️ Modificações Necessárias no Código Existente

**Main:**

- `main.cpp` - Seção de execução (linhas ~170-180)
  - Substituir `exampleColoring()` por chamadas aos algoritmos reais

**Makefile:**

- Adicionar novos arquivos `.o` às dependências

---

## 📈 Exemplo Completo de Integração

```cpp
// Pseudocódigo de como integrar novo algoritmo

// 1. Criar header (include/GreedyAlgorithm.h)
class GreedyAlgorithm {
    const Graph& graph;
    int p, q;
public:
    GreedyAlgorithm(const Graph& g, int p, int q);
    std::vector<int> solve();
};

// 2. Implementar (src/GreedyAlgorithm.cpp)
std::vector<int> GreedyAlgorithm::solve() {
    int n = graph.getNumVertices();
    std::vector<int> coloring(n, -1);

    for (int v = 0; v < n; v++) {
        int color = 0;
        bool valid = false;

        while (!valid) {
            valid = true;

            // Verificar vizinhos (distância 1)
            for (int u : graph.getNeighbors(v)) {
                if (coloring[u] != -1 && abs(color - coloring[u]) < p) {
                    valid = false;
                    break;
                }
            }

            // Verificar distância 2
            if (valid) {
                for (int u : graph.getVerticesAtDistance2(v)) {
                    if (coloring[u] != -1 && abs(color - coloring[u]) < q) {
                        valid = false;
                        break;
                    }
                }
            }

            if (!valid) color++;
        }

        coloring[v] = color;
    }

    return coloring;
}

// 3. Integrar no main.cpp
if (algorithm == "greedy") {
    GreedyAlgorithm greedy(graph, p, q);
    coloring = greedy.solve();
}
```
