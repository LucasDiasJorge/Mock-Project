# Nome do executável e diretório com a data
DATE = $(shell date +%Y-%m-%d)
BUILD_DIR = build/$(DATE)
TARGET = $(BUILD_DIR)/executable
COMMIT_FILE = $(BUILD_DIR)/commit.txt

# Compilador e flags
CC = gcc
INTERNLIBS = src/config.c src/pi.c
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lcurl -lm

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

#CONST = -DDEBUG

# Fontes e objetos
SOURCES = $(wildcard $(SRC_DIR)/**/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Regra padrão
all: $(TARGET) $(COMMIT_FILE)

# Regra para criar o executável
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) src/main.c $(INTERNLIBS) $(CFLAGS) $(CONST) $^ -o $@ $(LDFLAGS)

# Regra para criar os objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CONST) -c $< -o $@


# Cria o commit.txt com o hash do commit atual e o diretorio do config.yml
$(COMMIT_FILE):
	@mkdir -p $(BUILD_DIR)
	echo "Commit: $$(git rev-parse HEAD)" > $@
	@mkdir build/config && touch .> build/config/config.yml

# Limpeza
clean:
	rm -rf $(OBJ_DIR) build/$(DATE)

# Regra de depuração (compila com debug symbols)
debug: CFLAGS += -g
debug: clean all

.PHONY: all clean debug
