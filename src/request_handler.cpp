#include "../include/request_handler.h"
#include "../include/command.h"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> activeConnections(0);

void RequestHandler::update(int clientSocket) {
    // When a new client connection is received, process the request
    processRequest(clientSocket);
}

std::string RequestHandler::parseHttpRequest(int clientSocket) {
    char buffer[4096];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead < 0) {
        throw std::runtime_error("Error reading from socket");
    }

    buffer[bytesRead] = '\0';
    return std::string(buffer);
}



void RequestHandler::processRequest(int clientSocket) {
    try {
        // Read the HTTP request
        std::string requestString = parseHttpRequest(clientSocket);

        // Parse HTTP request into HttpRequest object
        HttpRequest::Method method = HttpRequest::Method::GET; // Default to GET for simplicity
        std::string path = "/";
        std::string body;

        // A simple parser to extract method and path
        if (requestString.find("POST") == 0) {
            method = HttpRequest::Method::POST;
        }

        // Simulate parsed request
        HttpRequest request(method, path, body);

        // Execute command
        CommandInvoker invoker;
        invoker.executeCommand(request);

        // Send a basic response
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nRequest handled successfully";
        sendHttpResponse(clientSocket, response);
    } catch (const std::exception& e) {
        std::cerr << "Request processing error: " << e.what() << std::endl;

        // Send error response
        sendHttpResponse(clientSocket, "HTTP/1.1 500 Internal Server Error\r\n\r\n");
    }
    close(clientSocket);
}
std::shared_ptr<RequestHandler> RequestHandlerFactory::createHandler() {
    return std::make_shared<RequestHandler>();
}


// Method definition
void RequestHandler::sendHttpResponse(int clientSocket, const std::string& response) {
    std::string httpResponse = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(response.size()) + "\r\n"
        "\r\n" +
        response;

    // Send the HTTP response
    ssize_t bytesSent = send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {
        std::cerr << "Failed to send HTTP response" << std::endl;
    }

    // Close the client socket
    close(clientSocket);
}

// void handleClient(int clientSocket) {
//     activeConnections++;
//     std::cout << "Active connections: " << activeConnections << std::endl;

//     // Handle the client (existing logic)

//     close(clientSocket);
//     activeConnections--;
//     std::cout << "Connection closed. Active connections: " << activeConnections << std::endl;
// }


// void handleClient(int clientSocket) {
//     activeConnections++;
//     std::cout << "Thread ID: " << std::this_thread::get_id() << " - Active connections: " << activeConnections << std::endl;

//     // Handle the client (existing logic)

//     close(clientSocket);
//     activeConnections--;
//     std::cout << "Thread ID: " << std::this_thread::get_id() << " - Connection closed. Active connections: " << activeConnections << std::endl;
// }

void handleClient(int clientSocket) {
    activeConnections++;
    std::cout << "Thread ID: " << std::this_thread::get_id() 
              << " - Active connections: " << activeConnections << std::endl;

    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(clientSocket, response.c_str(), response.size(), 0);

    close(clientSocket);
    activeConnections--;
    std::cout << "Thread ID: " << std::this_thread::get_id() 
              << " - Connection closed. Active connections: " << activeConnections << std::endl;
}

void monitorMetrics() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Update every 5 seconds
        std::cout << "=== Server Metrics ===" << std::endl;
        std::cout << "Active connections: " << activeConnections << std::endl;
        std::cout << "======================" << std::endl;
    }
}