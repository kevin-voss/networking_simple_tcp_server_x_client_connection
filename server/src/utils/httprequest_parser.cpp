#include "httprequest_parser.h"
#include <sstream>
#include <algorithm>

HttpRequest HttpRequestParser::parse(const std::string& request_string) {
    HttpRequest request;
    std::istringstream iss(request_string);
    std::string line;
    
    // Parse request line
    std::getline(iss, line);
    parse_request_line(line, request);

    // Parse headers
    while (std::getline(iss, line) && line != "\r") { // Headers end with an empty line
        if (line.back() == '\r') {
            line.pop_back(); // Remove carriage return
        }
        size_t colon_pos = line.find(": ");
        if (colon_pos != std::string::npos) {
            std::string name = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 2);
            request.headers[name] = value;
        }
    }

    // Parse body
    std::string body_line;
    while (std::getline(iss, body_line)) {
        request.body += body_line;
        if (body_line.back() == '\r') {
            request.body.pop_back();
        }
        if (!iss.eof()) {
            request.body += "\n";
        }
    }

    return request;
}

std::vector<std::string> HttpRequestParser::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void HttpRequestParser::parse_request_line(const std::string& line, HttpRequest& request) {
    std::vector<std::string> parts = split(line, ' ');
    if (parts.size() >= 2) {
        request.method = parts[0];
        // Remove trailing \r from path if present
        if (!parts[1].empty() && parts[1].back() == '\r') {
            parts[1].pop_back();
        }
        
        size_t query_start = parts[1].find('?');
        if (query_start != std::string::npos) {
            request.path = parts[1].substr(0, query_start);
            std::string query_string = parts[1].substr(query_start + 1);
            parse_query_params(query_string, request);
        } else {
            request.path = parts[1];
        }
    }
}

void HttpRequestParser::parse_headers(const std::vector<std::string>& lines, HttpRequest& request) {
    // This function is now mostly handled directly in the parse method, 
    // but keeping it for conceptual separation if needed later.
}

void HttpRequestParser::parse_query_params(const std::string& query_string, HttpRequest& request) {
    std::vector<std::string> params = split(query_string, '&');
    for (const std::string& param : params) {
        size_t eq_pos = param.find('=');
        if (eq_pos != std::string::npos) {
            std::string name = param.substr(0, eq_pos);
            std::string value = param.substr(eq_pos + 1);
            request.query_params[name] = value;
        }
    }
}
