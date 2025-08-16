#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

class Logger {
public:
    static Logger& getInstance();

    // Delete copy constructor and assignment operator to prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(const std::string& level, const std::string& message);

private:
    Logger(); // Private constructor to enforce singleton
    ~Logger(); // Private destructor

    std::ofstream log_file_;
    std::mutex log_mutex_;

    void init_log_file();
};

#endif // LOGGER_H
