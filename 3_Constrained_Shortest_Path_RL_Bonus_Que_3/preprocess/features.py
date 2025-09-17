import numpy as np
import networkx as nx
import copy
from collections import defaultdict
def find_max_demand(demand_list):
    max_demand = 0
    for _, demand_pairs in demand_list.items():
        for _, demand in demand_pairs:
            if max_demand < demand:
                max_demand = demand
    return max_demand



def node_to_hot_vectors(G, node_to_idx):
    nodes = sorted(G.nodes())
    num_nodes = len(nodes)
    node_one_hot_vectors = {}


    for node, index in node_to_idx.items():
        one_hot_vector = np.zeros(num_nodes)
        one_hot_vector[index] = 1
        node_one_hot_vectors[node] = one_hot_vector
    return node_one_hot_vectors


def prep_training_data(graph,demand_list):

    nodes = sorted(graph.nodes())
    node_to_idx = {node: idx for idx, node in enumerate(sorted(nodes))}
    idx_to_node = {idx: node for node, idx in node_to_idx.items()}


    max_demand = find_max_demand(demand_list)

    node_one_hot_vectors = {}
    node_one_hot_vectors = node_to_hot_vectors(graph, node_to_idx)
    
    
    
    
    iterative_paths = [] 
    rem_path = []
    shortest_path_list = compute_SP(graph, demand_list)
    for _, _, demand, path in shortest_path_list:
        for i in range(len(path)-1):
            u = path[i]
            v = path[i+1]
            iterative_paths.append((u, v, demand))
            rem_path.append(v)
        



    iterative_paths_hot_vec = []
    for u, v, demand in iterative_paths:
            u_hotvector = node_one_hot_vectors[u] 
            v_hotvector = node_one_hot_vectors[v]
            norm_demand = (float)(demand/max_demand)
            iterative_paths_hot_vec.append((u_hotvector, v_hotvector, norm_demand))


    rem_path_hot_vec = []
    for u in rem_path:
        u_hotvector = node_one_hot_vectors[u]
        rem_path_hot_vec.append(u_hotvector)





    X = [np.concatenate((u_hot, v_hot, [d])) for u_hot, v_hot, d in iterative_paths_hot_vec]

    idx_to_node = {idx: node for node, idx in node_to_idx.items()}


    y = []

    for hot_vec in rem_path_hot_vec:
        idx = int(np.argmax(hot_vec))  
        node = idx_to_node[idx]        
        y.append(node_to_idx[node])    


    return node_to_idx, idx_to_node, X, y




def compute_SP(G, demand_list):
    shortest_path_list = [] 

    for source_node, demand_pairs in demand_list.items():
        for target_node, demand in demand_pairs:

            path = nx.dijkstra_path(G, source=source_node, target=target_node)
            shortest_path_list.append((source_node, target_node, demand, path))
    return shortest_path_list

    

def predict_path(graph, demand_list, clf, node_one_hot_vectors, idx_to_node, edge_capacities, max_steps=20):
    
    Learned_paths = {}
    
    edge_flow_tracker = defaultdict(float)
    edge_remaining_capacity = copy.deepcopy(edge_capacities)

    for start_node, values in demand_list.items():
        for target_node, payload in values:
            actual_Payload = payload
        # Prepare the feature
            path = [start_node]
            current = start_node
            max_demand = find_max_demand(demand_list)

            demand_norm = float(payload) / max_demand
            visited = set()
            for i in range(max_steps):
                visited.add(current)
                current_vec = node_one_hot_vectors[current]
                target_vec = node_one_hot_vectors[target_node]

                feature = np.concatenate((current_vec, target_vec, [demand_norm])).reshape(1, -1)

                # Predict the next hop
                probs = clf.predict_proba(feature)[0]

                valid_label_indices = clf.classes_ 
                neighbors = list(graph.neighbors(current))

                # Map neighbors to indices
                node_to_idx, _, _,_ = prep_training_data(graph, demand_list)
                neighbor_indices = [node_to_idx[nbr] for nbr in neighbors if nbr in node_to_idx]

                # Filter neighbor indices to only those present in the classifier output
                filtered_neighbor_indices = [idx for idx in neighbor_indices if idx in valid_label_indices]

                if not filtered_neighbor_indices:
                    break 

                # Choose best next node from filtered neighbors
                best_idx = max(filtered_neighbor_indices, key=lambda idx: probs[list(valid_label_indices).index(idx)])  
                next_node = idx_to_node[best_idx]
                if (current, next_node) not in edge_remaining_capacity:
                    path.append("INVALID_EDGE")
                    break  
                # Stop if destination is reavhed
                if next_node == target_node:
                    path.append(next_node)
                    flow = min(payload, edge_remaining_capacity[(current, next_node)])
                    edge_flow_tracker[(current, next_node)] = edge_flow_tracker[(current, next_node)] + flow
                    edge_remaining_capacity[(current, next_node)] = edge_remaining_capacity[(current, next_node)] - flow
                    edge_remaining_capacity[( next_node, current)] = edge_remaining_capacity[(next_node, current)] - flow
                    payload =payload - flow
                    break
                    
                # Detect loop or dead-end
                if next_node in visited or edge_remaining_capacity[(current,next_node)]<=0:
                    path.append("LOOP/DEADEND")
                    break
                path.append(next_node)
                flow = min(payload, edge_remaining_capacity[(current, next_node)])
                edge_flow_tracker[(current, next_node)] = edge_flow_tracker[(current, next_node)] + flow
                edge_remaining_capacity[(current, next_node)] = edge_remaining_capacity[(current, next_node)]- flow
                edge_remaining_capacity[(next_node, current)] = edge_remaining_capacity[(next_node, current)]- flow
                payload = payload - flow
                current = next_node


            else:
                path.append("TOO LONG")

            Learned_paths[(start_node, target_node,actual_Payload)] = path
    
    return Learned_paths, edge_flow_tracker



