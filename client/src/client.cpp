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

    // === Third Request: GET /health ===
    int sock_health = 0;
    struct addrinfo hints_health, *res_health, *p_health;
    int status_health;

    memset(&hints_health, 0, sizeof hints_health);
    hints_health.ai_family = AF_UNSPEC;
    hints_health.ai_socktype = SOCK_STREAM;

    if ((status_health = getaddrinfo(server_address.c_str(), std::to_string(port).c_str(), &hints_health, &res_health)) != 0) {
        log_message("ERROR", "getaddrinfo for /health: " + std::string(gai_strerror(status_health)));
        return 1;
    }

    for(p_health = res_health; p_health != NULL; p_health = p_health->ai_next) {
        if ((sock_health = socket(p_health->ai_family, p_health->ai_socktype, p_health->ai_protocol)) == -1) {
            log_message("ERROR", "client /health: socket failed");
            continue;
        }

        if (connect(sock_health, p_health->ai_addr, p_health->ai_addrlen) == -1) {
            close(sock_health);
            log_message("ERROR", "client /health: connect failed");
            continue;
        }
        break;
    }

    if (p_health == NULL) {
        log_message("ERROR", "client /health: failed to connect");
        return 2;
    }
    freeaddrinfo(res_health);
    log_message("INFO", "Re-connected to server for /health request.");

    // Send GET /health request on the new connection
    std::string health_message = "GET /health\n";
    send(sock_health, health_message.c_str(), health_message.length(), 0);
    log_message("INFO", "Message sent: " + health_message);

    // Receive response for /health
    char buffer_health[1024] = {0};
    read(sock_health, buffer_health, 1024);
    log_message("INFO", "Server response for /health: " + std::string(buffer_health));

    // Close the third socket
    close(sock_health);
    log_message("INFO", "Closed third connection to server.");

    // === Fourth Request: GET /metrics ===
    int sock_metrics = 0;
    struct addrinfo hints_metrics, *res_metrics, *p_metrics;
    int status_metrics;

    memset(&hints_metrics, 0, sizeof hints_metrics);
    hints_metrics.ai_family = AF_UNSPEC;
    hints_metrics.ai_socktype = SOCK_STREAM;

    if ((status_metrics = getaddrinfo(server_address.c_str(), std::to_string(port).c_str(), &hints_metrics, &res_metrics)) != 0) {
        log_message("ERROR", "getaddrinfo for /metrics: " + std::string(gai_strerror(status_metrics)));
        return 1;
    }

    for(p_metrics = res_metrics; p_metrics != NULL; p_metrics = p_metrics->ai_next) {
        if ((sock_metrics = socket(p_metrics->ai_family, p_metrics->ai_socktype, p_metrics->ai_protocol)) == -1) {
            log_message("ERROR", "client /metrics: socket failed");
            continue;
        }

        if (connect(sock_metrics, p_metrics->ai_addr, p_metrics->ai_addrlen) == -1) {
            close(sock_metrics);
            log_message("ERROR", "client /metrics: connect failed");
            continue;
        }
        break;
    }

    if (p_metrics == NULL) {
        log_message("ERROR", "client /metrics: failed to connect");
        return 2;
    }
    freeaddrinfo(res_metrics);
    log_message("INFO", "Re-connected to server for /metrics request.");

    // Send GET /metrics request on the new connection
    std::string metrics_message = "GET /metrics\n";
    send(sock_metrics, metrics_message.c_str(), metrics_message.length(), 0);
    log_message("INFO", "Message sent: " + metrics_message);

    // Receive response for /metrics
    char buffer_metrics[1024] = {0};
    read(sock_metrics, buffer_metrics, 1024);
    log_message("INFO", "Server response for /metrics: " + std::string(buffer_metrics));

    // Close the fourth socket
    close(sock_metrics);
    log_message("INFO", "Closed fourth connection to server.");

    return 0;
}
