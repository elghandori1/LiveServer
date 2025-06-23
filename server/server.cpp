// server/server.cpp
#include "utils.hpp"
#include "signal.hpp"

#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    signal(SIGINT, signal_handler);
    int port = 8080;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return 1;
    }

    // Allow immediate reuse of address after Ctrl+C
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        return 1;
    }

    std::cout << "Server running at http://localhost:" << port << "\n";
    while (true) {
        socklen_t addrlen = sizeof(address);
        int client_fd = accept(server_fd, (sockaddr*)&address, &addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        char buffer[8192];
        int received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) {
            close(client_fd);
            continue;
        }
        
        buffer[received] = '\0';
        std::string request(buffer);
        std::istringstream req_stream(request);
        std::string method, path;
        req_stream >> method >> path;

        std::string status;
        std::string mime;
        std::string response_body;

        if(method =="GET"){
            if (path == "/"){
                path = "/index.html";
            }
            std::string full_path = "../public" + path;
            std::string file_content = read_file(full_path);

            if (!file_content.empty()) {
                status = "HTTP/1.1 200 OK\r\n";
                mime = get_mime_type(full_path);
                response_body = file_content;
            } else {
                status = "HTTP/1.1 404 Not Found\r\n";
                response_body = "<h1>404 Not Found</h1>";
                mime = "text/html";
            }

        }else if(method =="POST"){
            std::string headers_part = request.substr(0, request.find("\r\n\r\n"));
            std::string body = request.substr(request.find("\r\n\r\n") + 4);
        
            size_t content_length_pos = headers_part.find("Content-Length:");
            int content_length = 0;
            if (content_length_pos != std::string::npos) {
                std::istringstream cl_stream(headers_part.substr(content_length_pos));
                std::string discard;
                cl_stream >> discard >> content_length;
            }

            // Ensure we have the complete body
            if (body.size() < content_length) {
                char temp_buffer[8192];
                int remaining = content_length - body.size();
                while (remaining > 0) {
                int recvd = recv(client_fd, temp_buffer, std::min((int)sizeof(temp_buffer) - 1, remaining), 0);
                if (recvd <= 0) break;
                temp_buffer[recvd] = '\0';
                body += temp_buffer;
                remaining -= recvd;
                }
            }

            status = "HTTP/1.1 200 OK\r\n";
            mime = "text/plain";
            response_body = "Received POST data: " + body;
        }else{
            status = "HTTP/1.1 405 Method Not Allowed\r\n";
            response_body = "<h1>405 Method Not Allowed</h1>";
            mime = "text/html";
        }
        std::ostringstream response;
        response << status
                 << "Content-Type: " << mime << "\r\n"
                 << "Content-Length: " << response_body.size() << "\r\n"
                 << "Connection: close\r\n\r\n"
                 << response_body;

        std::string final_response = response.str();
        send(client_fd, final_response.c_str(), final_response.size(), 0);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}