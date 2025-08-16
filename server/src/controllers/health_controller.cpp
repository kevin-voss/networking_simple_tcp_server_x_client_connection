#include <string>
#include "../utils/httprequest.h"

std::string getHealthStatus(const HttpRequest& request) {
    return "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nServer is healthy!\n";
}
