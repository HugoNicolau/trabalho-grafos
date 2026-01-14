# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS = 

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos
TARGET = $(BIN_DIR)/lpq_coloring
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Regra padrão
all: directories $(TARGET)

# Criar diretórios necessários
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) output results instances

# Linkar o executável
$(TARGET): $(OBJECTS)
	@echo "Linkando $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build concluído com sucesso!"

# Compilar arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	@echo "Limpando arquivos compilados..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Limpeza concluída!"

# Limpar também os resultados
distclean: clean
	@echo "Limpando resultados..."
	rm -rf output/* results/*
	@echo "Limpeza completa concluída!"

# Executar com exemplo padrão
run: all
	./$(TARGET) -i instances/dimacs/dsjc125.1.col -p 2 -q 1 -o output/solucao.txt -v output/visualizacao.txt

# Executar testes
test: all
	@echo "Executando testes..."
	./$(TARGET) -i instances/dimacs/dsjc125.1.col -p 2 -q 1
	@echo "Testes concluídos!"

# Mostrar ajuda
help:
	./$(TARGET) --help

# Dependências
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/Graph.h $(INC_DIR)/InputReader.h $(INC_DIR)/OutputWriter.h $(INC_DIR)/ResultLogger.h $(INC_DIR)/Config.h
$(OBJ_DIR)/Graph.o: $(SRC_DIR)/Graph.cpp $(INC_DIR)/Graph.h
$(OBJ_DIR)/InputReader.o: $(SRC_DIR)/InputReader.cpp $(INC_DIR)/InputReader.h $(INC_DIR)/Graph.h
$(OBJ_DIR)/OutputWriter.o: $(SRC_DIR)/OutputWriter.cpp $(INC_DIR)/OutputWriter.h $(INC_DIR)/Graph.h
$(OBJ_DIR)/ResultLogger.o: $(SRC_DIR)/ResultLogger.cpp $(INC_DIR)/ResultLogger.h

.PHONY: all directories clean distclean run test help
