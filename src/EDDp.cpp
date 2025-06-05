#include <string>
#include <vector>
#include <algorithm> 

using namespace std;

int editDistanceDP(string str1, string str2)
{
    int filas = str1.length();
    int columnas = str2.length();

    // Creamos una matriz de ( filas +1) x ( columnas +1) para almacenar los resultados parciales
    vector<vector<int>>
        matriz(filas + 1, vector<int>(columnas + 1, 0));

    // Caso base : transformar str1 [0.. i -1] en cadena vacia requiere ieliminaciones
    for (int i = 0; i <= filas; i++)
        matriz[i][0] = i;

    // Caso base : transformar cadena vacia en str2 [0.. j -1] requiere j inserciones
    for (int j = 0; j <= columnas; j++)
        matriz[0][j] = j;

    // Llenamos la matriz usando la relacion de recurrencia
    for (int i = 1; i <= filas; i++)
    {
        for (int j = 1; j <= columnas; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                // Si los caracteres son iguales , no se requiere operacion extra
                matriz[i][j] = matriz[i - 1][j - 1];
            }
            else
            {
                // Si son diferentes , elegimos la mejor opcion entre insertar o eliminar
                matriz[i][j] = 1 + min(matriz[i - 1][j], matriz[i][j - 1]);
            }
        }
    }
    // El resultado final es la distancia minima de edicion
    return matriz[filas][columnas];
}
