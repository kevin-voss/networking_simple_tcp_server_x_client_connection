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

int main() {
    // Common variables for both requests
    int port = 8080;
    std::string server_address = "server"; // Use the server container name

    // === First Request: GET /hello ===
    int sock = 0;
    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(server_address.c_str(), std::to_string(port).c_str(), &hints, &res)) != 0) {
        log_message("ERROR", "getaddrinfo for /hello: " + std::string(gai_strerror(status)));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = res; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            log_message("ERROR", "client /hello: socket failed");
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            log_message("ERROR", "client /hello: connect failed");
            continue;
        }

        break; // if we get here, we successfully connected
    }

    if (p == NULL) {
        log_message("ERROR", "client /hello: failed to connect");
        return 2;
    }

    freeaddrinfo(res);
    log_message("INFO", "Connected to server for /hello request.");

    // Send GET /hello request
    std::string hello_message = "GET /hello\n";
    send(sock, hello_message.c_str(), hello_message.length(), 0);
    log_message("INFO", "Message sent: " + hello_message);

    // Receive response for /hello
    char buffer_hello[1024] = {0};
    read(sock, buffer_hello, 1024);
    log_message("INFO", "Server response for /hello: " + std::string(buffer_hello));

    // Close the first socket after the first request
    close(sock);
    log_message("INFO", "Closed first connection to server.");

    // === Second Request: GET /bye ===
    int sock_bye = 0;
    struct addrinfo hints_bye, *res_bye, *p_bye;
    int status_bye;

    memset(&hints_bye, 0, sizeof hints_bye);
    hints_bye.ai_family = AF_UNSPEC;
    hints_bye.ai_socktype = SOCK_STREAM;

    if ((status_bye = getaddrinfo(server_address.c_str(), std::to_string(port).c_str(), &hints_bye, &res_bye)) != 0) {
        log_message("ERROR", "getaddrinfo for /bye: " + std::string(gai_strerror(status_bye)));
        return 1;
    }

    for(p_bye = res_bye; p_bye != NULL; p_bye = p_bye->ai_next) {
        if ((sock_bye = socket(p_bye->ai_family, p_bye->ai_socktype, p_bye->ai_protocol)) == -1) {
            log_message("ERROR", "client /bye: socket failed");
            continue;
        }

        if (connect(sock_bye, p_bye->ai_addr, p_bye->ai_addrlen) == -1) {
            close(sock_bye);
            log_message("ERROR", "client /bye: connect failed");
            continue;
        }
        break;
    }

    if (p_bye == NULL) {
        log_message("ERROR", "client /bye: failed to connect");
        return 2;
    }
    freeaddrinfo(res_bye);
    log_message("INFO", "Re-connected to server for /bye request.");

    // Send GET /bye request on the new connection
    std::string bye_message = "GET /bye\n";
    send(sock_bye, bye_message.c_str(), bye_message.length(), 0);
    log_message("INFO", "Message sent: " + bye_message);

    // Receive response for /bye
    char buffer_bye[1024] = {0};
    read(sock_bye, buffer_bye, 1024);
    log_message("INFO", "Server response for /bye: " + std::string(buffer_bye));

    // Close the second socket
    close(sock_bye);
    log_message("INFO", "Closed second connection to server.");

    return 0;
}
