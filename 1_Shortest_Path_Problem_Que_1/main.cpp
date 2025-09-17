#include "Graph.hpp"
#include "Graph_logger.hpp"
#include <iostream>
#include <random> 

int main() 
{
    Logger logger("logs.txt");
    Graph g(10);
    g.Connected_Graph_Generator();
    
    g.loadGraphFromJSON("exercise_baseline.json"); 
    int nodes = g.getGraph().size();
    int edges = g.countEdges(); 
    logger.logGraphStats(nodes, edges);
    bool s = g.bfs_is_connected();
    const auto& graph = g.getGraph();
    std::string random_node, first_node, random_node_1;
    
    if (!graph.empty()) 
    {
        auto it = graph.begin();
        first_node = it->first;
        std::mt19937 gen(std::random_device{}());  // random number engine
        std::uniform_int_distribution<> distrib(0, graph.size() - 1);

        int rand_index = distrib(gen);

        auto it_r = graph.begin();
        std::advance(it_r, rand_index);

        random_node = it_r->first;
        rand_index = distrib(gen);

        auto it_s = graph.begin();
        std::advance(it_s, rand_index);

        random_node_1 = it_s->first;
    }

    std::cout<<first_node<<"\t"<<random_node<<"\t"<<random_node_1<<std::endl;
    bool from = false;
    
    // 1. Dijkstra from a source to all the other vertices
    logger.start("Dijkstra from source to all");
    g.dijkstra_Shortest_path(first_node, from);
    logger.stop("Dijkstra from source to all");

    // 2. Dijkstra between two specific nodes
    logger.start("Dijkstra between two nodes");
    g.dijkstra_SP_btw_two_nodes(first_node, random_node);
    logger.stop("Dijkstra between two nodes");

    // 3. All-pairs shortest paths (Floyd–Warshall)
    logger.start("All pairs shortest paths");
    g.all_pair_shortest_paths();
    logger.stop("All pairs shortest paths");

    // 4. Dijkstra from all nodes to a target
    logger.start("Dijkstra to target node");
    g.dijkstra_to_target_node(random_node_1, from);
    logger.stop("Dijkstra to target node");

    // Save logs to file
    logger.saveLog();

    return 0;
}
