#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <ctime>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection Failed\n";
        return 1;
    }
    try
    {
        int V, E, seed;
        std::cout << "Enter number of vertices: ";
        std::cin >> V;
        std::cout << "Enter number of edges: ";
        std::cin >> E;
        std::cout << "Enter seed: ";
        std::cin >> seed;
        int data[3] = {V, E, seed};

        send(sock, data, sizeof(data), 0);

        int size = 0;
        read(sock, &size, sizeof(size));

        if (size == 0)
        {
            std::cout << "No Eulerian Circuit found.\n";
        }
        else
        {
            std::vector<int> circuit(size);
            read(sock, circuit.data(), size * sizeof(int));
            std::cout << "Eulerian Circuit: ";
            for (int v : circuit)
                std::cout << v << " ";
            std::cout << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    close(sock);
    return 0;
}
