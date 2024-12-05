#include "../include/command.h"
#include <iostream>
#include <unordered_map>

void GetCommand::execute(const HttpRequest& request) {
    std::cout << "Executing GET request for path: " << request.getPath() << std::endl;
    // Implement GET request logic
}

void PostCommand::execute(const HttpRequest& request) {
    std::cout << "Executing POST request for path: " << request.getPath() 
              << " with body: " << request.getBody() << std::endl;
    // Implement POST request logic
}

void CommandInvoker::registerCommand(HttpRequest::Method method, std::unique_ptr<Command> command) {
    commands[method] = std::move(command);
}

void CommandInvoker::executeCommand(const HttpRequest& request) {
    auto it = commands.find(request.getMethod());
    if (it != commands.end()) {
        it->second->execute(request);
    } else {
        std::cerr << "No command registered for this method" << std::endl;
    }
}