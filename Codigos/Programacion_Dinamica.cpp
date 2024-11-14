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
        cout << "entre a "<< endl;
        return costos[costos.size()-1][charToIndex(b)];
    }else if (b =='*'){
        cout << "entre b "<< endl;
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

// Función de distancia de edición con costos específicos y optimización de espacio
int editDist(string& s1, string& s2,unordered_map<char, int> &costos_delete,unordered_map<char, int> &costos_insert, vector<vector<int>>& costos_transponer, vector<vector<int>>& costos_sustituir) {
    int m = s1.size();
    int n = s2.size();

    // prev almacena resultados para la fila (i-1)
    // y curr para la fila i
    vector<int> prev(n + 1, 0), curr(n + 1, 0);
    
    // Inicializar la fila 0 (costos de inserción)
    for (int j = 0; j <= n; j++)
        prev[j] = j * costo_ins(s2[j - 1],costos_insert);

    // Resto de las filas
    for (int i = 1; i <= m; i++) {
        curr[0] = i * costo_del(s1[i - 1],costos_delete);  // Costo de eliminación para j = 0

        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1]; // Sin costo si los caracteres coinciden
            }else if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                int costoTransposicion = prev[j - 2] + costo_trans(s1[i - 1], s2[j - 1],costos_transponer);
                curr[j] = min(curr[j], costoTransposicion);
            }
            else {
                // Calcular costos específicos para cada operación
                int costoInsercion = curr[j - 1] + costo_ins(s2[j - 1],costos_insert);
                int costoEliminacion = prev[j] + costo_del(s1[i - 1],costos_delete);
                int costoSustitucion = prev[j - 1] + costo_sub(s1[i - 1], s2[j - 1],costos_sustituir);

                // Encontrar el mínimo costo de inserción, eliminación o sustitución
                curr[j] = min({costoInsercion, costoEliminacion, costoSustitucion});
            }
        }
        prev = curr; // Actualizar la fila anterior
    }

    return prev[n];
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



int main() {
    unordered_map<char,int> costo_del=cargarCostosVector("Costos/Dinamicos/delete.txt");
    unordered_map<char,int> costo_ins=cargarCostosVector("Costos/Dinamicos/insert.txt");
    vector<vector<int>> costo_trans=cargarCostosMatriz("Costos/Dinamicos/substitution.txt");
    vector<vector<int>> costo_sub=cargarCostosMatriz("Costos/Dinamicos/transpose.txt");

    cout << "Repetidos" << endl;
    procesarDataset("Datasets/Repetidos.txt", "Resultados/Dinamicos/Repetidos.csv", costo_del, costo_ins, costo_trans, costo_sub);
    cout << "Traspuestos" << endl;
    procesarDataset("Datasets/Traspuestos.txt", "Resultados/Dinamicos/Traspuestos.csv", costo_del, costo_ins, costo_trans, costo_sub);
    cout << "Vacios" << endl;
    procesarDataset("Datasets/Vacias.txt", "Resultados/Dinamicos/Vacios.csv", costo_del, costo_ins, costo_trans, costo_sub);


}
