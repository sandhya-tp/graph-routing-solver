#ifndef MCMINFLOW_SOLVER_H
#define MCMINFLOW_SOLVER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>

void LoadGraphFromJSON_with_demands(const std::string& filename,
    std::unordered_map<std::string, std::vector<std::tuple<std::string, int, int>>>& graph);

void printGraph(const std::unordered_map<std::string, std::vector<std::tuple<std::string, int, int>>>& graph);

#endif