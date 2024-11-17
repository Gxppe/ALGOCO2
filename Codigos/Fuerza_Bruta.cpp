#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

int charToIndex(char c) {
    if (c == ' ') return 26;        // El espacio tiene índice 26
    return c - 'a';                 // Convertir de 'a' a 'z' a índices 0-25
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

    if (a =='*'){
        return costos[costos.size()-1][charToIndex(b)];
    }else if (b =='*'){
        return costos[charToIndex(a)][costos.size()-1];
    }
    // Validar índices para evitar acceso fuera de rango
    if (indexA < 0 || indexA >= 27 || indexB < 0 || indexB >= 27) {
        cout << "Caracteres no válidos(sub): " << a << ", " << b << endl;
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
        cout << "Caracteres no válidos(trans): " << a << ", " << b << endl;
        return -1;
    }
    // Retornar el costo de sustitución desde la matriz
    return costos[indexA][indexB];
}

// Función recursiva para calcular la distancia de edición con costos personalizados
int editDist(string& s1, string& s2, int m, int n, unordered_map<char, int> &costos_del,unordered_map<char, int> &costos_ins, vector<vector<int>>& costos_transponer, vector<vector<int>>& costos_sustituir) {
    // Si la primera cadena está vacía, insertamos todos los caracteres de la segunda cadena
    if (m == 0) return n * costo_ins(s2[n - 1],costos_ins);

    // Si la segunda cadena está vacía, eliminamos todos los caracteres de la primera cadena
    if (n == 0) return m * costo_del(s1[m - 1],costos_del);

    // Si los últimos caracteres de las dos cadenas son iguales
    if (s1[m - 1] == s2[n - 1]) 
        return editDist(s1, s2, m - 1, n - 1,costos_del,costos_ins,costos_transponer,costos_sustituir);

    // Si los últimos caracteres no son iguales, consideramos las cuatro operaciones:
    int insercion = editDist(s1, s2, m, n - 1,costos_del,costos_ins,costos_transponer,costos_sustituir) + costo_ins(s2[n - 1],costos_ins);  // Inserción
    int eliminacion = editDist(s1, s2, m - 1, n,costos_del,costos_ins,costos_transponer,costos_sustituir) + costo_del(s1[m - 1],costos_del);  // Eliminación
    int sustitucion = editDist(s1, s2, m - 1, n - 1,costos_del,costos_ins,costos_transponer,costos_sustituir) + costo_sub(s1[m - 1], s2[n - 1],costos_sustituir);  // Sustitución

    // Transposición: Si hay al menos dos caracteres en ambas cadenas y los caracteres actuales e inmediatos anteriores están intercambiados
    int transposicion = INT_MAX;
    if (m > 1 && n > 1 && s1[m - 1] == s2[n - 2] && s1[m - 2] == s2[n - 1]) {
        transposicion = editDist(s1, s2, m - 2, n - 2,costos_del,costos_ins,costos_transponer,costos_sustituir) + costo_trans(s1[m - 1], s2[n - 1],costos_transponer);
    }

    // Retorna el mínimo costo entre inserción, eliminación, sustitución y transposición
    return min({insercion, eliminacion, sustitucion, transposicion});
}

// Función para iniciar el cálculo recursivo
int editDist(string& s1, string& s2,unordered_map<char, int> &costos_del,unordered_map<char, int> &costos_ins, vector<vector<int>>& costos_transponer, vector<vector<int>>& costos_sustituir) {
    return editDist(s1, s2, s1.length(), s2.length(),costos_del,costos_ins,costos_transponer,costos_sustituir);
}

void procesarDataset(const string& inputFilename, const string& outputFilename,
                     unordered_map<char, int>& costos_del, unordered_map<char, int>& costos_ins,
                     vector<vector<int>>& costos_transponer, vector<vector<int>>& costos_sustituir) {
    ifstream inputFile(inputFilename);
    ofstream outputFile(outputFilename);

    if (!inputFile || !outputFile) {
        cerr << "Error al abrir los archivos." << endl;
        return;
    }

    int N;
    inputFile >> N;
    inputFile.ignore();

    // Escribir encabezado del archivo CSV
    outputFile << "largo_str1,largo_str2,tiempo_microsegundos,distancia_edicion\n";

    for (int i = 0; i < N; ++i) {
        string str1, str2;
        getline(inputFile, str1, ' ');
        getline(inputFile, str2);

        if (str1.empty()) str1 = "\"\"";
        if (str2.empty()) str2 = "\"\"";

        auto start = high_resolution_clock::now();
        int resultado = editDist(str1, str2, costos_del, costos_ins, costos_transponer, costos_sustituir);
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(end - start).count();

        // Nombre de la instancia basado en el índice


        // Escribir la línea en formato CSV
        outputFile << str1.length() << ","
                   << str2.length() << ","
                   << duration << ","
                   << resultado << "\n";
    }

    inputFile.close();
    outputFile.close();
}

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

// Código principal
int main() {
    unordered_map<char, int> costo_del = cargarCostosVector("Costos/Dinamicos/delete.txt");
    unordered_map<char, int> costo_ins = cargarCostosVector("Costos/Dinamicos/insert.txt");
    vector<vector<int>> costo_tran = cargarCostosMatriz("Costos/Dinamicos/transpose.txt");
    vector<vector<int>> costo_sub = cargarCostosMatriz("Costos/Dinamicos/substitution.txt");

    cout << "Repetidos" << endl;
    procesarDataset("Datasets/Repetidos.txt", "Resultados/Dinamicos/Repetidos.csv", costo_del, costo_ins, costo_tran, costo_sub);
    cout << "Traspuestos" << endl;
    procesarDataset("Datasets/Traspuestos.txt", "Resultados/Dinamicos/Traspuestos.csv", costo_del, costo_ins, costo_tran, costo_sub);
    cout << "Vacios" << endl;
    procesarDataset("Datasets/Vacias.txt", "Resultados/Dinamicos/Vacios.csv", costo_del, costo_ins, costo_tran, costo_sub);

    return 0;
}
