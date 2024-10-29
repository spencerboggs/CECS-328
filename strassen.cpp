#include <bits/stdc++.h>
#include <thread>
using namespace std;

vector<vector<int>> readMatrixFromFile(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> matrix;
    string line;

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    getline(file, line);
    file.close();

    if (line.front() == '{') line.erase(0, 2);
    if (line.back() == '}') line.erase(line.size() - 2);

    stringstream ss(line);
    string row;

    while (getline(ss, row, '}')) {
        if (!row.empty()) {
            row.erase(remove(row.begin(), row.end(), '{'), row.end());
            row.erase(remove(row.begin(), row.end(), ' '), row.end());

            stringstream rowStream(row);
            vector<int> rowValues;
            string value;

            while (getline(rowStream, value, ',')) {
                if (!value.empty()) {
                    rowValues.push_back(stoi(value));
                }
            }

            if (!rowValues.empty()) {
                matrix.push_back(rowValues);
            }
        }
    }

    return matrix;
}

void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& value : row) {
            cout << value << "\t";
        }
        cout << endl;
    }
}

void multiplyRow(const vector<vector<int>>& mat1, const vector<vector<int>>& mat2, vector<vector<int>>& rslt, int row) {
    int C1 = mat1[0].size();
    int C2 = mat2[0].size();
    
    for (int j = 0; j < C2; j++) {
        for (int k = 0; k < C1; k++) {
            rslt[row][j] += mat1[row][k] * mat2[k][j];
        }
    }
}

vector<vector<int>> mulMat(const vector<vector<int>>& mat1, const vector<vector<int>>& mat2) {
    int R1 = mat1.size();
    int C2 = mat2[0].size();
    
    if (mat1[0].size() != mat2.size()) {
        cout << "The number of columns in Matrix-1 must "
                "be equal to the number of rows in "
                "Matrix-2"
             << endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<int>> rslt(R1, vector<int>(C2, 0));

    int quarterThreads = (R1 + 1) / 4; 
    vector<thread> threads;

    for (int i = 0; i < quarterThreads; i++) {
        threads.emplace_back([&mat1, &mat2, &rslt, R1, i, quarterThreads]() {
            for (int row = i; row < R1; row += quarterThreads) {
                multiplyRow(mat1, mat2, rslt, row);
            }
        });
    }

    for (auto& th : threads) {
        th.join();
    }

    return rslt;
}

int sumMatrix(const vector<vector<int>>& matrix) {
    int total = 0;
    for (const auto& row : matrix) {
        for (const auto& value : row) {
            total += value;
        }
    }
    return total;
}

int main() {
    string file1, file2;

    cout << "Enter the first matrix filename: ";
    cin >> file1;
    cout << "Enter the second matrix filename: ";
    cin >> file2;

    vector<vector<int>> mat1 = readMatrixFromFile(file1);
    vector<vector<int>> mat2 = readMatrixFromFile(file2);

    // cout << "Matrix 1:\n";
    // printMatrix(mat1);
    
    // cout << "Matrix 2:\n";
    // printMatrix(mat2);

    vector<vector<int>> multipliedMatrix = mulMat(mat1, mat2);

    int totalSum = sumMatrix(multipliedMatrix);
    cout << "Sum of all elements in the multiplied matrix: " << totalSum << endl;

    return 0;
}