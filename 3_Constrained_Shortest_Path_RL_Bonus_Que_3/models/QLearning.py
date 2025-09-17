from collections import defaultdict
import random
import copy
import matplotlib.pyplot as plt


def train_qLearning(graph, adj_list_with_cap_weight,  edge_capacities, demand_list):
    # Initialize Q-table
    q_table = defaultdict(lambda: defaultdict(float))

    # Hyperparameters
    alpha = 0.1
    gamma = 0.9
    epsilon = 0.3
    episodes = 500

    edge_flow_tracker = defaultdict(float)
    edge_utilization = defaultdict(float)

    reward_per_episode = []


    for ep in range(episodes):
        adj_list_with_cap_weight_cpy = copy.deepcopy(adj_list_with_cap_weight)
        edge_flow_tracker = defaultdict(float)

        for source_node in demand_list:
            for (destination, total_payload) in demand_list[source_node]:
                current_node = source_node
                remaining_payload = total_payload

                remaining_edge_cap = 0
                attempts = 0
                max_attempts = 20 
                while remaining_payload > 0 and attempts < max_attempts:
                    attempts = attempts + 1
                    path = [current_node]
                    flow = 0
                    while (current_node != destination):
                        neighbors = list(graph.neighbors(current_node))

                        if not neighbors:
                            break  # no neighbors to explore
                        valid_actions = [(n, w, c) for (n, w, c) in adj_list_with_cap_weight_cpy[current_node] if c > 0]
                        if not valid_actions:
                            break  # No valid edge to explore

                        if random.random() < epsilon:
                            action = random.choice(valid_actions)[0]
                        else:
                            action = max(valid_actions, key=lambda x: q_table[current_node].get(x[0], 0))[0]


                        if action is None:
                            break
                        valid_edge = False

                        for idx, (neighbor, weight, cap) in enumerate(adj_list_with_cap_weight_cpy[current_node]):
                            if (neighbor == action):
                                if cap > 0:
                                    flow = min(cap, remaining_payload)
                                    adj_list_with_cap_weight_cpy[current_node][idx] = (neighbor, weight, cap - flow)
                                    cap = cap - flow
                                    remaining_payload = remaining_payload - flow
                                    edge_flow_tracker[(current_node, action)] = edge_flow_tracker[(current_node, action)] + flow

                                    Reward = -weight*flow
                                    
                                    path.append(action)
                                    old_q = q_table[current_node].get(action, 0)
                                    next_max_q = max(q_table[action].values(), default=0)
                                    q_table[current_node][action] = old_q + alpha * (Reward + gamma * next_max_q - old_q)
                                    current_node = action
                                    valid_edge = True
                                    break

                                
                        if not valid_edge:
                        
                            Reward = -10
                            q_table[current_node][action] = q_table[current_node].get(action, 0) + alpha * (Reward - q_table[current_node].get(action, 0))
                            break
                            
                    
        


    for (u, v), flow in edge_flow_tracker.items():
        if edge_capacities[(u, v)] > 0:
            edge_utilization[(u, v)] = flow / edge_capacities[(u, v)]
        else:
            edge_utilization[(u, v)] = 0  # avoid division by zero
    
    return q_table







def extract_learned_paths_flows(q_table, edge_capacities, demand_list):
    edge_flow_tracker = defaultdict(float)
    learned_paths = {}
    edge_remaining_capacity = copy.deepcopy(edge_capacities)


    for start_node, values in demand_list.items():
        #edge_flow_tracker = defaultdict(float)
        for end_node, payload in values:
            current_node = start_node
            path = [current_node]
            visited = set()
            remaining_demand = payload

            while current_node != end_node:
                visited.add(current_node)
                if current_node not in q_table:
                    break  # No Q-values learned
                next_node = max(q_table[current_node], key=q_table[current_node].get, default=None)
                if not next_node or next_node in visited:
                    break  # Dead-end or loop
                if edge_remaining_capacity[(current_node, next_node)] <= 0:
                    break  # Cannot send through this edge
                flow = min(payload, edge_remaining_capacity[(current_node, next_node)])

                edge_flow_tracker[(current_node, next_node)] = edge_flow_tracker[(current_node, next_node)] + flow
                path.append(next_node)
                edge_remaining_capacity[(current_node, next_node)] = edge_remaining_capacity[(current_node, next_node)]- flow
                remaining_demand = remaining_demand - flow
                current_node = next_node

            learned_paths[(start_node, end_node, payload)] = path

    return learned_paths, edge_flow_tracker

