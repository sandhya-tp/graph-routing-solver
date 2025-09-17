#include "MCMinFlow_solver.h"

#include "gurobi_c++.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <set>
#include <map>

using namespace std;
using namespace rapidjson;



void printGraph(const std::unordered_map<std::string, std::vector<std::tuple<std::string, int, int>>>& graph) 
{
    std::cout << "Graph adjacency list:\n";
    for (const auto& [node,neighbors] : graph) 
    {
        std::cout << node << " -> "<<endl;;
        for (const auto & [neighbor,weight,capacity] : neighbors)
            std::cout <<"\t"<< neighbor << " "<<weight<<" "<<capacity<<endl;
        
    }
}



void LoadGraphFromJSON_with_demands(const std::string& filename,
                       std::unordered_map<std::string, std::vector<std::tuple<std::string, int, int>>>& graph) 
    {


    std::vector<std::tuple<std::string, std::string, int, int>> edges_list;
    std::vector<std::tuple<std::string, std::string, float>> commodities;
    std::set<std::string> node_set;
        
      
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) 
    {
        std::cerr << "Error opening file\n";
        return;
    }

    char buffer[65536];
    FileReadStream is(fp, buffer, sizeof(buffer));

    Document d;
    d.ParseStream(is);
    fclose(fp);
    
    

    if (!d.HasMember("edges") || !d["edges"].IsArray()) 
    {
        std::cerr << "Invalid JSON format\n";
        return;
    }
     
    const Value& edges = d["edges"];
    for (SizeType i = 0; i < edges.Size(); i++) 
    {
        const Value& edge = edges[i];

        if (!edge.HasMember("from") || !edge.HasMember("to")) continue;
        if (!edge["from"].IsString() || !edge["to"].IsString()) continue;

        std::string u = edge["from"].GetString();
        std::string v = edge["to"].GetString();
        int weight = edge["transit_time"].GetInt();
        int capacity = edge["capacity"].GetInt();

        graph[u].emplace_back(v, weight,capacity);
        graph[v].emplace_back(u, weight,capacity);
        node_set.insert(u);
        node_set.insert(v);
    }

    
      
    if (!d.HasMember("demands") || !d["demands"].IsArray()) 
    {
        std::cerr << "Invalid JSON format\n";
        return;
    }
     
    const Value& demands = d["demands"];
    for (SizeType i = 0; i < demands.Size(); i++) 
    {
        const Value& demand = demands[i];

        if (!demand.HasMember("from") || !demand.HasMember("to")) continue;
        if (!demand["from"].IsString() || !demand["to"].IsString()) continue;

        std::string u = demand["from"].GetString();
        std::string v = demand["to"].GetString();
        float payload = demand["payload"].GetFloat();
      
        commodities.emplace_back(u,v,payload);
      
    }

    try {


        GRBEnv env = GRBEnv(true);
        env.start();
        GRBModel model = GRBModel(env);
        int num_nodes=node_set.size();
        int num_arcs = edges.Size();
        int num_comms = commodities.size();
        std::map<std::string, GRBVar> flow;

        for(const auto &[from_node,nbrs]:graph)
        {
     
            for(const auto &[to_node,transit,cap]:nbrs)      
            {
         
                for (int k = 0; k < num_comms; k++) 
                {
                    std::string name = "f_K" +std::to_string(k) + "_"+from_node +"_"+to_node;
                    flow[name] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, name);
                    //std::cout<<"Name: "<<name<<std::endl;
                }

            }


        }
        // Capacity constraints: total flow on each arc ≤ capacity
        std::set<std::pair<std::string, std::string>> processed_edges;


        for(const auto &[from_node,nbrs]:graph)
        {   

            for(const auto &[to_node, transit, cap]:nbrs) 
            {
         
                if (processed_edges.count({from_node, to_node})) continue;
                processed_edges.insert({from_node, to_node});
                processed_edges.insert({to_node, from_node});
        
                GRBLinExpr cap_expr = 0;
        
                for (int k = 0; k < num_comms; k++) 
                {
                    cap_expr=cap_expr + flow["f_K" +std::to_string(k) + "_"+from_node +"_"+to_node];
                    cap_expr=cap_expr + flow["f_K" +std::to_string(k) + "_"+to_node +"_"+from_node];
                }
        
        
                model.addConstr(cap_expr <= cap, "cap_" + from_node+"_"+to_node);

        

            }

        }


    
    

        // Flow conservation constraints
    

        for (int k = 0; k < num_comms; ++k) 
        {
            std::string src = std::get<0>(commodities[k]);
            std::string dst = std::get<1>(commodities[k]);
            float demand = std::get<2>(commodities[k]);
            for (const std::string& current_node : node_set) 
            {
                GRBLinExpr flow_in = 0;
                GRBLinExpr flow_out = 0;

      
                for(const auto& [from_node,nbrs]:graph)
                {
                    for(const auto& [to_node,transit,cap]:nbrs)
                    {
                        std::string name = "f_K" +std::to_string(k) + "_"+from_node +"_"+to_node;

        
                        if (to_node == current_node) flow_in += flow[name];
                        if (from_node == current_node) flow_out += flow[name];
                    }
                }
        

        
            //Adding flow conservation constraints for each node

                if (current_node == src) 
                {
                    model.addConstr(flow_out - flow_in == demand, "flow_K" +std::to_string(k) + "_"+current_node );
                } else if (current_node == dst) 
                {
                    model.addConstr(flow_out - flow_in == -demand, "flow_k" + std::to_string(k) + "_" +current_node);
                } else 
                {
                    model.addConstr(flow_out - flow_in == 0, "flow_k" + std::to_string(k) + "_" + current_node);
                }
            }
    

        }

        std::set<std::pair<std::string, std::string>> processed_edges_obj;

        

        // Objective: Minimize total transit cost
        GRBLinExpr objective = 0;
        for (int k = 0; k < num_comms; ++k) 
        {
      
            for(const auto &[from_node,nbrs]:graph)
            {
     
                for(const auto &[to_node, transit, cap]:nbrs)   
                {
                    std::string varname = "f_K" + std::to_string(k) + "_" + from_node + "_" + to_node;
                    if (flow.count(varname)) 
                    {
                        objective += transit * flow[varname];
                    }
                }
            }

        }
        
        model.setObjective(objective, GRB_MINIMIZE);


        //Solving the OPT problem


        model.optimize();
        int optimstatus = model.get(GRB_IntAttr_Status);
        if (optimstatus == GRB_OPTIMAL) 
        {
            Document d;
            d.SetObject();  
            Document::AllocatorType& allocator = d.GetAllocator();

            Value commodities_array(kArrayType);
    
            //std::cout << "\n=== Solution ===\n";
    
            int k=0;
            float total_transit_time = 0;
            for (const auto &[source,destination,demand] : commodities)
            {
                float total_transit_time = 0.0;
                //std::cout << "Commodity " << k << ":\n";
                //commodity_obj.AddMember("commodities", flows_array, allocator);
                Value commodity_obj(kObjectType);
        


                Value demand_array(kArrayType); 
                Value demand_obj(kObjectType);
                demand_obj.AddMember("source", Value().SetString(source.c_str(), allocator), allocator);
                demand_obj.AddMember("destination", Value().SetString(destination.c_str(), allocator), allocator);
                demand_obj.AddMember("demand", demand, allocator);
                demand_array.PushBack(demand_obj, allocator);
                //demand_obj.AddMember("demand", flows_array, allocator);
        
                commodity_obj.AddMember("demands", demand_array, allocator);
     

                Value flows_array(kArrayType); 
        
                //commodity_obj.AddMember("demand", k, allocator);

        
                for(const auto &[from_node,nbrs]:graph)
                {   
            
                    for(const auto &[to_node,transit, cap]:nbrs)  
                    {   
                    
                        std::string name = "f_K" +std::to_string(k) + "_"+from_node +"_"+to_node;
                        float fval = flow[name].get(GRB_DoubleAttr_X);
                        if (fval>0)
                        {


                            Value flow_obj(kObjectType);
                            std::string edge = from_node+"-"+to_node;
                            flow_obj.AddMember("edge", Value().SetString(edge.c_str(), allocator), allocator);
                            flow_obj.AddMember("transit_time", transit, allocator);
                            flow_obj.AddMember("capacity", cap, allocator);

                            flows_array.PushBack(flow_obj, allocator);
                            total_transit_time = total_transit_time + transit * fval;

                            //std::cout << "  Flow on arc " << from_node<<" "<<to_node << ": " << flow[name].get(GRB_DoubleAttr_X) << "\n";
                        }
                    }

                }
                commodity_obj.AddMember("paths", flows_array, allocator);
                commodity_obj.AddMember("total_transit_time", total_transit_time, allocator);

                commodities_array.PushBack(commodity_obj, allocator);
   

                //std::cout << "Manual total transit: " << total_transit_time << std::endl;


                //std::cout << "Gurobi objective: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;


                k++;

            }
            d.AddMember("commodities", commodities_array, allocator);




            Value summary(kObjectType);
            summary.AddMember("objective", model.get(GRB_DoubleAttr_ObjVal), allocator);
            summary.AddMember("runtime_seconds", model.get(GRB_DoubleAttr_Runtime), allocator);
            summary.AddMember("num_variables", model.get(GRB_IntAttr_NumVars), allocator);
            summary.AddMember("num_constraints", model.get(GRB_IntAttr_NumConstrs), allocator);
            summary.AddMember("solutions_found", model.get(GRB_IntAttr_SolCount), allocator);
            int threads_used = env.get(GRB_IntParam_Threads);
            summary.AddMember("threads_used", threads_used, allocator);

            d.AddMember("summary", summary, allocator);



            FILE* fp = fopen("Routing_output.json", "w");
            if (!fp) 
            {
                std::cerr << "Error opening file for writing\n";
                return;
            }
     
            char writeBuffer[65536];
            FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
            Writer<FileWriteStream> writer(os);
            d.Accept(writer);
            fclose(fp); 
            std::cout<<"The result is written in Routing_output.json file"<<std::endl;

            //std::cout << "Total cost = " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
        }
        else if (optimstatus == GRB_INFEASIBLE) 
        {
            std::cerr << "Model is infeasible.\n";
            model.computeIIS();
            model.write("/home/sandhya/Documents/infeasibility.ilp");
        } else 
        {
            std::cerr << "Optimization was stopped with status = " << optimstatus << "\n";
        }
   


    }   catch (GRBException &e) 
    {
        std::cerr << "Gurobi error code = " << e.getErrorCode() << ": " << e.getMessage() << "\n";
    } catch (...) 
    {
        std::cerr << "Unknown error occurred.\n";
    }



}



