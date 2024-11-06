#include <bits/stdc++.h>
using namespace std;

// Función para calcular el costo de sustituir el carácter 'a' por 'b'
int costo_sub(char a, char b) {

    return (a != b) ? 2 : 0; // Costo de sustitución es 2 si son diferentes
}

// Función para calcular el costo de insertar el carácter 'b'
int costo_ins(char b) {

    return 1; // Costo de inserción es 1
}

// Función para calcular el costo de eliminar el carácter 'a'
int costo_del(char a) {

    return 1; // Costo de eliminación es 1
}

// Función para calcular el costo de transponer los caracteres 'a' y 'b'
int costo_trans(char a, char b) {
    return (a != b) ? 1 : 0;  // Retorna 1 si 'a' y 'b' son diferentes, 0 si son iguales
}

// Función de distancia de edición con costos específicos y optimización de espacio
int editDist(string& s1, string& s2) {
    int m = s1.size();
    int n = s2.size();

    // prev almacena resultados para la fila (i-1)
    // y curr para la fila i
    vector<int> prev(n + 1, 0), curr(n + 1, 0);
    
    // Inicializar la fila 0 (costos de inserción)
    for (int j = 0; j <= n; j++)
        prev[j] = j * costo_ins(s2[j - 1]);

    // Resto de las filas
    for (int i = 1; i <= m; i++) {
        curr[0] = i * costo_del(s1[i - 1]);  // Costo de eliminación para j = 0

        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1]; // Sin costo si los caracteres coinciden
            }else if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                int costoTransposicion = prev[j - 2] + costo_trans(s1[i - 1], s2[j - 1]);
                curr[j] = min(curr[j], costoTransposicion);
            }
            else {
                // Calcular costos específicos para cada operación
                int costoInsercion = curr[j - 1] + costo_ins(s2[j - 1]);
                int costoEliminacion = prev[j] + costo_del(s1[i - 1]);
                int costoSustitucion = prev[j - 1] + costo_sub(s1[i - 1], s2[j - 1]);

                // Encontrar el mínimo costo de inserción, eliminación o sustitución
                curr[j] = min({costoInsercion, costoEliminacion, costoSustitucion});
            }
        }
        prev = curr; // Actualizar la fila anterior
    }

    return prev[n];
}

int main() {
    string s1 = "balcon";
    string s2 = "blanco";

    cout << "La distancia de edición con costos específicos es: " << editDist(s1, s2) << "\n";
    return 0;
}
