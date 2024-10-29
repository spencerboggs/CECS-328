#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <regex>

using namespace std;

vector<vector<int>> parseDimensions(const string &filename) {
    ifstream file(filename);
    string line;
    vector<vector<int>> dimensions;

    if (file.is_open()) {
        getline(file, line);

        regex re("\\d+");
        auto words_begin = sregex_iterator(line.begin(), line.end(), re);
        auto words_end = sregex_iterator();

        vector<int> currentDims;

        for (sregex_iterator it = words_begin; it != words_end; ++it) {
            currentDims.push_back(stoi(it->str()));
            if (currentDims.size() == 2) {
                dimensions.push_back(currentDims);
                currentDims.clear();
            }
        }
        file.close();
    } else {
        cerr << "Error: Could not open file." << endl;
    }

    return dimensions;
}

int matrixChainMultiplication(const vector<vector<int>>& dims) {
    int n = dims.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = numeric_limits<int>::max();

            for (int k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + dims[i][0] * dims[k][1] * dims[j][1];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }

    return dp[0][n - 1];
}

int main() {
    string filename;
    cout << "Enter the file path to the matrix: ";
    cin >> filename;

    vector<vector<int>> dimensions = parseDimensions(filename);

    if (dimensions.empty()) {
        cerr << "Error: Invalid dimensions." << endl;
        return -1;
    }

    int result = matrixChainMultiplication(dimensions);

    cout << "Minimum number of multiplications required: " << result << endl;

    return 0;
}