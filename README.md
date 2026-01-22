# trabalho-grafos

Trabalho para a disciplina de Grafos no tema Coloração L

## Comandos Make

```bash
# Compilar o projeto (gera bin/lpq_coloring)
make

# Executar com exemplo padrão
make run

# Executar testes
make test

# Executar experimentos completos
make experiments

# Ver ajuda do programa
make help

# Limpar artefatos compilados
make clean

# Limpar tudo (artefatos + saídas em output/ e results/)
make distclean
```

## Exemplos de execução manual

```bash
# Executar com grafo DIMACS
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -p 2 -q 1 -o output/solucao.txt -v output/visualizacao.txt

# Exemplo com GRASP alpha=0.3 e 100 iterações
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -p 2 -q 1 -a grasp --alpha 0.3 --iter 100

# Ver ajuda
./bin/lpq_coloring --help
```