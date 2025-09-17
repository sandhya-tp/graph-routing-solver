import json

def json_output(filename, learned_paths, edge_flows):
    
        output_data = []
        for i, ((start, end, dmnd), path) in enumerate(learned_paths.items()):
            path_edges = list(zip(path[:-1], path[1:]))
            for (u, v), flow in edge_flows.items():
                #print(edge_flows[(u,v)])
                entry={"source": start,
                     "target": end,
                     "demand": dmnd,
                     "predicted_path": list(path),
                     "edge-utilization":  {f"{u}-{v}": edge_flows.get((u, v), 0) for (u, v) in path_edges }
                        }
            output_data.append(entry)
            
        with open(filename, "w") as f:
            json.dump(output_data, f, indent=4)


