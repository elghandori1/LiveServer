#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int port = 8080; // Choose a port

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return 1;
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;
    std::cout << "Open your browser and visit: http://localhost:" << port << std::endl;

    while (true)
    {
        // Accept connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept failed");
            return 1;
        }

        char buffer[1024] = {0};
        recv(new_socket, buffer, 1024, 0); // Receive HTTP request

        // Extract requested file from HTTP GET request
        std::string request(buffer);
        std::string requested_file = "index.html"; // Default file

        size_t pos = request.find("GET ");
        if (pos != std::string::npos) {
            size_t start = pos + 4; // Move past "GET "
            size_t end = request.find(" ", start);
            std::string url = request.substr(start, end - start);

            if (url == "/") {
                requested_file = "index.html"; // Default to index page
            } else {
                requested_file = url.substr(1);
            }
        }

        // Open the requested HTML file
        std::ifstream html_file("./Public/" + requested_file);
        std::ostringstream response;

        if (html_file.is_open())
        {
            std::string html_content((std::istreambuf_iterator<char>(html_file)),
                                     std::istreambuf_iterator<char>());

            // Construct HTTP response
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: text/html\r\n";
            response << "Content-Length: " << html_content.length() << "\r\n";
            response << "\r\n";
            response << html_content;

            html_file.close();
        } else {
            // If file not found, return a 404 error page
            std::string not_found = "<html><body><h1>404 Not Found</h1></body></html>";
            response << "HTTP/1.1 404 Not Found\r\n";
            response << "Content-Type: text/html\r\n";
            response << "Content-Length: " << not_found.length() << "\r\n";
            response << "\r\n";
            response << not_found;
        }

        // Send response to client
        send(new_socket, response.str().c_str(), response.str().length(), 0);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
