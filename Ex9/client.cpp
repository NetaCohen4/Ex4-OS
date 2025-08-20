#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <cstring>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Socket creation error\n";
        return 1;
    }

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

        int msgSize = 0;
        if (read(sock, &msgSize, sizeof(msgSize)) != sizeof(msgSize))
        {
            std::cerr << "Failed to read response size.\n";
        }

        if (msgSize <= 0 || msgSize > 10000)
        {
            std::cerr << "Invalid message size received.\n";
        }

        std::vector<char> buffer(msgSize + 1, 0);
        int received = read(sock, buffer.data(), msgSize);
        if (received != msgSize)
        {
            std::cerr << "Incomplete message received.\n";
        }

        std::cout << "Server Response: \n"
                  << buffer.data();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    close(sock);
    return 0;
}
