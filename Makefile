CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O3 -I./include

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Fontes e objetos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
CORE_SRCS = $(filter-out $(SRC_DIR)/main.cpp,$(SRCS))

# Binário principal
TARGET = $(BIN_DIR)/trabalho_arvores

# Testes
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_SRCS))
TEST_TARGET = $(BIN_DIR)/test_arvores
ASAN_TARGET = $(BIN_DIR)/test_asan
DIAGRAM_TARGET = $(BIN_DIR)/generate_diagrams
TECTONIC ?= tectonic
ASAN_OPTIONS ?= detect_leaks=1:halt_on_error=1

all: directories $(TARGET)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) data docs/graficos

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: directories $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

asan: directories
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -g -O1 \
		-fsanitize=address,undefined -fno-omit-frame-pointer -I./include \
		$(filter-out $(SRC_DIR)/main.cpp,$(SRCS)) $(TEST_SRCS) -o $(ASAN_TARGET)
	ASAN_OPTIONS=$(ASAN_OPTIONS) ./$(ASAN_TARGET)

$(DIAGRAM_TARGET): $(CORE_SRCS) tools/generate_diagrams.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

diagrams: directories $(DIAGRAM_TARGET)
	@mkdir -p docs/diagramas/dot
	./$(DIAGRAM_TARGET)
	@if command -v dot >/dev/null; then \
		for f in docs/diagramas/dot/*.dot; do \
			base=$$(basename "$$f" .dot); \
			dot -Tpdf "$$f" -o "docs/diagramas/$$base.pdf"; \
			dot -Gdpi=180 -Tpng "$$f" -o "docs/diagramas/$$base.png"; \
		done; \
	else \
		python3 tools/render_diagrams.py; \
	fi

report: diagrams
	@if command -v $(TECTONIC) >/dev/null && $(TECTONIC) -o docs docs/relatorio.tex; then \
		:; \
	elif command -v latexmk >/dev/null; then \
		cd docs && latexmk -pdf -interaction=nonstopmode -halt-on-error relatorio.tex; \
	else \
		echo "Instale Tectonic ou latexmk para compilar o relatorio."; \
		exit 1; \
	fi

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f ./*.o

.PHONY: all directories clean run test asan diagrams report
