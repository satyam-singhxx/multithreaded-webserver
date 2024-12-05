#include "../include/server.h"
#include "../include/observer.h"
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include "../include/thread_pool.h"
#include "../include/request_handler.h"


Server* Server::instance = nullptr;
std::mutex Server::instanceMutex;

Server& Server::getInstance(int port) {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (!instance) {
        instance = new Server(port);
    }
    return *instance;
}

Server::Server(int port) : port(port), isRunning(false), serverSocket(-1) {}

void Server::initialize() {
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Socket address configuration
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Failed to bind socket");
    }

    // Listen for connections
    if (listen(serverSocket, 10) < 0) {
        close(serverSocket);
        throw std::runtime_error("Failed to listen on socket");
    }
}

void Server::start() {
    if (isRunning) return;

    try {
        initialize();
        isRunning = true;
        std::cout << "Server started on port " << port << std::endl;
        
        // Start accepting connections
        acceptConnections();
    } catch (const std::exception& e) {
        std::cerr << "Server start error: " << e.what() << std::endl;
        stop();
    }
}

void Server::stop() {
    if (!isRunning) return;

    isRunning = false;
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
    std::cout << "Server stopped." << std::endl;
}



void Server::acceptConnections() {
    // Create a thread pool
    ThreadPool threadPool(4); // Number of worker threads

    // Subject to manage observers
    Subject subject;

    // Register a shared request handler
    auto handler = RequestHandlerFactory::createHandler();
    subject.addObserver(handler);

    while (isRunning) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        // Accept client connection
        int clientSocket = accept(serverSocket, 
                                  (struct sockaddr*)&clientAddress, 
                                  &clientAddressLength);

        if (clientSocket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        // Notify observers (e.g., RequestHandler) in a thread-safe way
        threadPool.enqueue([clientSocket, &subject]() {
            subject.notifyObservers(clientSocket);
        });
    }
}