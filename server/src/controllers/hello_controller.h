#ifndef HELLO_CONTROLLER_H
#define HELLO_CONTROLLER_H

#include <string>
#include "../utils/httprequest.h"

std::string getHello(const HttpRequest& request);

#endif // HELLO_CONTROLLER_H
