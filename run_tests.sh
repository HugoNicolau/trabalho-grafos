echo "========================================="
echo "  Testes Automatizados - L(p,q)-Coloring"
echo "========================================="
echo

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TOTAL=0
PASSED=0
FAILED=0

run_test() {
    local name=$1
    local instance=$2
    local p=$3
    local q=$4
    local extra_args=$5
    
    TOTAL=$((TOTAL + 1))
    echo "Teste $TOTAL: $name"
    echo "  Instância: $instance, p=$p, q=$q"
    
    if ./bin/lpq_coloring -i "$instance" -p "$p" -q "$q" $extra_args > /dev/null 2>&1; then
        echo -e "  ${GREEN}✓ PASSOU${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "  ${RED}✗ FALHOU${NC}"
        FAILED=$((FAILED + 1))
    fi
    echo
}

echo "Compilando projeto..."
if make > /dev/null 2>&1; then
    echo -e "${GREEN}✓ Compilação bem-sucedida${NC}"
else
    echo -e "${RED}✗ Falha na compilação${NC}"
    exit 1
fi
echo

echo "Executando testes..."
echo

run_test "Grafo pequeno - dsjc125.1" "instances/dimacs/dsjc125.1.col" 2 1 ""
run_test "Grafo pequeno - r125.5" "instances/dimacs/r125.5.col" 2 1 ""
run_test "Grafo médio - flat300_28_0" "instances/dimacs/flat300_28_0.col" 2 1 ""
run_test "Grafo Leighton - le450_15c" "instances/dimacs/le450_15c.col" 2 1 ""
run_test "Parâmetros diferentes (p=3, q=2)" "instances/dimacs/dsjc125.1.col" 3 2 ""
run_test "Com seed customizada" "instances/dimacs/dsjc125.1.col" 2 1 "-s 123456"

echo "========================================="
echo "  Resumo dos Testes"
echo "========================================="
echo "Total de testes: $TOTAL"
echo -e "Passaram: ${GREEN}$PASSED${NC}"
echo -e "Falharam: ${RED}$FAILED${NC}"
echo

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ Todos os testes passaram!${NC}"
    exit 0
else
    echo -e "${YELLOW}⚠ Alguns testes falharam${NC}"
    exit 1
fi
