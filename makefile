
CXX       := g++
CXXFLAGS  := -Wall -Wextra -std=c++17 -O2
INC_FLAGS := -Iinclude


SRC_DIR   := src
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
BIN_DIR   := $(BUILD_DIR)/bin

TARGET    := $(BIN_DIR)/flowshop


SRCS      := $(wildcard $(SRC_DIR)/*.cpp)

OBJS      := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: directories $(TARGET)


directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful! Executable is at $(TARGET)"


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned."

.PHONY: all directories clean
