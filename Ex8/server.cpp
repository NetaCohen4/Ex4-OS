#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <set>
#include <random>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <stack>
#include <functional>
#include <sstream>

// ======== Graph Class (כמו שלך, קיצרתי כאן לצורך דוגמה) ========

class Graph
{
    int V;

public:
    std::vector<std::vector<int>> adj;

    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool isConnected()
    {
        std::vector<bool> visited(V, false);
        int start = -1;
        for (int i = 0; i < V; ++i)
        {
            if (!adj[i].empty())
            {
                start = i;
                break;
            }
        }
        if (start == -1)
            return true;
        dfs(start, visited);
        for (int i = 0; i < V; ++i)
            if (!visited[i] && !adj[i].empty())
                return false;
        return true;
    }

    void dfs(int v, std::vector<bool> &visited)
    {
        visited[v] = true;
        for (int u : adj[v])
            if (!visited[u])
                dfs(u, visited);
    }

    bool isEulerianCircuitPossible()
    {
        for (int i = 0; i < V; ++i)
            if (adj[i].size() % 2 != 0)
                return false;
        return isConnected();
    }

    std::vector<int> findEulerianCircuit()
    {
        std::vector<int> circuit;
        if (!isEulerianCircuitPossible())
            return circuit;
        std::vector<std::vector<int>> tempAdj = adj;
        std::stack<int> currPath;
        std::vector<int> circuitResult;
        currPath.push(0);
        while (!currPath.empty())
        {
            int v = currPath.top();
            if (!tempAdj[v].empty())
            {
                int u = tempAdj[v].back();
                tempAdj[v].pop_back();
                auto &revList = tempAdj[u];
                revList.erase(std::find(revList.begin(), revList.end(), v));
                currPath.push(u);
            }
            else
            {
                circuitResult.push_back(v);
                currPath.pop();
            }
        }
        std::reverse(circuitResult.begin(), circuitResult.end());
        return circuitResult;
    }
    std::string eulerianCircuit()
    {
        auto circuit = findEulerianCircuit();
        if (circuit.empty())
            return "No Eulerian Circuit";
        std::string result = "Eulerian Circuit: ";
        for (int v : circuit)
            result += std::to_string(v) + " ";
        return result;
    }

    int findMSTWeight()
    {
        std::vector<std::tuple<int, int, int>> edges;
        std::vector<int> parent(V);
        for (int u = 0; u < V; ++u)
        {
            for (int v : adj[u])
            {
                if (u < v)
                    edges.emplace_back(1, u, v);
            }
        }
        std::sort(edges.begin(), edges.end());
        std::function<int(int)> find = [&](int x)
        {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        for (int i = 0; i < V; ++i)
            parent[i] = i;
        int mstWeight = 0;
        for (auto [w, u, v] : edges)
        {
            int ru = find(u), rv = find(v);
            if (ru != rv)
            {
                mstWeight += w;
                parent[ru] = rv;
            }
        }
        return mstWeight;
    }
    std::string MST()
    {
        int weight = findMSTWeight();
        return "MST weight: " + std::to_string(weight);
    }

    bool isClique(const std::vector<int> &nodes)
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            for (size_t j = i + 1; j < nodes.size(); ++j)
            {
                int u = nodes[i], v = nodes[j];
                if (std::find(adj[u].begin(), adj[u].end(), v) == adj[u].end())
                    return false;
            }
        }
        return true;
    }

    int countCliques()
    {
        int count = 0;
        int total = 1 << V; // All subsets

        for (int mask = 1; mask < total; ++mask)
        {
            std::vector<int> subset;
            for (int i = 0; i < V; ++i)
            {
                if (mask & (1 << i))
                    subset.push_back(i);
            }
            if (isClique(subset))
                count++;
        }

        return count;
    }
    std::string numCliques()
    {
        int count = countCliques();
        return "Number of Cliques: " + std::to_string(count);
    }
    void fillOrder(int v, std::vector<bool> &visited, std::stack<int> &Stack)
    {
        visited[v] = true;
        for (int u : adj[v])
            if (!visited[u])
                fillOrder(u, visited, Stack);
        Stack.push(v);
    }

    Graph getTranspose()
    {
        Graph gT(V);
        for (int u = 0; u < V; ++u)
            for (int v : adj[u])
                gT.addEdge(v, u);
        return gT;
    }

    void dfsSCCCollect(int v, std::vector<bool> &visited, std::vector<int> &component)
    {
        visited[v] = true;
        component.push_back(v);
        for (int u : adj[v])
            if (!visited[u])
                dfsSCCCollect(u, visited, component);
    }

    std::vector<std::vector<int>> findSCCGroups()
    {
        std::stack<int> Stack;
        std::vector<bool> visited(V, false);

        // שלב 1: מילוי הסטאק לפי סדר סיום DFS
        for (int i = 0; i < V; ++i)
            if (!visited[i])
                fillOrder(i, visited, Stack);

        // שלב 2: הפיכת הגרף
        Graph gr = getTranspose();
        visited.assign(V, false);

        std::vector<std::vector<int>> sccGroups;

        // שלב 3: הרצת DFS בגרף ההפוך לפי הסדר בסטאק
        while (!Stack.empty())
        {
            int v = Stack.top();
            Stack.pop();
            if (!visited[v])
            {
                std::vector<int> component;
                gr.dfsSCCCollect(v, visited, component);
                sccGroups.push_back(component);
            }
        }

        return sccGroups;
    }

    std::string SCC()
    {
        auto groups = findSCCGroups();
        if (groups.empty())
        {
            return "No Strongly Connected Components found.";
        }

        std::ostringstream oss;
        oss << "Strongly Connected Components:\n";
        for (size_t i = 0; i < groups.size(); ++i)
        {
            oss << "Component " << i + 1 << ": ";
            for (int v : groups[i])
            {
                oss << v << " ";
            }
            oss << "\n";
        }
        return oss.str();
    }

    bool hamiltonUtil(int pos, std::vector<int> &path, std::vector<bool> &visited)
    {
        if (pos == V)
        {
            // בודק חזרה לנקודת ההתחלה
            return std::find(adj[path[pos - 1]].begin(), adj[path[pos - 1]].end(), path[0]) != adj[path[pos - 1]].end();
        }

        for (int v = 1; v < V; ++v)
        {
            if (!visited[v] && std::find(adj[path[pos - 1]].begin(), adj[path[pos - 1]].end(), v) != adj[path[pos - 1]].end())
            {
                path[pos] = v;
                visited[v] = true;
                if (hamiltonUtil(pos + 1, path, visited))
                    return true;
                visited[v] = false;
            }
        }
        return false;
    }

    std::string hamiltonianCircuit()
    {
        std::vector<int> path(V, -1);
        std::vector<bool> visited(V, false);
        path[0] = 0;
        visited[0] = true;

        if (hamiltonUtil(1, path, visited))
        {
            std::string result = "Hamiltonian Circuit: ";
            for (int v : path)
                result += std::to_string(v) + " -> ";
            result += std::to_string(path[0]); // חזרה להתחלה
            return result;
        }
        else
        {
            return "Graph does not have a Hamiltonian Circuit";
        }
    }
};

// ======== Leader-Follower globals ========
int server_fd;
std::mutex mtx;
std::condition_variable cv;
bool hasLeader = false;
bool stopServer = false;

// ======== Handle one client ========
void handleClient(int client_socket, sockaddr_in client_addr)
{
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << "Client connected from " << client_ip << ":" << ntohs(client_addr.sin_port) << "\n";

    int data[3]; // V, E, seed בלבד
    ssize_t bytesRead = read(client_socket, data, sizeof(data));
    if (bytesRead != sizeof(data)) {
        std::cout << "Connection closed or invalid request.\n";
        close(client_socket);
        return;
    }

    int V = data[0], E = data[1], seed = data[2];

    // יצירת גרף רנדומלי
    Graph g(V);
    std::set<std::pair<int,int>> existing;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, V - 1);

    while ((int)existing.size() < E) {
        int u = dist(rng);
        int v = dist(rng);
        if (u == v) continue;
        auto edge = std::minmax(u, v);
        if (!existing.count(edge)) {
            g.addEdge(edge.first, edge.second);
            existing.insert(edge);
        }
    }

    // שליחה של כל תוצאות האלגוריתמים
    std::ostringstream oss;
    oss << g.eulerianCircuit() << "\n";
    oss << g.MST() << "\n";
    oss << g.numCliques() << "\n";
    oss << g.SCC() << "\n";
    oss << g.hamiltonianCircuit() << "\n";

    std::string result = oss.str();
    int len = result.size();
    write(client_socket, &len, sizeof(len));
    write(client_socket, result.c_str(), len);

    close(client_socket);
    std::cout << "Client disconnected.\n";
}

// ======== Worker thread function (Leader-Follower) ========
void workerThread()
{
    while (!stopServer) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_socket = -1;

        // שלב ה-Leader Election
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return !hasLeader || stopServer; });
            if (stopServer) return;

            hasLeader = true; // אני הלידר
            lock.unlock();

            // קבלת חיבור מהלקוח
            client_socket = accept(server_fd, (sockaddr *)&client_addr, &client_len);

            // העברת ההנהגה לפולואר הבא מיד אחרי accept
            lock.lock();
            hasLeader = false;
            cv.notify_one();
            lock.unlock();
        }

        // טיפול בלקוח
        if (client_socket >= 0)
            handleClient(client_socket, client_addr);
    }
}

// ======== Main ========
int main()
{
    struct sockaddr_in address{};
    // int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port 8080...\n";

    // יצירת pool של threads
    const int THREAD_COUNT = 4;
    std::vector<std::thread> threads;
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads.emplace_back(workerThread);
    }

    // לחכות לעצירה (במבחן אמיתי, אפשר לשים Ctrl+C handler)
    for (auto &t : threads)
        t.join();

    close(server_fd);
    return 0;
}
