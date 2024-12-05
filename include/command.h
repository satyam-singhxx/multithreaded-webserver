#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>
#include <unordered_map>

class HttpRequest {
public:
    enum class Method { GET, POST, PUT, DELETE };
    
    HttpRequest(Method method, const std::string& path, const std::string& body = "")
        : method(method), path(path), body(body) {}

    Method getMethod() const { return method; }
    std::string getPath() const { return path; }
    std::string getBody() const { return body; }

private:
    Method method;
    std::string path;
    std::string body;
};

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const HttpRequest& request) = 0;
};

class GetCommand : public Command {
public:
    void execute(const HttpRequest& request) override;
};

class PostCommand : public Command {
public:
    void execute(const HttpRequest& request) override;
};

class CommandInvoker {
public:
    void registerCommand(HttpRequest::Method method, std::unique_ptr<Command> command);
    void executeCommand(const HttpRequest& request);

private:
    std::unordered_map<HttpRequest::Method, std::unique_ptr<Command>> commands;
};

#endif // COMMAND_H