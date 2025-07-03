#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

class Graph {
private:
    std::vector<std::vector<int>> adj;
    int V;
public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected
    }

    void printGraph() const {
        for (int i = 0; i < V; ++i) {
            std::cout << i << " -> ";
            for (int neighbor : adj[i]) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }
    
};
