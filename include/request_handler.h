#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "observer.h"
#include <string>
#include <sys/socket.h>

class RequestHandler : public Observer {
public:
    void update(int clientSocket) override;

private:
    void processRequest(int clientSocket);
    std::string parseHttpRequest(int clientSocket);
    void sendHttpResponse(int clientSocket, const std::string& response);
};

class RequestHandlerFactory {
public:
    static std::shared_ptr<RequestHandler> createHandler();
};

#endif // REQUEST_HANDLER_H