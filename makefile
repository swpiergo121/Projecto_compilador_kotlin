# Makefile

# Nombre del ejecutable
EXEC = kotlin

# Compilador y flags
CXX      = g++
CXXFLAGS = -std=c++17 -g

# Fuentes a compilar
SRC = main.cpp scanner.cpp parser.cpp exp.cpp visitor.cpp token.cpp

.PHONY: all clean

all:
	@echo "Compilando ejecutable '$(EXEC)'..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC)
	@echo "Creando carpeta de salida..."
	mkdir -p outputs
	@echo "Ejecutando batch runner para todos los tests..."
	./$(EXEC)
	@echo "¡Terminado! Revisa los .s en outputs/"

clean:
	@echo "Limpiando ejecutable y salidas..."
	rm -f $(EXEC)
	rm -rf outputs
