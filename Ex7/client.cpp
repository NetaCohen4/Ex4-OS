#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <cstring>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error\n";
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed\n";
        return 1;
    }

    while (true) {
        try {
            int V, E, seed, algoCode;
            std::cout << "Enter number of vertices: ";
            std::cin >> V;
            std::cout << "Enter number of edges: ";
            std::cin >> E;
            std::cout << "Enter seed: ";
            std::cin >> seed;
            while (true) {
                std::cout << "Choose algorithm:\n";
                std::cout << "0 - New graph\n";
                std::cout << "1 - Eulerian Circuit\n";
                std::cout << "2 - MST Weight\n";
                std::cout << "3 - Number of Cliques\n";
                std::cout << "4 - Strongly Connected Components\n";
                std::cout << "5 - Hamiltonian Circuit\n";
                std::cout << "Enter algorithm code: ";
                std::cin >> algoCode;
                if (algoCode > 5){
                    std::cout << "Invalid algorithm code. Please try again.\n";
                    break;
                }
                if (algoCode == 0){
                    break;
                }

                int data[4] = {V, E, seed, algoCode};
                send(sock, data, sizeof(data), 0);

                int msgSize = 0;
                if (read(sock, &msgSize, sizeof(msgSize)) != sizeof(msgSize)) {
                    std::cerr << "Failed to read response size.\n";
                    break;
                }

                if (msgSize <= 0 || msgSize > 10000) {
                    std::cerr << "Invalid message size received.\n";
                    break;
                }

                std::vector<char> buffer(msgSize + 1, 0);
                int received = read(sock, buffer.data(), msgSize);
                if (received != msgSize) {
                    std::cerr << "Incomplete message received.\n";
                    break;
                }

                std::cout << "Server Response: " << buffer.data() << "\n\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }

        std::cout << "\nPress Ctrl+C to exit or continue for another input.\n" << std::endl;
        sleep(1); // Prevent flooding
    }

    close(sock);
    return 0;
}
