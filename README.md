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

# Ver ajuda
./bin/lpq_coloring --help
```
