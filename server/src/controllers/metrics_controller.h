#ifndef METRICS_CONTROLLER_H
#define METRICS_CONTROLLER_H

#include <string>
#include "../utils/httprequest.h"

std::string getMetrics(const HttpRequest& request);

#endif // METRICS_CONTROLLER_H
