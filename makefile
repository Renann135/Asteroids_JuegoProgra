# Directorios
SRC_DIR := src
BIN_DIR := bin
INC_DIR := include

# Librerías (Solo SFML)
SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main -lbox2d
# 1. Encontrar todos los archivos .cpp
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# 2. Convertir nombres .cpp a .o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_FILES))

# Nombre del ejecutable final
TARGET := $(BIN_DIR)/Asteroids.exe

# Regla por defecto (la que corre cuando escribes 'make')
all: $(TARGET)

# 3. Linkeo final
$(TARGET): $(OBJ_FILES)
	g++ $^ -o $@ $(SFML)

# 4. Compilación de objetos (REVISA QUE HAYA TABULACIÓN ANTES DE g++ y @mkdir)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	g++ -c $< -o $@ -I$(INC_DIR)

# Ejecutar el juego
run: all
	./$(TARGET)

# Limpiar archivos basura (En Windows usa del en vez de rm si rm falla)
clean:
	del /Q $(BIN_DIR)\*.o $(BIN_DIR)\Asteroids.exe

.PHONY: all run clean