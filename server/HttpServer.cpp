#include "HttpServer.hpp"
#include "methods.hpp"
#include "signal.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

HttpServer::HttpServer(int port) : port(port) {
    signal(SIGINT, signal_handler);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) throw std::runtime_error("Socket creation failed");

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt failed");

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("bind failed");

    if (listen(server_fd, 10) < 0)
        throw std::runtime_error("listen failed");
}

void HttpServer::run() {
    std::cout << "Server running at http://localhost:" << port << "\n";
    sockaddr_in client_addr{};
    socklen_t addrlen = sizeof(client_addr);

    while (true) {
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }
        handle_client(client_fd);
        close(client_fd);
    }
}

void HttpServer::handle_client(int client_fd) {
    char buffer[8192];
    int received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0) return;

    buffer[received] = '\0';
    std::string request(buffer);

    RequestHandler handler;
    std::string response = handler.handle_request(request, client_fd);

    send(client_fd, response.c_str(), response.size(), 0);
}


HttpServer::~HttpServer() {
    close(server_fd);
}
