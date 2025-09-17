import matplotlib.pyplot as plt
import networkx as nx


def draw_graph_with_flows(G, edge_flows=None, paths=None, filename='graph_output.png'):
    pos = nx.spring_layout(G, seed=42)

    # Draw base graph
    nx.draw(G, pos, with_labels=True, node_size=500, node_color='lightblue', edge_color='gray')

    # Draw edge labels (e.g., capacities or flows)
    if edge_flows:
        edge_labels = { (u, v): f"{flow:.1f}" for (u, v), flow in edge_flows.items() }
        nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

    #plt.axis('off')
    plt.savefig(filename, dpi=300)
    plt.close()










