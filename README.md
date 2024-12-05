Multithreaded Web Server with OOP Design Patterns
Project Overview
This project demonstrates a multithreaded web server implemented using Object-Oriented Programming (OOP) design patterns in C++.
Design Patterns Used

Singleton Pattern: Ensures only one server instance exists
Command Pattern: Handles different HTTP request types
Observer Pattern: Manages request notifications
Multithreading: Handles concurrent client connections

Building the Project
bashCopymake
Running the Server
bashCopy./build/webserver
Key Components

Server: Singleton server management
CommandInvoker: HTTP method request routing
ThreadPool: Concurrent request handling
Observer: Request notification mechanism

Dependencies

C++17 compatible compiler
POSIX-compliant system (Linux/macOS)