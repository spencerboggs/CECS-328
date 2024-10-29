#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void calculateHuffmanLength(Node* root, int depth, int& totalBits) {
    if (!root) return;
    if (!root->left && !root->right) {
        totalBits += root->freq * depth;
    }
    calculateHuffmanLength(root->left, depth + 1, totalBits);
    calculateHuffmanLength(root->right, depth + 1, totalBits);
}

int huffmanBitLength(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return -1;
    }

    unordered_map<char, int> frequency;
    char ch;
    while (file >> noskipws >> ch) {
        frequency[ch]++;
    }
    file.close();

    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    for (const auto& [ch, freq] : frequency) {
        minHeap.push(new Node(ch, freq));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();
        Node* combined = new Node('\0', left->freq + right->freq);
        combined->left = left;
        combined->right = right;
        minHeap.push(combined);
    }

    int totalBits = 0;
    calculateHuffmanLength(minHeap.top(), 0, totalBits);
    return totalBits;
}

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    int bitLength = huffmanBitLength(filename);
    if (bitLength != -1) {
        cout << "Huffman encoding bit-length: " << bitLength << " bits" << endl;
    }

    return 0;
}