# trabalho-grafos

Trabalho para a disciplina de Grafos no tema Coloração L

## Como compilar e executar

```bash
# Compilar (gera bin/lpq_coloring)
make

# Limpar artefatos compilados
make clean

# Limpar tudo (artefatos + saídas em output/ e results/)
make distclean

# Executar exemplo com grafo DIMACS
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -p 2 -q 1 -o output/solucao.txt -v output/visualizacao.txt

#Exemplo com GRASP alpha=0.3 e 100 iterações
./bin/lpq_coloring -i instances/dimacs/dsjc125.1.col -p 2 -q 1 -a grasp --alpha 0.3 --iter 100

# Ver ajuda
./bin/lpq_coloring --help
```
## Como executar os experimentos
# Compilar o projeto
make

# Tornar o script executável (caso necessário)
chmod +x run_experiments.sh

# Executar os experimentos
./run_experiments.sh