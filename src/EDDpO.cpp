#include <string>
#include <vector>
#include <algorithm> 

using namespace std;

int editDistanceDPOptimized(string str1, string str2)
{
    int m = str1.length();
    int n = str2.length();

    vector<int> prev(n + 1, 0);
    vector<int> curr(n + 1, 0);

    for (int j = 0; j <= n; j++)
    {
        prev[j] = j;
    }

    for (int i = 1; i <= m; i++)
    {
        curr[0] = i;
        for (int j = 1; j <= n; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                curr[j] = prev[j - 1];
            }
            else
            {
                curr[j] = 1 + min(prev[j], curr[j - 1]);
            }
        }
        prev = curr;
    }

    return prev[n];
}