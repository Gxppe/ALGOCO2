# Nombre de los ejecutables
EXEC1 = Compilados/Fuerza_Bruta
EXEC2 = Compilados/Programacion_Dinamica

# Archivos fuente
SRC1 = Codigos/Fuerza_Bruta.cpp
SRC2 = Codigos/Programacion_Dinamica.cpp

# Compilador y opciones
CXX = g++
CXXFLAGS = -Wall -g

# Crear la carpeta de compilados si no existe
Compilados:
	mkdir -p Compilados

# Objetivo para compilar y ejecutar Programacion_Dinamica.cpp
dinamico: Compilados $(EXEC2)
	@echo "Ejecutando Programacion_Dinamica..."
	@./$(EXEC2)

# Objetivo para compilar y ejecutar Fuerza_Bruta.cpp
bruto: Compilados $(EXEC1)
	@echo "Ejecutando Fuerza_Bruta..."
	@./$(EXEC1)

# Compilación de Programacion_Dinamica.cpp
$(EXEC2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $(EXEC2) $(SRC2)

# Compilación de Fuerza_Bruta.cpp
$(EXEC1): $(SRC1)
	$(CXX) $(CXXFLAGS) -o $(EXEC1) $(SRC1)

# Limpiar archivos compilados
clean:
	rm -rf Compilados
