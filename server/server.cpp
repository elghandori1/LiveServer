#include <iostream>
#include <exception>
#include "HttpServer.hpp"

int main() {
    try {
        HttpServer server(8080);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}