#include <iostream>
#include <getopt.h>
#include <random>
#include <set>
#include <vector>
#include <stack>
#include <algorithm>

class Graph {
private:
    int V;
    std::vector<std::vector<int>> adj;
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

    bool isEulerianCircuitPossible() {
        for (int i = 0; i < V; ++i) {
            if (adj[i].size() % 2 != 0)
                return false; // odd degree
        }
        return isConnected(); // and connected
    }

    bool isConnected() {
        std::vector<bool> visited(V, false);
        int start = -1;

        // Find non-zero degree vertex
        for (int i = 0; i < V; ++i) {
            if (!adj[i].empty()) {
                start = i;
                break;
            }
        }
        if (start == -1) return true; // No edges

        dfs(start, visited);

        for (int i = 0; i < V; ++i) {
            if (!visited[i] && !adj[i].empty())
                return false;
        }
        return true;
    }

    void dfs(int v, std::vector<bool>& visited) {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u])
                dfs(u, visited);
        }
    }

    std::vector<int> findEulerianCircuit() {
        std::vector<int> circuit;
        if (!isEulerianCircuitPossible()) {
            std::cout << "Eulerian Circuit does not exist.\n";
            return circuit;
        }

        std::vector<std::vector<int>> tempAdj = adj; // Copy
        std::stack<int> currPath;
        std::vector<int> circuitResult;

        currPath.push(0); // Start from vertex 0

        while (!currPath.empty()) {
            int v = currPath.top();
            if (!tempAdj[v].empty()) {
                int u = tempAdj[v].back();
                tempAdj[v].pop_back();

                // Remove reverse edge
                auto& revList = tempAdj[u];
                revList.erase(std::find(revList.begin(), revList.end(), v));

                currPath.push(u);
            } else {
                circuitResult.push_back(v);
                currPath.pop();
            }
        }

        std::reverse(circuitResult.begin(), circuitResult.end());
        return circuitResult;
    }
};


void usage(const char* progname) {
    std::cerr << "Usage: " << progname << " -v <vertices> -e <edges> -s <seed>\n";
}

int main(int argc, char* argv[]) {
    int V = 0, E = 0, seed = time(nullptr);
    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:")) != -1) {
        switch (opt) {
            case 'v': V = std::stoi(optarg); break;
            case 'e': E = std::stoi(optarg); break;
            case 's': seed = std::stoi(optarg); break;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    if (V <= 0 || E < 0) {
        usage(argv[0]);
        return 1;
    }

    Graph g(V);
    std::set<std::pair<int, int>> existing;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, V - 1);

    while (existing.size() < (size_t)E) {
        int u = dist(rng); // first fandom vertex
        int v = dist(rng); // second random vertex
        if (u == v) continue;
        auto edge = std::minmax(u, v);
        if (existing.count(edge) == 0) {
            g.addEdge(edge.first, edge.second);
            existing.insert(edge);
        }
    }

    std::cout << "Generated Graph with " << V << " vertices and " << E << " edges\n";
    g.printGraph();
    auto circuit = g.findEulerianCircuit();
    if (!circuit.empty()) {
        std::cout << "Eulerian Circuit: ";
        for (int v : circuit) std::cout << v << " ";
        std::cout << "\n";
    }

    return 0;
}
