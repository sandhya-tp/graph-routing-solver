#include "Graph.hpp"
#include <fstream>
#include <random>
#include <set>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <climits>


#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h" 
#include "rapidjson/prettywriter.h" 
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;


void Graph::loadGraphFromJSON(const std::string& filename) 
{
    //std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> graph;
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        std::cerr << "Error opening file\n";
        return;
    }

    char buffer[65536];
    FileReadStream is(fp, buffer, sizeof(buffer));

    Document d;
    d.ParseStream(is);
    fclose(fp);
    
    

    if (!d.HasMember("edges") || !d["edges"].IsArray()) {
        std::cerr << "Invalid JSON format\n";
        return;
    }
     
    const Value& edges = d["edges"];
    for (SizeType i = 0; i < edges.Size(); i++) {
        const Value& edge = edges[i];

        if (!edge.HasMember("from") || !edge.HasMember("to")) continue;
        if (!edge["from"].IsString() || !edge["to"].IsString()) continue;

        std::string u = edge["from"].GetString();
        std::string v = edge["to"].GetString();
        int weight = edge["transit_time"].GetInt();

        if (u.empty() || v.empty()) {
            //std::cerr << "Skipping edge with empty node names at index " << i << "\n";
            continue;
        }
       // graph[u].push_back(v);
       // graph[v].push_back(u);  // if undirected
        graph_instance[u].emplace_back(v, weight);
        graph_instance[v].emplace_back(u, weight);
    }
    //return graph;
}

void Graph::writeToJSON(const std::string &filename,
    const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& graph_to_export)const 
{

    Document d;
    d.SetObject();
        
    // Get the allocator
        
    Document::AllocatorType& allocator = d.GetAllocator();
    Value edge_array(kArrayType);
     for (const auto& [u, neighbors] : graph_to_export) 
     {
        for (const auto& [v, weight] : neighbors) 
        {
            Value edge(kObjectType);
            edge.AddMember("from", Value().SetString(u.c_str(), allocator), allocator);
            edge.AddMember("to", Value().SetString(v.c_str(), allocator), allocator);
            edge.AddMember("transit_time", weight, allocator);
            edge_array.PushBack(edge, allocator);   
        }
        
    }
    d.AddMember("edges", edge_array, allocator); 
        
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    writer.SetIndent(' ', 4); 
    d.Accept(writer);
    std::ofstream ofs(filename);
    if (ofs) 
    {
        ofs << buffer.GetString();
        std::cout << "JSON written successfully to "<<filename<<"\n";
    } 
    else 
    {
        std::cerr << "Error opening "<<filename<<" for writing\n";
    }
    ofs.close();
    
}

void Graph::writeShortestPathsToJSON(
    const std::string& filename,
    const std::string& label,
    const std::vector<std::tuple<std::string, std::string, int, std::vector<std::string>>>& paths
) const {
    Document d;
    d.SetObject();
    Document::AllocatorType& allocator = d.GetAllocator();
    Value result_array(kArrayType);

    for (const auto& [from, to, distance, path] : paths) {
        Value entry(kObjectType);
        entry.AddMember("from", Value().SetString(from.c_str(), allocator), allocator);
        entry.AddMember("to", Value().SetString(to.c_str(), allocator), allocator);

        if (distance >= INT_MAX / 2) {
            entry.AddMember("distance", "unreachable", allocator);
            entry.AddMember("path", "none", allocator);
        } else {
            entry.AddMember("distance", distance, allocator);
            Value json_path(kArrayType);
            for (const auto& node : path) {
                json_path.PushBack(Value().SetString(node.c_str(), allocator), allocator);
            }
            entry.AddMember("path", json_path, allocator);
        }

        result_array.PushBack(entry, allocator);
    }

    d.AddMember(Value().SetString(label.c_str(), allocator), result_array, allocator);

    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    writer.SetIndent(' ', 4);
    d.Accept(writer);

    std::ofstream ofs(filename);
    if (ofs) {
        ofs << buffer.GetString();
        std::cout << "Shortest paths written to " << filename << "\n";
    } else {
        std::cerr << "Error writing to " << filename << "\n";
    }
}
