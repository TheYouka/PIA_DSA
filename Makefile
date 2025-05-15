# Makefile para el proyecto Mini-Uber
# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I./include

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

# Asegurarse de que los directorios existan
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Archivos fuente y objetos
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Nombre del ejecutable
TARGET = $(BIN_DIR)/mini-uber

# Regla principal
all: $(TARGET)

# Enlazar los objetos para crear el ejecutable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Compilar cada archivo fuente en su correspondiente objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencias para los archivos de objetos (cabeceras)
-include $(OBJ_FILES:.o=.d)

# Generar archivos de dependencia para cada objeto
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM -I$(INCLUDE_DIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Regla para limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(TARGET)

# Regla para limpiar todo (incluyendo directorios)
clean-all:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Regla para reconstruir todo desde cero
rebuild: clean all

# Ejecución del programa
run: $(TARGET)
	$(TARGET)

# Reglas ficticias que no generan archivos
.PHONY: all clean clean-all rebuild run 