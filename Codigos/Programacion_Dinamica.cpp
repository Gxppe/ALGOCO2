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

// Función de distancia de edición con costos específicos y optimización de espacio
// Función para calcular la distancia mínima de edición
int editDist_dp(const string &str1, const string &str2, unordered_map<char, int> &costos_del,unordered_map<char, int> &costos_ins, vector<vector<int>>& costos_transponer, vector<vector<int>>& costos_sustituir) {
    int n = str1.length();
    int m = str2.length();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // Inicialización
    for (int i = 1; i <= n; i++) {
        dp[i][0] = dp[i - 1][0] + costo_del(str1[i - 1],costos_del);
    }
    for (int j = 1; j <= m; j++) {
        dp[0][j] = dp[0][j - 1] + costo_ins(str2[j - 1],costos_ins);
    }

    // Llenar la matriz DP
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int costo_delete = dp[i - 1][j] + costo_del(str1[i - 1],costos_del);
            int costo_insert = dp[i][j - 1] + costo_ins(str2[j - 1],costos_ins);
            int costo_substitute = dp[i - 1][j - 1] + costo_sub(str1[i - 1], str2[j - 1],costos_sustituir);

            dp[i][j] = min(costo_delete, min(costo_insert, costo_substitute));

            // Verificar transposición
            if (i > 1 && j > 1 && str1[i - 1] == str2[j - 2] && str1[i - 2] == str2[j - 1]) {
                int costo_transpose = dp[i - 2][j - 2] + costo_trans(str1[i - 1], str1[i - 2],costos_transponer);
                dp[i][j] = min(dp[i][j], costo_transpose);
            }
        }
    }

    return dp[n][m];
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
        int resultado = editDist_dp(str1, str2, costos_del, costos_ins, costos_transponer, costos_sustituir);
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
    vector<vector<int>> costo_tran=cargarCostosMatriz("Costos/Dinamicos/transpose.txt");
    vector<vector<int>> costo_sub=cargarCostosMatriz("Costos/Dinamicos/substitution.txt");
    
    cout << "Repetidos" << endl;
    procesarDataset("Datasets/Repetidos.txt", "Resultados/Dinamicos/Repetidos.csv", costo_del, costo_ins, costo_tran, costo_sub);
    cout << "Traspuestos" << endl;
    procesarDataset("Datasets/Traspuestos.txt", "Resultados/Dinamicos/Traspuestos.csv", costo_del, costo_ins, costo_tran, costo_sub);
    cout << "Vacios" << endl;
    procesarDataset("Datasets/Vacias.txt", "Resultados/Dinamicos/Vacios.csv", costo_del, costo_ins, costo_tran, costo_sub);
    return 0;
}


