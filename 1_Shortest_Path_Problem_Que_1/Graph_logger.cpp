#include "Graph_logger.hpp"
#include <iostream>
#include <sstream>

Logger::Logger(const std::string &filename) : logFilename(filename) {}

void Logger::start(const std::string &operation) {
    currentOp = operation;
    callCounter[operation]++;
    startTime = std::chrono::high_resolution_clock::now();
}

void Logger::stop(const std::string &operation) {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::ostringstream oss;
    oss << "[LOG] Operation: " << operation
        << " | Time: " << duration << " ms"
        << " | Call Count: " << callCounter[operation] << "\n";
    logBuffer += oss.str();
    std::cout << oss.str();
}

void Logger::logGraphStats(int nodes, int edges) {
    std::ostringstream oss;
    oss << "[GRAPH] Nodes: " << nodes << " | Edges: " << edges << "\n";
    logBuffer += oss.str();
    std::cout << oss.str();
}

void Logger::logPathInfo(int source, int dest, int length, int hops) {
    std::ostringstream oss;
    oss << "[PATH] From: " << source << " To: " << dest
        << " | Distance: " << length << " | Hops: " << hops << "\n";
    logBuffer += oss.str();
    std::cout << oss.str();
}

void Logger::saveLog() {
    if (!logFilename.empty()) {
        std::ofstream out(logFilename, std::ios::app);
        out << logBuffer;
        out.close();
    }
}

