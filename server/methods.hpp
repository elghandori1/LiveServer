#pragma once
#include <string>
#include <sys/socket.h> 

class RequestHandler {
public:
    std::string handle_request(const std::string& request, int client_fd);
private:
    std::string handle_get(const std::string& path);
    std::string handle_post(const std::string& headers, std::string& body, int client_fd);
};
