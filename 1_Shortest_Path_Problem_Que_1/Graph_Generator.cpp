#include "Graph.hpp"
#include <random>
#include <set>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <fstream>


#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h" 
#include "rapidjson/prettywriter.h" 
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;

int Graph::countEdges() const {
    int edge_count = 0;
    for (const auto& [node, neighbors] : graph_instance) {
        edge_count += neighbors.size();
    }
    return edge_count / 2; // Since it's undirected
}


Graph::Graph(int n) : numVertices(n) 
{
    adjList.resize(n);  // Initialize adjacency list for each vertex
}

const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& Graph::getGraph() const 
{
    return graph_instance;
}


void Graph::Connected_Graph_Generator()
{
    std::random_device rd; 
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(5, 100);

    int n = distrib(gen); 
    int max=floor(n*(n-1)/2);
    std::uniform_int_distribution<> distrib1(n-1, max);

    
    int m= distrib1(gen);
    std::string nodes[n];
    int j=0;
    int k=0;
    for (int i=0;i<int(n/26)+1;i++)
    {   
        for (int j=0;j<26;j++)
        {
            if(k>n-1) continue;
            nodes[k]="";
            nodes[k]=nodes[k]+char(97+i);//+char(97+i);//char(97+i);
            nodes[k]=nodes[k]+char(97+j);   
            //cout<<nodes[k]<<endl;
            k++;
        }
    }
    
    /*std::cout<<"List of nodes"<<std::endl;
    for (int i=0;i<n;i++)
    {
        std::cout<<nodes[i]<<"\t";
    }
    std::cout<<endl;
    */
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> graph;
    std::uniform_int_distribution<> distrib2(1, 100);
    
    std::uniform_int_distribution<> distrib_span_tree(1, 5);
    //int h= distrib_span_tree(gen);
    int h=5;
    switch(h)
    {
        case 1:
        //Create a spanning tree (star) on the set of nodes
    
            for (int i = 0; i < n-1; i++) 
            {
                int weight  = distrib2(gen);
                graph[nodes[0]].emplace_back(nodes[i+1], weight);
                graph[nodes[i+1]].emplace_back(nodes[0], weight);
                //cout<<"node: "<<nodes[i+1]<<"\t neighbor: "<<nodes[0]<<"\t weight: "<<weight<<endl;        
            }

        break;

    
        case 2:
            //Crete a spanning tree (path) on n vertices

            for (int i = 0; i < n-1; i++) 
            {
                int weight  = distrib2(gen);
                graph[nodes[i]].emplace_back(nodes[i+1], weight);
                graph[nodes[i+1]].emplace_back(nodes[i], weight);
                //cout<<"node: "<<nodes[i+1]<<"\t neighbor: "<<nodes[0]<<"\t weight: "<<weight<<endl;        
            }
        break;


        default:

            std::vector<std::string> visited;
            std::vector<std::string> unvisited;
            for (int i = 0; i < n; i++) 
            {
                graph[nodes[i]] = {};
            }

            for(const auto&[node, nbr]:graph)
                unvisited.push_back(node);
            std::uniform_int_distribution<> distrib_conn_graph(0, n-1);
            //int h= distrib_span_tree(gen);
            std::string start_node=nodes[distrib_conn_graph(gen)];
            visited.push_back(start_node);
            int k=0;
            while(visited.size()<n) 
            {
                if(visited.empty()||unvisited.empty()) break;
                int weight  = distrib2(gen);
                std::string first_node=visited[distrib_conn_graph(gen)%visited.size()];
                std::string second_node=unvisited[distrib_conn_graph(gen)%unvisited.size()];
                graph[first_node].emplace_back(second_node, weight);
                graph[second_node].emplace_back(first_node, weight);
                visited.push_back(second_node);
                //unvisited.erase(second_node);
                auto it = std::find(unvisited.begin(), unvisited.end(), second_node);
                if (it != unvisited.end()) 
                {
                    unvisited.erase(it);
                }
        
            }

    }



    //Adding remaining m-(n-1) edges
    k=0;
    std::uniform_int_distribution<> distribe1(0, n-1);
    while(k<m-n+1)
    {   
        int token=1;
        std::string first_node=nodes[distribe1(gen)];
        std::string second_node=nodes[distribe1(gen)];
        if (first_node == second_node) continue;
        for(const auto &[neighbor,weight]:graph[first_node])
        {  
                 
            if(neighbor==second_node)  
                token=0;
        }
        if(token==1)
        {   
            int weight  = distrib2(gen);
            graph[first_node].emplace_back(second_node, weight);
            graph[second_node].emplace_back(first_node, weight);
            k++;
        }
    }

    //Writing onto a JSON file

    Document d;
    d.SetObject();
        
    // Get the allocator
        
    Document::AllocatorType& allocator = d.GetAllocator();
    Value edge_array(kArrayType);
    for(const auto &[node,nbr]: graph)
    {
        for(const auto &[nbrs,weight]: nbr)
        {   
            Value edge(kObjectType);
            edge.AddMember("from", Value().SetString(node.c_str(), allocator), allocator);
            edge.AddMember("to", Value().SetString(nbrs.c_str(), allocator), allocator);
            edge.AddMember("transit_time", weight, allocator);
            edge_array.PushBack(edge, allocator);   
        }
        
    }
    d.AddMember("edges", edge_array, allocator); 
        
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    writer.SetIndent(' ', 4); 
    d.Accept(writer);
    std::ofstream ofs("Output/Graph_generator_output.json");
    if (ofs) 
    {
        ofs << buffer.GetString();
        std::cout << "JSON written successfully to output.json\n";
    } 
    else 
    {
        std::cerr << "Error opening output.json for writing\n";
    }
    ofs.close();
    
}

