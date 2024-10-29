#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
#include <utility>
#include <algorithm>

using namespace std;

vector<tuple<int, int, int>> read_graph(const string& filename) {
    vector<tuple<int, int, int>> edges;
    ifstream file(filename);
    string data;

    if (file) {
        getline(file, data);
        file.close();
    }

    if (data.size() >= 4 && data.substr(0, 2) == "{{" && data.substr(data.size() - 2) == "}}") {
        data = data.substr(2, data.size() - 4);
    } else if (data.size() >= 2 && data.front() == '{' && data.back() == '}') {
        data = data.substr(1, data.size() - 2);
    }

    stringstream ss(data);
    string part;
    while (getline(ss, part, '}')) {
        if (part.back() == ',') part.pop_back();
        part.erase(remove(part.begin(), part.end(), '{'), part.end());
        part.erase(remove(part.begin(), part.end(), ' '), part.end());

        stringstream component_stream(part);
        string component;
        vector<int> components;
        while (getline(component_stream, component, ',')) {
            if (!component.empty()) {  
                try {
                    components.push_back(stoi(component));
                } catch (const invalid_argument& e) {
                    cout << "Error converting part to integers: " << part << ". Error: " << e.what() << endl;
                    exit(1);
                }
            }
        }

        if (components.size() == 3) {
            edges.emplace_back(components[0], components[1], components[2]);
        }
    }
    return edges;
}

int dijkstra(int n, const vector<tuple<int, int, int>>& edges, int start, int end) {
    vector<vector<pair<int, int>>> graph(n + 1);
    for (const auto& [u, v, w] : edges) {
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    vector<int> distances(n + 1, numeric_limits<int>::max());
    distances[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> priority_queue;
    priority_queue.emplace(0, start);

    while (!priority_queue.empty()) {
        auto [current_distance, current_vertex] = priority_queue.top();
        priority_queue.pop();

        if (current_distance > distances[current_vertex]) {
            continue;
        }

        for (const auto& [neighbor, weight] : graph[current_vertex]) {
            int distance = current_distance + weight;

            if (distance < distances[neighbor]) {
                distances[neighbor] = distance;
                priority_queue.emplace(distance, neighbor);
            }
        }
    }
    return distances[end];
}

int main() {
    auto edges = read_graph("./graph.txt");
    int start_vertex;
    int end_vertex;
    cout << "Enter the start vertex: ";
    cin >> start_vertex;
    cout << "Enter the end vertex: ";
    cin >> end_vertex;
    int distance = dijkstra(1000, edges, start_vertex, end_vertex);

    if (distance != numeric_limits<int>::max()) {
        cout << "The shortest distance from vertex " << start_vertex << " to vertex " << end_vertex << " is " << distance << endl;
    } else {
        cout << "No path exists from vertex " << start_vertex << " to vertex " << end_vertex << endl;
    }

    return 0;
}