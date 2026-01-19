echo "========================================="
echo "  Experimentos - L(p,q)-Coloring"
echo "========================================="

INSTANCES=(
  "instances/dimacs/dsjc125.1.col"
  "instances/dimacs/r125.5.col"
)

ALGO="greedy"
P=2
Q=1
RUNS=10

for inst in "${INSTANCES[@]}"; do
  echo "Instância: $inst"
  
  for ((i=1; i<=RUNS; i++)); do
    SEED=$RANDOM
    echo "  Execução $i | seed=$SEED"
    
    ./bin/lpq_coloring \
      -i "$inst" \
      -a $ALGO \
      -p $P \
      -q $Q \
      -s $SEED \
      > /dev/null
  done

  echo
done

echo "Experimentos concluídos."
