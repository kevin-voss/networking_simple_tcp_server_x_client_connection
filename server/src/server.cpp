#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <chrono>   // For std::chrono
#include <ctime>    // For std::time_t, std::localtime
#include <iomanip>  // For std::put_time
#include <arpa/inet.h> // Required for inet_ntoa

// Basic logging function
void log_message(const std::string& level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::cout << "[" << std::put_time(std::localtime(&current_time), "%Y-%m-%d %H:%M:%S")
              << "] [" << level << "] " << message << std::endl;
}

int main() {
    // 1. Create a socket
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log_message("ERROR", "socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind the socket to an IP address and port
    sockaddr_in address;
    int port = 8080;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        log_message("ERROR", "bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 3) < 0) { // 3 is the maximum length of the queue of pending connections
        log_message("ERROR", "listen failed");
        exit(EXIT_FAILURE);
    }

    log_message("INFO", "Server listening on port " + std::to_string(port));

    while (true) { // Main server loop to continuously accept connections
        // 4. Accept a client connection
        int client_socket;
        socklen_t addrlen = sizeof(address);
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            log_message("ERROR", "accept failed");
            continue; // Continue listening for other connections
        }

        // Get client IP and port using getpeername
        struct sockaddr_in client_address;
        socklen_t client_addrlen = sizeof(client_address);
        if (getpeername(client_socket, (struct sockaddr *)&client_address, &client_addrlen) == 0) {
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
            int client_port = ntohs(client_address.sin_port);
            log_message("INFO", "Client connected from " + std::string(client_ip) + ":" + std::to_string(client_port));
        } else {
            log_message("ERROR", "getpeername failed");
            log_message("INFO", "Client connected!"); // Fallback if getpeername fails
        }

        // 5. Receive data from the client
        char buffer[1024] = {0};
        long valread = read(client_socket, buffer, 1024);
        std::string client_message(buffer); // Convert char array to std::string

        log_message("INFO", "Client message: " + client_message);

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
        log_message("INFO", "Response sent to client.");

        // 7. Close the client socket (connection to this specific client)
        close(client_socket);
        log_message("INFO", "Client disconnected.");
    }

    // The server_fd will typically be closed only when the server application is shut down.
    close(server_fd);

    return 0;
}
