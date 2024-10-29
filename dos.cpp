#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <algorithm>
#include <atomic>

const int MAX_THREAD_DEPTH = 4;
std::atomic<int> thread_count(0);

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(std::vector<int>& arr, int low, int high, int depth = 0) {
    if (low < high) {
        int pi = partition(arr, low, high);

        if (depth < MAX_THREAD_DEPTH && thread_count.load() < std::thread::hardware_concurrency()) {
            ++thread_count;
            std::thread left_thread([&arr, low, pi, depth]() {
                quicksort(arr, low, pi - 1, depth + 1);
                --thread_count;
            });

            quicksort(arr, pi + 1, high, depth + 1);
            left_thread.join();
        } else {
            quicksort(arr, low, pi - 1, depth + 1);
            quicksort(arr, pi + 1, high, depth + 1);
        }
    }
}

void read_input(const std::string& filename, int& n, std::vector<int>& numbers) {
    std::ifstream infile(filename);
    std::string line;

    if (infile.is_open()) {
        getline(infile, line);
        std::istringstream iss(line);
        char ch;
        iss >> ch >> n >> ch;
        while (iss >> ch) {
            if (ch == '{' || ch == '}' || ch == ',') continue;
            int num;
            iss.putback(ch);
            iss >> num;
            numbers.push_back(num);
        }
    }
    infile.close();
}

int main() {
    std::string filename;
    int n;
    std::vector<int> numbers;

    std::cout << "Enter the filename: ";
    std::cin >> filename;

    read_input(filename, n, numbers);

    quicksort(numbers, 0, numbers.size() - 1);

    if (n - 1 >= 0 && n - 1 < numbers.size()) {
        std::cout << "The " << n << "-th number in the sorted array is: " << numbers[n - 1] << std::endl;
    } else {
        std::cout << "Error: n is out of bounds." << std::endl;
    }

    return 0;
}