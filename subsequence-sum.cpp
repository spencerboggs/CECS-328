#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;

struct Result {
    int maxSum;
    int start;
    int end;
};

void maxSubArraySum(const vector<int>& arr, int startIdx, int endIdx, Result& result) {
    int maxSoFar = arr[startIdx];
    int maxEndingHere = arr[startIdx];
    int tempStart = startIdx;

    for (int i = startIdx + 1; i <= endIdx; ++i) {
        maxEndingHere += arr[i];

        if (maxEndingHere < arr[i]) {
            maxEndingHere = arr[i];
            tempStart = i;
        }

        if (maxEndingHere > maxSoFar) {
            maxSoFar = maxEndingHere;
            result.start = tempStart;
            result.end = i;
        }
    }

    result.maxSum = maxSoFar;
}

Result findMaxSubArray(const vector<int>& arr) {
    Result maxResult = {arr[0], 0, 0};
    Result tempResult = {arr[0], 0, 0};
    
    int currentStart = 0;
    int currentSum = arr[0];

    for (int i = 1; i < arr.size(); ++i) {
        currentSum += arr[i];

        if (currentSum < arr[i]) {
            currentSum = arr[i];
            currentStart = i;
        }

        if (currentSum > maxResult.maxSum) {
            maxResult.maxSum = currentSum;
            maxResult.start = currentStart;
            maxResult.end = i;
        }
    }

    return maxResult;
}

vector<int> readFile(const string& filename) {
    ifstream file(filename);
    vector<int> arr;
    string line;

    if (file.is_open()) {
        getline(file, line);
        
        line.erase(remove(line.begin(), line.end(), '{'), line.end());
        line.erase(remove(line.begin(), line.end(), '}'), line.end());
        
        stringstream ss(line);
        string number;

        while (getline(ss, number, ',')) {
            arr.push_back(stoi(number));
        }

        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }

    return arr;
}

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    vector<int> arr = readFile(filename);
    if (arr.empty()) {
        cerr << "The array is empty or could not be read from the file." << endl;
        return 1;
    }

    Result maxResult = findMaxSubArray(arr);

    cout << "The largest sum of continuous subsequences is: ";
    for (int i = maxResult.start; i <= maxResult.end; ++i) {
        cout << arr[i] << " ";
    }
    cout << "\nLargest Sum: " << maxResult.maxSum << endl;

    return 0;
}
