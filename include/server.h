#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {
public:
    // Singleton access method
    static Server& getInstance(int port = 8080);

    // Delete copy constructor and assignment operator
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void start();
    void stop();

private:
    // Private constructor
    Server(int port);

    // Singleton instance
    static Server* instance;
    static std::mutex instanceMutex;

    int serverSocket;
    int port;
    bool isRunning;

    void initialize();
    void acceptConnections();
};

#endif // SERVER_H