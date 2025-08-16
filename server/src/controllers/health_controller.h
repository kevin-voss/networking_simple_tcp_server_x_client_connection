#ifndef HEALTH_CONTROLLER_H
#define HEALTH_CONTROLLER_H

#include <string>
#include "../httprequest.h"

std::string getHealthStatus(const HttpRequest& request);

#endif // HEALTH_CONTROLLER_H
