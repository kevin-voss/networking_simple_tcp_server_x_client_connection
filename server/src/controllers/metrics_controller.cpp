#include <string>
#include <map>
#include <sstream>
#include "../utils/httprequest.h"
#include "metrics_controller.h"

std::string getMetrics(const HttpRequest& request, int total_requests, const std::map<std::string, int>& endpoint_counts) {
    std::stringstream response_body;

    response_body << "# HELP http_requests_total Total number of HTTP requests.\n";
    response_body << "# TYPE http_requests_total counter\n";
    response_body << "http_requests_total " << total_requests << "\n\n";

    response_body << "# HELP http_requests_endpoint_total Total number of HTTP requests per endpoint.\n";
    response_body << "# TYPE http_requests_endpoint_total counter\n";
    for (const auto& pair : endpoint_counts) {
        std::string endpoint_name = pair.first;
        // Prometheus labels should be safe: replace non-alphanumeric with underscores
        for (char &c : endpoint_name) {
            if (!std::isalnum(c)) {
                c = '_';
            }
        }
        response_body << "http_requests_endpoint_total{endpoint=\"" << endpoint_name << "\"} " << pair.second << "\n";
    }

    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain; version=0.0.4; charset=utf-8\n\n" + response_body.str();

    return response;
}
