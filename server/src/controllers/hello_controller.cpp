#include <string>
#include "../httprequest.h"

std::string getHello(const HttpRequest& request) {
    return "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, World!\n";
}
