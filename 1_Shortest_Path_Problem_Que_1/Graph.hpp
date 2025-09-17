#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

class Graph {
public:
    Graph(int n);
    int countEdges() const;


    void Connected_Graph_Generator();
    void loadGraphFromJSON(const std::string& filename); 
    void writeToJSON(const std::string &filename, const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& graph_to_export)const; 
    bool bfs_is_connected();
    const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& getGraph() const;

    void dijkstra_Shortest_path(std::string start_node, bool);

    void dijkstra_SP_btw_two_nodes(std::string start_node, std::string target_node);
    void dijkstra_to_target_node(std::string target_node, bool to);

    void all_pair_shortest_paths();

    void logMetrics(const std::string &operation, int source = -1, int dest = -1) const;

private:
    void writeShortestPathsToJSON(
    const std::string& filename,
    const std::string& label,
    const std::vector<std::tuple<std::string, std::string, int, std::vector<std::string>>>& paths) const;

    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> graph_instance;
    
    int numVertices;
    std::vector<std::vector<std::pair<int, int>>> adjList;

};

#endif

