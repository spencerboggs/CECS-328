#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Task {
    int start;
    int end;
};

int maxTasks(vector<Task>& tasks) {
    sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
        return a.end < b.end;
    });

    int count = 0;
    int lastEnd = -1;

    for (const auto& task : tasks) {
        if (task.start >= lastEnd) {
            count++;
            lastEnd = task.end;
        }
    }

    return count;
}

vector<int> parseInput(const string& input) {
    vector<int> values;
    stringstream ss(input);
    string temp;

    while (getline(ss, temp, ',')) {
        values.push_back(stoi(temp));
    }
    return values;
}

int main() {
    string startInput, endInput;

    cout << "Enter start times (comma-separated): ";
    getline(cin, startInput);
    cout << "Enter end times (comma-separated): ";
    getline(cin, endInput);

    vector<int> startTimes = parseInput(startInput);
    vector<int> endTimes = parseInput(endInput);
    vector<Task> tasks;

    if (startTimes.size() != endTimes.size()) {
        cerr << "Error: Start and end times must have the same number of entries." << endl;
        return 1;
    }

    for (size_t i = 0; i < startTimes.size(); i++) {
        tasks.push_back({startTimes[i], endTimes[i]});
    }

    int result = maxTasks(tasks);
    cout << "Maximum number of tasks: " << result << endl;

    return 0;
}