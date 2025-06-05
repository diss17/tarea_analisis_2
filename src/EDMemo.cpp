#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int editDistanceMemoHelper(string &S, string &T, int m, int n, vector<vector<int>> &memo)
{
    if (m == 0)
        return n;
    if (n == 0)
        return m;
    if (memo[m][n] != -1)
        return memo[m][n];
    if (S[m - 1] == T[n - 1])
    {
        memo[m][n] = editDistanceMemoHelper(S, T, m - 1, n - 1, memo);
    }
    else
    {
        memo[m][n] = 1 + min(editDistanceMemoHelper(S, T, m - 1, n, memo),
                             editDistanceMemoHelper(S, T, m, n - 1, memo));
    }
    return memo[m][n];
}

int editDistanceMemo(string S, string T)
{
    int m = S.length();
    int n = T.length();
    vector<vector<int>> memo(m + 1, vector<int>(n + 1, -1));
    return editDistanceMemoHelper(S, T, m, n, memo);
}
