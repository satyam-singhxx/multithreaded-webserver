#include <iostream>
#include <stdexcept>

#include "../include/server.h"
#include "../include/command.h"
#include "../include/thread_pool.h"

int main() {
    try {
        // Create and configure the command invoker
        CommandInvoker commandInvoker;
        commandInvoker.registerCommand(HttpRequest::Method::GET, std::make_unique<GetCommand>());
        commandInvoker.registerCommand(HttpRequest::Method::POST, std::make_unique<PostCommand>());

        // Get singleton server instance
        Server& server = Server::getInstance(8080);

        // Start the server
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}