#include <iostream>
#include <vector>
#include <string>
using namespace std;

int editDistanceMemoHelper(string &S, string &T, int m, int n, vector<vector<int>> &memo) {
    if (m == 0) return n;
    if (n == 0) return m;

    if (memo[m][n] != -1) return memo[m][n];

    if (S[m-1] == T[n-1]) {
        memo[m][n] = editDistanceMemoHelper(S, T, m-1, n-1, memo);
    } else {
        memo[m][n] = 1 + min(editDistanceMemoHelper(S, T, m-1, n, memo),
                             editDistanceMemoHelper(S, T, m, n-1, memo));
    }
    return memo[m][n];
}

int editDistanceMemo(string S, string T) {
    int m = S.length();
    int n = T.length();
    vector<vector<int>> memo(m+1, vector<int>(n+1, -1));
    return editDistanceMemoHelper(S, T, m, n, memo);
}

int main() {
    //test case Texto: https://www.gutenberg.org/cache/epub/76146/pg76146.txt
    cout << editDistanceMemo("It is a pleasant duty to repeat my thanks to many friends who have helped me in various ways.", "Dr Kidston generously and without reserve allowed me access to his splendid collection of Palaeozoic plants") << endl;

    return 0;
}