#include <bits/stdc++.h>
using namespace std;

// Definición de funciones de costo

// Costo de sustitución
int costo_sub(char a, char b) {
    return (a != b) ? 2 : 0;  // Retorna 1 si 'a' y 'b' son diferentes, 0 si son iguales
}

// Costo de inserción
int costo_ins(char b) {
    return 1;  // Asumimos un costo fijo de 1 para la inserción
}

// Costo de eliminación
int costo_del(char a) {
    return 1;  // Asumimos un costo fijo de 1 para la eliminación
}

// Costo de transposición
int costo_trans(char a, char b) {
    return (a != b) ? 1 : 0;  // Retorna 1 si 'a' y 'b' son diferentes, 0 si son iguales
}

// Función recursiva para calcular la distancia de edición con costos personalizados
int editDistRec(string& s1, string& s2, int m, int n) {
    // Si la primera cadena está vacía, insertamos todos los caracteres de la segunda cadena
    if (m == 0) return n * costo_ins(s2[n - 1]);

    // Si la segunda cadena está vacía, eliminamos todos los caracteres de la primera cadena
    if (n == 0) return m * costo_del(s1[m - 1]);

    // Si los últimos caracteres de las dos cadenas son iguales
    if (s1[m - 1] == s2[n - 1]) 
        return editDistRec(s1, s2, m - 1, n - 1);

    // Si los últimos caracteres no son iguales, consideramos las cuatro operaciones:
    int insercion = editDistRec(s1, s2, m, n - 1) + costo_ins(s2[n - 1]);  // Inserción
    int eliminacion = editDistRec(s1, s2, m - 1, n) + costo_del(s1[m - 1]);  // Eliminación
    int sustitucion = editDistRec(s1, s2, m - 1, n - 1) + costo_sub(s1[m - 1], s2[n - 1]);  // Sustitución

    // Transposición: Si hay al menos dos caracteres en ambas cadenas y los caracteres actuales e inmediatos anteriores están intercambiados
    int transposicion = INT_MAX;
    if (m > 1 && n > 1 && s1[m - 1] == s2[n - 2] && s1[m - 2] == s2[n - 1]) {
        transposicion = editDistRec(s1, s2, m - 2, n - 2) + costo_trans(s1[m - 1], s2[n - 1]);
    }

    // Retorna el mínimo costo entre inserción, eliminación, sustitución y transposición
    return min({insercion, eliminacion, sustitucion, transposicion});
}

// Función para iniciar el cálculo recursivo
int editDist(string& s1, string& s2) {
    return editDistRec(s1, s2, s1.length(), s2.length());
}

// Código principal
int main() {
    string s1 = "balcon";
    string s2 = "blanco";

    // Resultado de la distancia de edición
    cout << "Distancia de edición: " << editDist(s1, s2) << endl;

    return 0;
}
