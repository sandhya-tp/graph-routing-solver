import json

def load_graph_data(filename):
    with open(filename, 'r') as file:
        return json.load(file)

