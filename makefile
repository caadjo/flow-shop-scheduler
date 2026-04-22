# Compilador e flags
CXX       := g++
CXXFLAGS  := -Wall -Wextra -std=c++17 -O2
INC_FLAGS := -Iinclude

# Diretórios
SRC_DIR   := src
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
BIN_DIR   := $(BUILD_DIR)/bin

# Nome do executável final
TARGET    := $(BIN_DIR)/flowshop

# Encontra todos os arquivos .cpp dentro de src/
SRCS      := $(wildcard $(SRC_DIR)/*.cpp)
# Gera o nome dos arquivos .o correspondentes
OBJS      := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regra principal
all: directories $(TARGET)

# Cria as pastas de build se não existirem
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Linkagem do executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful! Executable is at $(TARGET)"

# Compilação dos arquivos objetos (.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned."

.PHONY: all directories clean
