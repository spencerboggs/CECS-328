#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>

using namespace std;

void parseInput(const string& filename, int &C, vector<pair<int, int>>& items) {
    ifstream file(filename);
    string input;

    if (file.is_open()) {
        getline(file, input);
        file.close();
    } else {
        cout << "Unable to open file!";
        return;
    }

    input.erase(remove_if(input.begin(), input.end(), [](char c) { return isspace(c) || c == '{' || c == '}'; }), input.end());
    size_t pos = input.find(',');

    if (pos == string::npos) {
        cout << "Invalid input format." << endl;
        return;
    }

    C = stoi(input.substr(0, pos));
    
    if (C < 0) {
        cout << "Knapsack capacity cannot be negative." << endl;
        return;
    }

    input.erase(0, pos + 1);
    stringstream ss(input);
    string token;
    while (getline(ss, token, ',')) {
        int v = stoi(token);
        getline(ss, token, ',');
        int w = stoi(token);
        getline(ss, token, ',');
        items.push_back({v, w});
    }
}

int knapSack(int W, const vector<pair<int, int>>& items) {
    if (W <= 0) return 0;

    int n = items.size();
    vector<int> dp(W + 1, 0);

    for (int i = 0; i < n; ++i) {
        int value = items[i].first;
        int weight = items[i].second;
        
        if (weight <= 0) continue;
        
        for (int w = W; w >= weight; --w) {
            dp[w] = max(dp[w], dp[w - weight] + value);
        }
    }

    return dp[W];
}

int main() {
    int C;
    vector<pair<int, int>> items;

    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    parseInput(filename, C, items);

    if (C < 0) {
        cout << "Knapsack capacity cannot be negative." << endl;
        return 1;
    }

    int result = knapSack(C, items);
    cout << "Max value: " << result << endl;

    return 0;
}