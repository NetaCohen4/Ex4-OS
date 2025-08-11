#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <stack>
#include <set>
#include <random>
#include <algorithm>
#include <arpa/inet.h>

#define PORT 8080

class Graph {
    int V;
    std::vector<std::vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool isConnected() {
        std::vector<bool> visited(V, false);
        int start = -1;
        for (int i = 0; i < V; ++i) {
            if (!adj[i].empty()) {
                start = i;
                break;
            }
        }
        if (start == -1) return true;
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

    bool isEulerianCircuitPossible() {
        for (int i = 0; i < V; ++i)
            if (adj[i].size() % 2 != 0)
                return false;
        return isConnected();
    }

    std::vector<int> findEulerianCircuit() {
        std::vector<int> circuit;
        if (!isEulerianCircuitPossible())
            return circuit;

        std::vector<std::vector<int>> tempAdj = adj;
        std::stack<int> currPath;
        std::vector<int> circuitResult;

        currPath.push(0);
        while (!currPath.empty()) {
            int v = currPath.top();
            if (!tempAdj[v].empty()) {
                int u = tempAdj[v].back();
                tempAdj[v].pop_back();
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

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) continue;

        std::cout << "Client connected from "
            << inet_ntoa(address.sin_addr)
            << ":" << ntohs(address.sin_port) << std::endl;

        int data[3]; // V, E, seed
        read(new_socket, data, sizeof(data));
        int V = data[0], E = data[1], seed = data[2];

        Graph g(V);
        std::set<std::pair<int, int>> existing;
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(0, V - 1);

        while (existing.size() < (size_t)E) {
            int u = dist(rng);
            int v = dist(rng);
            if (u == v) continue;
            auto edge = std::minmax(u, v);
            if (!existing.count(edge)) {
                g.addEdge(edge.first, edge.second);
                existing.insert(edge);
            }
        }

        std::vector<int> circuit = g.findEulerianCircuit();
        int size = circuit.size();
        write(new_socket, &size, sizeof(size));
        if (size > 0)
            write(new_socket, circuit.data(), size * sizeof(int));
        close(new_socket);
    }

    return 0;
}
