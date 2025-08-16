#ifndef HTTP_REQUEST_PARSER_H
#define HTTP_REQUEST_PARSER_H

#include "httprequest.h"
#include <string>
#include <vector>

class HttpRequestParser {
public:
    static HttpRequest parse(const std::string& request_string);

private:
    static std::vector<std::string> split(const std::string& s, char delimiter);
    static void parse_request_line(const std::string& line, HttpRequest& request);
    static void parse_headers(const std::vector<std::string>& lines, HttpRequest& request);
    static void parse_query_params(const std::string& path, HttpRequest& request);
};

#endif // HTTP_REQUEST_PARSER_H
