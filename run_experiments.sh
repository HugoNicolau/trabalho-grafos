#!/bin/bash

echo "========================================="
echo "  Experimentos - L(p,q)-Coloring"
echo "========================================="

INSTANCES=(
  "instances/dimacs/dsjc125.1.col"
  "instances/dimacs/dsjc125.9.col"
)

ALGOS=("greedy" "grasp" "reactive")
P=2
Q=1
RUNS=10
GRASP_ALPHAS=(0.1 0.3 0.5)
GRASP_ITER=30
REACTIVE_ITER=300
REACTIVE_BLOCK=30

# A semente é gerada automaticamente pelo programa C++ (baseada em data/hora)
# Para reproduzir um teste específico, passe a semente com -s <valor>

for inst in "${INSTANCES[@]}"; do
  echo "Instância: $inst"

  for algo in "${ALGOS[@]}"; do
    
    if [ "$algo" == "grasp" ]; then
      for alpha in "${GRASP_ALPHAS[@]}"; do
        echo "  Algoritmo: $algo (alpha=$alpha)"
        
        for ((i=1; i<=RUNS; i++)); do
          echo "    Execução $i"
          ./bin/lpq_coloring \
            -i "$inst" \
            -a "$algo" \
            -p $P \
            -q $Q \
            --alpha $alpha \
            --iter $GRASP_ITER
        done
      done

    elif [ "$algo" == "reactive" ]; then
      echo "  Algoritmo: $algo"
      
      for ((i=1; i<=RUNS; i++)); do
        echo "    Execução $i"
        ./bin/lpq_coloring \
          -i "$inst" \
          -a "$algo" \
          -p $P \
          -q $Q \
          --iter $REACTIVE_ITER \
          --block $REACTIVE_BLOCK
      done

    else
      # Greedy
      echo "  Algoritmo: $algo"
      
      for ((i=1; i<=RUNS; i++)); do
        echo "    Execução $i"
        ./bin/lpq_coloring \
          -i "$inst" \
          -a "$algo" \
          -p $P \
          -q $Q
      done
    fi
  done
  echo
done


echo "Experimentos concluídos."
