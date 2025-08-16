#include <string>

std::string getHealthStatus() {
    return "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nServer is healthy!\n";
}
