#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int editDistanceDP(string str1, string str2){
    int filas = str1.length();
    int columnas = str2.length();

    vector<vector<int>> matriz(filas + 1, vector<int>(columnas + 1, 0));
    
    //Casos bases

    for (int i = 0; i <= filas; i++)
    {
        matriz[i][0] = i; // delete i pos
    }
    for (int j = 0; j <= columnas; j++)
    {
        matriz[0][j] = j; // insert j pos
    }
    for (int i = 1; i < filas + 1; i++)
    {
        for (int j = 1; j < columnas + 1; j++)
        {
            if (str1[i-1] == str2[j-1]){
                matriz[i][j] = matriz[i-1][j-1];
            }
            else{
                matriz[i][j] = 1 + min(matriz[i-1][j], matriz[i][j-1]);
            }
        }
    }

    // Reconstrucción de la cadena resultante
    int i = filas, j = columnas;
    string result;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && str1[i-1] == str2[j-1]) {
            result = str1[i-1] + result;
            i--; j--;
        } else if (j > 0 && (i == 0 || matriz[i][j-1] < matriz[i-1][j])) {
            // Insertar carácter de str2
            result = str2[j-1] + result;
            j--;
        } else if (i > 0) {
            // Eliminar carácter de str1
            i--;
        }
    }
    cout << "Cadena resultante: " << result << endl;

    return matriz[filas][columnas];
}
int main() {
    //test case Texto: https://www.gutenberg.org/cache/epub/76146/pg76146.txt
    cout << editDistanceDP("It is a pleasant duty to repeat my thanks to many friends who have helped me in various ways.", "Dr Kidston generously and without reserve allowed me access to his splendid collection of Palaeozoic plants") << endl;

    return 0;
}
