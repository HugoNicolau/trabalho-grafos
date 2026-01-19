echo "========================================="
echo "  Experimentos - L(p,q)-Coloring"
echo "========================================="

INSTANCES=(
  "instances/dimacs/dsjc125.1.col"
  "instances/dimacs/r125.5.col"
)

ALGOS=("greedy" "grasp")
P=2
Q=1
RUNS=10
GRASP_ALPHA=0.3
GRASP_ITER=30

for inst in "${INSTANCES[@]}"; do
  echo "Instância: $inst"

  for algo in "${ALGOS[@]}"; do
    echo "  Algoritmo: $algo"

    for ((i=1; i<=RUNS; i++)); do
      SEED=$RANDOM
      echo "    Execução $i (seed=$SEED)"

      if [ "$algo" == "grasp" ]; then
        ./bin/lpq_coloring \
          -i "$inst" \
          -a "$algo" \
          -p $P \
          -q $Q \
          -s $SEED \
          --alpha $GRASP_ALPHA \
          --iter $GRASP_ITER \
          > /dev/null
      else
        ./bin/lpq_coloring \
          -i "$inst" \
          -a "$algo" \
          -p $P \
          -q $Q \
          -s $SEED \
          > /dev/null
      fi
    done
  done
  echo
done


echo "Experimentos concluídos."
