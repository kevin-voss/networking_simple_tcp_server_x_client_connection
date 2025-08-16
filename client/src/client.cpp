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
#include <thread>    // Required for std::thread
#include <mutex>     // Required for std::mutex

// Mutex for thread-safe logging
std::mutex log_mutex;

// Basic logging function
void log_message(const std::string& level, const std::string& message) {
    std::lock_guard<std::mutex> guard(log_mutex);
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::cout << "[" << std::put_time(std::localtime(&current_time), "%Y-%m-%d %H:%M:%S")
              << "] [" << level << "] " << message << std::endl;
}

// Function to send a GET request and receive a response
std::string fetch(const std::string& path, const std::string& server_address, int port) {
    auto start_time = std::chrono::high_resolution_clock::now();

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

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    log_message("INFO", "Request to " + path + " took " + std::to_string(duration.count()) + " ms.");

    return response;
}

// Helper function to send a request in a separate thread
void send_request(const std::string& path, const std::string& server_address, int port) {
    log_message("INFO", "----------------------------------------");
    log_message("INFO", "Starting request for: " + path);
    fetch(path, server_address, port);
    log_message("INFO", "Finished request for: " + path);
    log_message("INFO", "----------------------------------------");
}

int main() {
    // Common variables for all requests
    int port = 8080;
    std::string server_address = "server"; // Use the server container name

    log_message("INFO", "Launching client threads...");

    // Create threads for each request
    std::thread thread1(send_request, "/hello?block=5", server_address, port);
    std::thread thread2(send_request, "/bye", server_address, port);
    std::thread thread3(send_request, "/health", server_address, port);
    std::thread thread4(send_request, "/metrics", server_address, port);

    // Join threads to wait for their completion
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    log_message("INFO", "All client requests sent and responses received.");

    std::string input_path;
    while (true) {
        std::cout << "Enter request path (e.g., /hello or /metrics?block=3), or 'exit' to quit: " << std::endl;
        std::getline(std::cin, input_path);

        if (input_path == "exit") {
            log_message("INFO", "Exiting client.");
            break;
        }

        if (input_path.empty()) {
            log_message("WARN", "Empty input. Please enter a valid path.");
            continue;
        }

        // Create a new thread to send the request
        std::thread(send_request, input_path, server_address, port).detach();
    }

    return 0;
}
