### Simple Makefile for Asteroids project

SRC_DIR := src
BIN_DIR := bin
INC_DIR := include

# Only SFML libraries (edit if your SFML installation differs)
SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main -lbox2d

CXX := g++
CXXFLAGS := -I$(INC_DIR) -std=c++17 -O2
MKDIR_P := mkdir -p

CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_FILES))

TARGET := $(BIN_DIR)/Asteroids.exe

all: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(SFML)

$(BIN_DIR):
	$(MKDIR_P) $(BIN_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	@mkdir -p $(BIN_DIR)
	$(CXX) -c $< -o $@ -I$(INC_DIR) -std=c++17 -O2

run: all
	./$(TARGET)

clean:
	del /Q $(BIN_DIR)\*.o $(TARGET) 2>nul || rm -f $(BIN_DIR)/*.o $(TARGET)

.PHONY: all run clean