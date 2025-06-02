#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int editDistanceRecursive(string S, string T, int m, int n) {
    // Caso base: si S esta vacio, se requieren n inserciones
    if (m == 0) return n;
    // Caso base: si T esta vacio, se requieren m eliminaciones
    if (n == 0) return m;

    if (S[m-1] == T[n-1]) {
        // Si los caracteres coinciden, no hay costo adicional
        return editDistanceRecursive(S, T, m-1, n-1);
    } else {
        // Minimo entre eliminar (m-1,n) o insertar (m,n-1)
        return 1 + min(editDistanceRecursive(S, T, m-1, n),
                      editDistanceRecursive(S, T, m, n-1));
    }
}

int main() {
    //test case Texto: https://www.gutenberg.org/cache/epub/76146/pg76146.txt
    string s1 = "It is a pleasant duty to repeat my thanks to many friends who have helped me in various ways.";
    string s2 = "Dr Kidston generously and without reserve allowed me access to his splendid collection of Palaeozoic plants";
    cout << editDistanceRecursive(s1, s2, s1.length(), s2.length()) << endl;
    return 0;
}