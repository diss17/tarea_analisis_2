#include <string> // Necesario para usar std::string
#include <algorithm> // Para usar std::min

using namespace std; 

int editDistanceRecursive(string S, string T, int m, int n)
{
    // Caso base : si S esta vacio , se requieren n inserciones
    if (m == 0)
        return n;
    // Caso base : si T esta vacio , se requieren m eliminaciones
    if (n == 0)
        return m;

    if (S[m - 1] == T[n - 1])
    {
        // Si los caracteres coinciden , no hay costo adicional
        return editDistanceRecursive(S, T, m - 1, n - 1);
    }
    else
    {
        // Minimo entre eliminar (m -1 , n ) o insertar (m ,n -1)
        return 1 + min(editDistanceRecursive(S, T, m - 1, n),
                       editDistanceRecursive(S, T, m, n - 1));
    }
}