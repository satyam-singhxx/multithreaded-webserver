#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void sendRequest(const std::string& serverIp, int port, int requestsPerThread) {
    for (int i = 0; i < requestsPerThread; ++i) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            return;
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        inet_pton(AF_INET, serverIp.c_str(), &serverAddress.sin_addr);

        if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            perror("Connection failed");
            close(sock);
            return;
        }

        std::string request = 
            "GET / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Connection: close\r\n\r\n";
        send(sock, request.c_str(), request.size(), 0);

        char buffer[4096];
        int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            // Uncomment to see the response
            // std::cout << buffer << std::endl;
        }

        close(sock);
    }
}

int main() {
    const std::string serverIp = "127.0.0.1";
    const int port = 8080;         // Replace with your server's port
    const int threads = 100;        // Number of client threads
    const int requestsPerThread = 100; // Requests per thread

    std::vector<std::thread> clients;
    for (int i = 0; i < threads; ++i) {
        clients.emplace_back(sendRequest, serverIp, port, requestsPerThread);
    }

    for (auto& client : clients) {
        client.join();
    }

    std::cout << "Load test completed" << std::endl;
    return 0;
}