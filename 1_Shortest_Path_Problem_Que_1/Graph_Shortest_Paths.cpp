#include "Graph.hpp"
#include <iostream>
#include <string>
#include <stack> 
#include <queue> 
#include <climits>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h" 
#include "rapidjson/prettywriter.h" 
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;


    
bool Graph::bfs_is_connected()
{
    if (graph_instance.empty()) return true;
    // Initialize a queue and a set to keep track of visited nodes
    std::queue<std::string> nodes;
    std::string startNode = graph_instance.begin()->first;
    std::unordered_map<std::string, bool> visited;
  
    nodes.push(startNode);
    std::string current_node;
    // While there are nodes in the queue to explore
    while (!nodes.empty())
    {
        current_node = nodes.front();
        nodes.pop();
        if (visited[current_node]) continue;
        visited[current_node]=true;
        //std::cout << "Visited: " << current_node << "\n";
        for (const auto & neighbor_pair : graph_instance[current_node])
        { 
            const std::string& neighbor = neighbor_pair.first;

            if (!visited[neighbor])
                nodes.push(neighbor);
    
        }
    }        
    int numberOfNodes = graph_instance.size();
    int noofvisitednodes=visited.size();
    //std::cout<<numberOfNodes<<"             "<<noofvisitednodes<<std::endl;
    if(noofvisitednodes==numberOfNodes)
        return true;
    else
        return false;
}


void Graph::dijkstra_Shortest_path(std::string start_node, bool to)
{   
    
    std::priority_queue<std::pair<int,std::string>,std::vector<std::pair<int,std::string>>, std::greater<>> distance;
    distance.emplace(0,start_node);
    std::unordered_map<std::string, int> dist;
    std::unordered_map<std::string,std::string> parent;
    parent[start_node]="";
    dist[start_node]=0;
    for(const auto& [node,neighbor]:graph_instance)
    {
        if(node!=start_node)
        {
            //distance.emplace(10000,node);
            dist[node]=INT_MAX;
        }
    }
    std::string current_node;
    //std::priority_queue<std::string, int> pq;
    std::unordered_set<std::string> visited;

    while(!distance.empty())
    {   
        auto [distan,current_node]=distance.top();
        distance.pop();
        if (visited.count(current_node)) continue;
        visited.insert(current_node);
    
        //visited.insert(current_node);

        for(const auto [neighbor,weight] :graph_instance[current_node])
        {
            //visited.push(neighbor);
            if(dist[neighbor]>dist[current_node]+weight)
                {
                    dist[neighbor]=dist[current_node]+weight;
                    distance.emplace(dist[current_node]+weight,neighbor);
                    parent[neighbor]=current_node;
                }
        } 
        
    }    
    std::vector<std::tuple<std::string, std::string, int, std::vector<std::string>>> paths;

    for (const auto& [node, distance] : dist) 
    {
        std::vector<std::string> path;
        if (distance != INT_MAX) 
        {
            std::string traverse = node;
            while (traverse != start_node) 
            {
                path.push_back(traverse);
                traverse = parent[traverse];
            }
            path.push_back(start_node);
            std::reverse(path.begin(), path.end());
        }

        paths.emplace_back(start_node, node, distance, path);
    }
    if(to)
    {
        writeShortestPathsToJSON("Output/dijkstra_paths_to.json", "shortest_paths", paths);
    }
    else
    {
        writeShortestPathsToJSON("Output/dijkstra_paths_from.json", "shortest_paths", paths);
    }
}


//Shortest path between two vertices using Dijkstra algo




void Graph::dijkstra_SP_btw_two_nodes(
  std::string start_node, 
  std::string target_node)
{

  if (graph_instance.find(target_node) == graph_instance.end()) {
    std::cout << "Target node does not exist in the graph.\n";
    return;
    }
std::priority_queue<std::pair<int,std::string>,std::vector<std::pair<int,std::string>>, std::greater<>> distance;
distance.emplace(0,start_node);
std::unordered_map<std::string, int> dist;
std::unordered_map<std::string,std::string> parent;
parent[start_node]="";
dist[start_node]=0;
for(const auto& [node,neighbor]:graph_instance)
{
if(node!=start_node)
    {
        //distance.emplace(10000,node);
        dist[node]=INT_MAX;
    }
}
std::string current_node;
//std::priority_queue<std::string, int> pq;
std::unordered_set<std::string> visited;

while(!distance.empty())
{   
    auto [distan,current_node]=distance.top();
    distance.pop();
    if (visited.count(current_node)) continue;
    visited.insert(current_node);
    if (current_node == target_node) break;
    //visited.insert(current_node);

        for(const auto [neighbor,weight] :graph_instance[current_node])
        {
            //visited.push(neighbor);
            if(dist[neighbor]>dist[current_node]+weight)
                {
                    dist[neighbor]=dist[current_node]+weight;
                    distance.emplace(dist[current_node]+weight,neighbor);
                    parent[neighbor]=current_node;
                }
        } 
        
}    
std::vector<std::string> path;
    if (dist[target_node] != INT_MAX) {
        std::string traverse = target_node;
        while (traverse != start_node) {
            path.push_back(traverse);
            traverse = parent[traverse];
        }
        path.push_back(start_node);
        std::reverse(path.begin(), path.end());
    }

    std::vector<std::tuple<std::string, std::string, int, std::vector<std::string>>> paths;
    paths.emplace_back(start_node,target_node,dist[target_node], path);
   
    writeShortestPathsToJSON("Output/dijkstra_path_between_two_nodes.json", "shortest_paths", paths);


    
}




//Floyd-Warshall Algorithm for all pairs shortest paths (assuming NO negative cycles)
void Graph::all_pair_shortest_paths() {
    const int INF = INT_MAX / 2;

    // Map node names to indices and create node list
    std::vector<std::string> nodes;
    std::unordered_map<std::string, int> node_to_index;
    int idx = 0;
    for (const auto& [node, _] : graph_instance) {
        node_to_index[node] = idx++;
        nodes.push_back(node);
    }
    int n = nodes.size();

    // Initialize distance and predecessor matrices
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    std::vector<std::vector<std::string>> pred(n, std::vector<std::string>(n, ""));

    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
        pred[i][i] = nodes[i];
    }

    // Fill initial distances from graph
    for (const auto& [u, neighbors] : graph_instance) {
        int i = node_to_index[u];
        for (const auto& [v, weight] : neighbors) {
            int j = node_to_index[v];
            dist[i][j] = weight;
            pred[i][j] = u;
        }
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pred[i][j] = pred[k][j];
                }
            }
        }
    }
std::vector<std::tuple<std::string, std::string, int, std::vector<std::string>>> all_paths;

for (const std::string& from : nodes) {
    for (const std::string& to : nodes) {
        int i = node_to_index[from];
        int j = node_to_index[to];
        int distance = dist[i][j];

        std::vector<std::string> path;
        if (distance < INT_MAX / 2) {
            std::string traverse = to;
            while (traverse != from) {
                int t_idx = node_to_index[traverse];
                traverse = pred[i][t_idx];
                path.push_back(traverse);
            }
            path.push_back(from);
            std::reverse(path.begin(), path.end());
        }

        all_paths.emplace_back(from, to, distance, path);
    }
}

writeShortestPathsToJSON("Output/all_pairs_output.json", "all_pairs_shortest_paths", all_paths);

}
// same function as single-source
void Graph::dijkstra_to_target_node(std::string target_node, bool to) {
to =true;
std::cout << "Computing shortest paths to " << target_node << "...\n";
dijkstra_Shortest_path(target_node, to); 
}