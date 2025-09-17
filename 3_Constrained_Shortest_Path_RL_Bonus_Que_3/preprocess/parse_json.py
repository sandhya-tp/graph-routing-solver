import json
import networkx as nx
from collections import defaultdict


def load_graph_demand(filename): 
    with open(filename, 'r') as file:
        graph_data = json.load(file)

    G = nx.Graph()
    
    adj_list_with_cap_weight = defaultdict(list)


    edge_capacities = defaultdict(list)


    graph_edges = []

    for edge in graph_data['edges']:
        u = edge['from']
        v = edge['to']
        wt = edge['transit_time']
        cap = edge['capacity']
        edge_capacities[(u,v)] = cap
        edge_capacities[(v,u)] = cap
        G.add_edge(u, v, weight=wt)
        
        adj_list_with_cap_weight[u].append((v, wt,cap))
        adj_list_with_cap_weight[v].append((u, wt,cap))

    
    for u, neighbors in adj_list_with_cap_weight.items():
        for v, weight, capacity in neighbors:
            graph_edges.append((u, v, weight))  
    
    
    weight_dict = {
    u: {v: weight for v, weight, _ in neighbors}
    for u, neighbors in adj_list_with_cap_weight.items()}

    demand_list = defaultdict(list)

    for demand in graph_data['demands']:
        u = demand['from']
        v = demand['to']
        payload = demand['payload']
        
        demand_list[u].append((v, payload))
    
    return G, adj_list_with_cap_weight, demand_list, edge_capacities, weight_dict, graph_edges

