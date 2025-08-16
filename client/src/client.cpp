#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <netdb.h>   // For getaddrinfo
#include <cstring>   // For memset
#include <chrono>    // For std::chrono
#include <ctime>     // For std::time_t, std::localtime
#include <iomanip>   // For std::put_time

// Basic logging function
void log_message(const std::string& level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::cout << "[" << std::put_time(std::localtime(&current_time), "%Y-%m-%d %H:%M:%S")
              << "] [" << level << "] " << message << std::endl;
}

// Function to send a GET request and receive a response
std::string fetch(const std::string& path, const std::string& server_address, int port) {
    int sock = 0;
    struct addrinfo hints, *res, *p;
    int status;
    char buffer[1024] = {0};

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(server_address.c_str(), std::to_string(port).c_str(), &hints, &res)) != 0) {
        log_message("ERROR", "getaddrinfo for " + path + ": " + std::string(gai_strerror(status)));
        return "";
    }

    for(p = res; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            log_message("ERROR", "client " + path + ": socket failed");
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            log_message("ERROR", "client " + path + ": connect failed");
            continue;
        }
        break;
    }

    if (p == NULL) {
        log_message("ERROR", "client " + path + ": failed to connect");
        return "";
    }

    freeaddrinfo(res);
    log_message("INFO", "Connected to server for " + path + " request.");

    std::string message = "GET " + path + "\n";
    send(sock, message.c_str(), message.length(), 0);
    log_message("INFO", "Message sent: " + message);

    read(sock, buffer, 1024);
    std::string response(buffer);
    log_message("INFO", "Server response for " + path + ": " + response);

    close(sock);
    log_message("INFO", "Closed connection to server for " + path + ".");

    return response;
}

int main() {
    // Common variables for both requests
    int port = 8080;
    std::string server_address = "server"; // Use the server container name

    // === First Request: GET /hello ===
    fetch("/hello", server_address, port);

    // === Second Request: GET /bye ===
    fetch("/bye", server_address, port);

    // === Third Request: GET /health ===
    fetch("/health", server_address, port);

    // === Fourth Request: GET /metrics ===
    fetch("/metrics", server_address, port);

    return 0;
}
