#include <bits/stdc++.h>
using namespace std;

int charToIndex(char c) {
    if (c == ' ') return 26;        // El espacio tiene índice 26
    return c - 'a';                 // Convertir de 'a' a 'z' a índices 0-25
}

// Función para cargar los costos de eliminación desde el archivo "delete.txt"
unordered_map<char, int> cargarCostosVector(const string& filename) {
    unordered_map<char, int> costos;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo " << filename << endl;
        return costos;
    }

    char letra;
    int costo;

    // Leer cada línea y llenar el mapa con los costos
    while (file >> letra >> costo) {
        costos[letra] = costo;
    }

    file.close();
    return costos;
}

// Función para leer una matriz desde un archivo de texto
vector<vector<int>> cargarCostosMatriz(const string &nombreArchivo) {
    ifstream file(nombreArchivo);
    vector<vector<int>> matriz;
    string line;

    // Verificar si el archivo se abrió correctamente
    if (!file.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return matriz;
    }

    // Leer cada línea del archivo
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> fila;
        int valor;

        // Leer cada valor entero en la línea
        while (ss >> valor) {
            fila.push_back(valor);
        }

        // Añadir la fila a la matriz
        matriz.push_back(fila);
    }

    file.close();
    return matriz;
}

int costo_del(char letra, const unordered_map<char, int>& costos) {
    // Validar si la letra está en el mapa de costos
    if (costos.find(letra) != costos.end()) {
        return costos.at(letra);
    } 
    return -1;
}

int costo_ins(char letra, const unordered_map<char, int>& costos) {
    // Validar si la letra está en el mapa de costos
    if (costos.find(letra) != costos.end()) {
        return costos.at(letra);
    } 
    return -1;
}



// Función para calcular el costo de sustitución entre dos caracteres
int costo_sub(char a, char b, const vector<vector<int>>& costos) {
    int indexA = charToIndex(a);
    int indexB = charToIndex(b);

    // Validar índices para evitar acceso fuera de rango
    if (indexA < 0 || indexA >= 27 || indexB < 0 || indexB >= 27) {
        cout << "Caracteres no válidos: " << a << ", " << b << endl;
        return -1;
    }

    // Retornar el costo de sustitución desde la matriz
    return costos[indexA][indexB];
}

int costo_trans(char a, char b, const vector<vector<int>>& costos) {
    int indexA = charToIndex(a);
    int indexB = charToIndex(b);

    // Validar índices para evitar acceso fuera de rango
    if (indexA < 0 || indexA >= 27 || indexB < 0 || indexB >= 27) {
        cout << "Caracteres no válidos: " << a << ", " << b << endl;
        return -1;
    }

    // Retornar el costo de sustitución desde la matriz
    return costos[indexA][indexB];
}


// Función de distancia de edición con costos específicos y optimización de espacio
int editDist(string& s1, string& s2,unordered_map<char, int> &costos_del,unordered_map<char, int> &costos_ins, vector<vector<int>>& costos_transponer, vector<vector<int>>& costos_sustituir) {
    int m = s1.size();
    int n = s2.size();

    // prev almacena resultados para la fila (i-1)
    // y curr para la fila i
    vector<int> prev(n + 1, 0), curr(n + 1, 0);
    
    // Inicializar la fila 0 (costos de inserción)
    for (int j = 0; j <= n; j++)
        prev[j] = j * costo_ins(s2[j - 1],costos_ins);

    // Resto de las filas
    for (int i = 1; i <= m; i++) {
        curr[0] = i * costo_del(s1[i - 1],costos_del);  // Costo de eliminación para j = 0

        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1]; // Sin costo si los caracteres coinciden
            }else if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                int costoTransposicion = prev[j - 2] + costo_trans(s1[i - 1], s2[j - 1],costos_transponer);
                curr[j] = min(curr[j], costoTransposicion);
            }
            else {
                // Calcular costos específicos para cada operación
                int costoInsercion = curr[j - 1] + costo_ins(s2[j - 1],costos_ins);
                int costoEliminacion = prev[j] + costo_del(s1[i - 1],costos_del);
                int costoSustitucion = prev[j - 1] + costo_sub(s1[i - 1], s2[j - 1],costos_sustituir);

                // Encontrar el mínimo costo de inserción, eliminación o sustitución
                curr[j] = min({costoInsercion, costoEliminacion, costoSustitucion});
            }
        }
        prev = curr; // Actualizar la fila anterior
    }

    return prev[n];
}

int main() {
    unordered_map<char,int> costo_del=cargarCostosVector("Costos/Dinamicos/delete.txt");
    unordered_map<char,int> costo_ins=cargarCostosVector("Costos/Dinamicos/insert.txt");
    vector<vector<int>> costo_trans=cargarCostosMatriz("Costos/Dinamicos/substitution.txt");
    vector<vector<int>> costo_sub=cargarCostosMatriz("Costos/Dinamicos/transpose.txt");

    


}
