#include "logger.h"
#include <sstream> // Required for std::stringstream

Logger& Logger::getInstance() {
    static Logger instance; // Guaranteed to be destroyed, instantiated on first use.
    return instance;
}

Logger::Logger() {
    init_log_file();
}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::init_log_file() {
    // You can make this configurable or based on environment variables
    std::string log_file_path = "/var/log/server/server.log"; 
    log_file_.open(log_file_path, std::ios_base::app); // Open in append mode

    if (!log_file_.is_open()) {
        std::cerr << "ERROR: Could not open log file: " << log_file_path << std::endl;
    }
}

void Logger::log(const std::string& level, const std::string& message) {
    std::lock_guard<std::mutex> guard(log_mutex_);

    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream log_stream;
    log_stream << "[" << std::put_time(std::localtime(&current_time), "%Y-%m-%d %H:%M:%S")
               << "] [" << level << "] " << message << std::endl;

    // Write to console
    std::cout << log_stream.str();

    // Write to file
    if (log_file_.is_open()) {
        log_file_ << log_stream.str();
    }
}
