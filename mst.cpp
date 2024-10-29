#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

struct Edge {
    int v1, v2;
    double weight;
};

bool compareEdges(const Edge& e1, const Edge& e2) {
    return e1.weight < e2.weight;
}

class UnionFind {
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int v) {
        if (parent[v] != v)
            parent[v] = find(parent[v]);
        return parent[v];
    }

    void unite(int v1, int v2) {
        int root1 = find(v1);
        int root2 = find(v2);
        if (root1 != root2) {
            if (rank[root1] > rank[root2])
                parent[root2] = root1;
            else if (rank[root1] < rank[root2])
                parent[root1] = root2;
            else {
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }

private:
    vector<int> parent;
    vector<int> rank;
};

double findMSTWeight(const vector<Edge>& edges, int vertices) {
    UnionFind uf(vertices + 1);
    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), compareEdges);

    double mstWeight = 0.0;
    int edgesUsed = 0;

    for (const Edge& edge : sortedEdges) {
        if (uf.find(edge.v1) != uf.find(edge.v2)) {
            uf.unite(edge.v1, edge.v2);
            mstWeight += edge.weight;
            edgesUsed++;
            if (edgesUsed == vertices - 1) break;
        }
    }
    return mstWeight;
}

int main() {
    cout << "Enter the filename: ";
    string filename;
    cin >> filename;

    ifstream file(filename);
    if (!file) {
        cerr << "Could not open the file." << endl;
        return 1;
    }

    vector<Edge> edges;
    string line;
    int vertices = 0;

    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    regex edgePattern("\\{(\\d+),\\s*(\\d+),\\s*([0-9]*\\.?[0-9]+)\\}");
    smatch matches;

    string::const_iterator searchStart(fileContent.cbegin());
    while (regex_search(searchStart, fileContent.cend(), matches, edgePattern)) {
        int v1 = stoi(matches[1].str());
        int v2 = stoi(matches[2].str());
        double weight = stod(matches[3].str());

        edges.push_back({v1, v2, weight});
        vertices = max(vertices, max(v1, v2));

        searchStart = matches.suffix().first;
    }

    double mstWeight = findMSTWeight(edges, vertices);
    cout << "Weight of the Minimum Spanning Tree: " << mstWeight << endl;

    return 0;
}