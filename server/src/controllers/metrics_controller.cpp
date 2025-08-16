#include <string>

std::string getMetrics() {
    return "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nMetrics: CPU=50%, RAM=70%\n";
}
