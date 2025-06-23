#include "methods.hpp"
#include "./utils/utils.hpp"
#include <sstream>
#include <algorithm>
#include <unistd.h>

std::string RequestHandler::handle_request(const std::string& request, int client_fd) {
    std::istringstream req_stream(request);
    std::string method, path;
    req_stream >> method >> path;

    std::string headers = request.substr(0, request.find("\r\n\r\n"));
    std::string body = request.substr(request.find("\r\n\r\n") + 4);

    std::string status, mime, response_body;

    if (method == "GET") {
        return handle_get(path);
    } else if (method == "POST") {
        return handle_post(headers, body, client_fd);
    } else {
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

    return response.str();
}

std::string RequestHandler::handle_get(const std::string& path) {
    std::string real_path = path == "/" ? "/index.html" : path;
    std::string full_path = "./public" + real_path;
    std::string file_content;
    
    // Check if it's a PHP file
    if (real_path.substr(real_path.find_last_of(".") + 1) == "php") {
        // Execute PHP file and get output
        FILE* phphp_outputp = popen(("php " + full_path).c_str(), "r");
        if (php_output) {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), php_output) != NULL) {
                file_content += buffer;
            }
            pclose(php_output);
        }
    } else {
        file_content = read_file(full_path);
    }

    std::string status, mime, response_body;

    if (!file_content.empty()) {
        status = "HTTP/1.1 200 OK\r\n";
        mime = get_mime_type(full_path);
        if (real_path.substr(real_path.find_last_of(".") + 1) == "php") {
            mime = "text/html";
        }
        response_body = file_content;
    } else {
        status = "HTTP/1.1 404 Not Found\r\n";
        response_body = "<h1>404 Not Found</h1>";
        mime = "text/html";
    }

    std::ostringstream response;
    response << status
             << "Content-Type: " << mime << "\r\n"
             << "Content-Length: " << response_body.size() << "\r\n"
             << "Connection: close\r\n\r\n"
             << response_body;

    return response.str();
}

std::string RequestHandler::handle_post(const std::string& headers, std::string& body, int client_fd) {
    size_t content_length_pos = headers.find("Content-Length:");
    int content_length = 0;
    if (content_length_pos != std::string::npos) {
        std::istringstream cl_stream(headers.substr(content_length_pos));
        std::string discard;
        cl_stream >> discard >> content_length;
    }

    if ((int)body.size() < content_length) {
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

    std::string status = "HTTP/1.1 200 OK\r\n";
    std::string mime = "text/plain";
    std::string response_body = "Received POST data:\n" + body;

    std::ostringstream response;
    response << status
             << "Content-Type: " << mime << "\r\n"
             << "Content-Length: " << response_body.size() << "\r\n"
             << "Connection: close\r\n\r\n"
             << response_body;

    return response.str();
}
