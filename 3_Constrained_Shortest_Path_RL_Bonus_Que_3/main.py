# main.py
from config import GRAPH_JSON_PATH, TEST_SIZE, RANDOM_STATE, VAL_SIZE
#from data_loader import load_graph_data
from preprocess.parse_json import load_graph_demand
from preprocess.features import compute_SP, prep_training_data, predict_path, node_to_hot_vectors
from models.decision_tree import train_decision_tree, train_random_forest
from models.QLearning import train_qLearning, extract_learned_paths_flows
from output_processing.json_output_processing import json_output
from output_processing.graph_drawing import draw_graph_with_flows
from output_processing.report_generation import generate_analysis_report
import numpy as np

def main():
    
    G, adj_list_with_cap_weight, demand_list, edge_capacities, weight_dict, graph_edges = load_graph_demand(GRAPH_JSON_PATH)
    node_to_idx, idx_to_node, X, y = prep_training_data(G, demand_list)

    clf_dt, X_train_dt, X_test_dt, y_train_dt, y_test_dt, X_val_dt, y_val_dt = train_decision_tree(X, y, test_size=TEST_SIZE, val_size = VAL_SIZE, random_state=RANDOM_STATE )
    node_one_hot_vectors = node_to_hot_vectors(G, node_to_idx)
    Learned_paths, edge_flow_tracker = predict_path(G, demand_list, clf_dt, node_one_hot_vectors, idx_to_node, edge_capacities, max_steps=20)
    
    filename = 'output/Constrained_SP_Supervised_Learning.json'
    json_output(filename, Learned_paths, edge_flow_tracker)
    draw_graph_with_flows(G, edge_flows=edge_flow_tracker, paths=Learned_paths, filename = 'output/Flow_Graph_SP_SupervisedLearning_DecisionTree.png')

    

    clf_rf, X_train_rf, X_val_rf, y_train_rf, y_val_rf, X_test_rf, y_test_rf = train_random_forest(X, y, test_size=TEST_SIZE, val_size = VAL_SIZE, random_state=RANDOM_STATE)
    Learned_paths, edge_flow_tracker = predict_path(G, demand_list, clf_rf, node_one_hot_vectors, idx_to_node, edge_capacities, max_steps=20)
    filename = 'output/Constrained_SP_Supervised_Learning_RandomForest.json'
    json_output(filename, Learned_paths, edge_flow_tracker)
    draw_graph_with_flows(G, edge_flows=edge_flow_tracker, paths=Learned_paths, filename = 'output/Flow_Graph_SP_SupervisedLearning_RandomForest.png')


    q_table = train_qLearning(G, adj_list_with_cap_weight,  edge_capacities, demand_list)
    Learned_paths, edge_flows = extract_learned_paths_flows(q_table, edge_capacities, demand_list)

    filename = 'output/Constrained_Q_Learning.json'
    json_output(filename, Learned_paths, edge_flows)

    G.add_weighted_edges_from(graph_edges)  
    draw_graph_with_flows(G, edge_flows=edge_flows, paths=Learned_paths, filename = 'output/Flow_Graph_Constrained_Q_Learning.png')

    
    generate_analysis_report(Learned_paths, clf_dt, clf_rf, X_test_dt, y_test_dt, X_val_dt, y_val_dt, X_test_rf, y_test_rf, X_val_rf, y_val_rf,  edge_flows, edge_capacities, weight_dict, reward_log=reward_log if 'reward_log' in globals() else None,  output_pdf_path="output/Model_Analysis_Report.pdf")
    
    
if __name__ == "__main__":
    main()
