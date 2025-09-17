#ifndef Graph_LOGGER_HPP
#define Graph_LOGGER_HPP

#include <string>
#include <chrono>
#include <fstream>
#include <unordered_map>

class Logger {
public:
    Logger(const std::string &filename = "");

    void start(const std::string &operation);
    void stop(const std::string &operation);
    void logGraphStats(int nodes, int edges);
    void logPathInfo(int source, int dest, int length, int hops);
    void saveLog();

private:
    std::string logFilename;
    std::unordered_map<std::string, int> callCounter;
    std::chrono::high_resolution_clock::time_point startTime;
    std::string currentOp;
    std::string logBuffer;
};

#endif

