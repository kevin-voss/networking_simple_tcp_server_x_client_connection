#ifndef METRICS_CONTROLLER_H
#define METRICS_CONTROLLER_H

#include <string>
#include "../utils/httprequest.h"

std::string getMetrics(const HttpRequest& request, int total_requests, const std::map<std::string, int>& endpoint_counts);

#endif // METRICS_CONTROLLER_H
