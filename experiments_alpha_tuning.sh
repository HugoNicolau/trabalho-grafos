#!/bin/bash

echo "========================================="
echo "  Alpha tuning - GRASP L(p,q)-Coloring"
echo "========================================="

INSTANCE="instances/dimacs/dsjc125.1.col"

P=2
Q=1
RUNS=5
GRASP_ITER=30

ALPHAS=(0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7)

for alpha in "${ALPHAS[@]}"; do
  echo
  echo "Alpha = $alpha"

  for ((i=1; i<=RUNS; i++)); do
    SEED=$RANDOM
    echo "  Execução $i (seed=$SEED)"

    ./bin/lpq_coloring \
      -i "$INSTANCE" \
      -a grasp \
      -p $P \
      -q $Q \
      -s $SEED \
      --alpha $alpha \
      --iter $GRASP_ITER
  done
done

echo
echo "Alpha tuning finalizado."
