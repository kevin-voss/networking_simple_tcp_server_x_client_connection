#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

int main() {
    // 1. Create a socket
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind the socket to an IP address and port
    sockaddr_in address;
    int port = 8080;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 3) < 0) { // 3 is the maximum length of the queue of pending connections
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true) { // Main server loop to continuously accept connections
        // 4. Accept a client connection
        int client_socket;
        socklen_t addrlen = sizeof(address);
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            continue; // Continue listening for other connections
        }
        std::cout << "Client connected!" << std::endl;

        // 5. Receive data from the client
        char buffer[1024] = {0};
        long valread = read(client_socket, buffer, 1024);
        std::string client_message(buffer); // Convert char array to std::string

        std::cout << "Client message: " << client_message << std::endl;

        // 6. Send a response back to the client based on the message
        std::string response;
        if (client_message.find("GET /hello") != std::string::npos) {
            response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, World!\n";
        } else if (client_message.find("GET /bye") != std::string::npos) {
            response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nGoodbye!\n";
        } else {
            response = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n\nUnknown endpoint\n";
        }

        send(client_socket, response.c_str(), response.length(), 0);
        std::cout << "Response sent to client." << std::endl;

        // 7. Close the client socket (connection to this specific client)
        close(client_socket);
        std::cout << "Client disconnected." << std::endl;
    }

    // The server_fd will typically be closed only when the server application is shut down.
    close(server_fd);

    return 0;
}
