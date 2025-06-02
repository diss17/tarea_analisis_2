#include <iostream>
#include <vector>
#include <string>
using namespace std;

int editDistanceInsertDelete(string str1, string str2) {
    int m = str1.length();
    int n = str2.length();

    vector<int> prev(n + 1, 0);
    vector<int> curr(n + 1, 0);

    for (int j = 0; j <= n; j++) {
        prev[j] = j;  // insertar todos los caracteres de str2
    }

    for (int i = 1; i <= m; i++) {
        curr[0] = i;  // eliminar todos los caracteres de str1
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                curr[j] = prev[j - 1];  // no se necesita operación
            } else {
                curr[j] = 1 + min(prev[j],   // eliminar de str1
                                  curr[j - 1]); // insertar en str1
            }
        }
        prev = curr;
    }

    return prev[n];
}


int main() {
    //test case Texto: https://www.gutenberg.org/cache/epub/76146/pg76146.txt
    cout << editDistanceInsertDelete("It is a pleasant duty to repeat my thanks to many friends who have helped me in various ways.", "Dr Kidston generously and without reserve allowed me access to his splendid collection of Palaeozoic plants") << endl;

    return 0;
}