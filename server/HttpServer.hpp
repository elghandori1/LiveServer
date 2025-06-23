#pragma once
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


class HttpServer {
public:
    HttpServer(int port);
    ~HttpServer();
    void run();

private:
    int server_fd;
    int port;
    void handle_client(int client_fd);
};
