#include "MCMinFlow_solver.h"
#include <unordered_map>
#include <string>
#include <tuple>
#include <vector>


int main()
{   
    std::unordered_map<std::string, std::vector<std::tuple<std::string, int, int>>> graph; 

    LoadGraphFromJSON_with_demands("exercise_bonus.json", graph) ;
    return 0;

}