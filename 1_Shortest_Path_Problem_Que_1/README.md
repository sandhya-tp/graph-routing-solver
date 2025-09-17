## 🚚 Shortest Path Finder
This project provides a C++ implementation for generating graphs, importing graph structures from JSON, and computing shortest paths using Dijkstra's and Floyd-Warshall algorithms. It also includes detailed logging of operations and performance statistics.
#### 🧩 Problem Statement

Develop a shortest-path module with the following capabilities:

- ✅ Generate a **random connected graph** with `n` vertices and `m` edges (ensuring full connectivity).
- ✅ Import/export graph structures via **JSON files**.
- ✅ Implement shortest path algorithms for:
  - Shortest path **between any two vertices**
  - Shortest paths **to a single vertex** from all others
  - Shortest paths **from a single vertex** to all others
  - All-pairs shortest paths (Floyd–Warshall)
- ✅ Logging of calls to shortest path functions, performance metrics, and graph properties.

---

#### Features
- Random Connected Graph Generation
- Import/Export Graphs via JSON
- Shortest Path Algorithms
    - Single-source shortest paths using **Dijkstra's algorithm** (assumes **no negative edge weights**).
    - Shortest path between two specified nodes using **Dijkstra's algorithm**.
    - All-pairs shortest paths using **Floyd-Warshall algorithm** (assumes **no negative cycles**).

- Connectivity Checking using **BFS**
- Logging

#### Requirements 
C++17 or later
RapidJSON library (included in the project)
CMake (for building)

#### 🛠️ Build Instructions
Using g++ directly

```bash
g++ -std=c++17 -o graph_shortest_paths \
    main.cpp \
    Graph_Generator.cpp \
    Graph_json_processing.cpp \
    Graph_logger.cpp \
    Graph_Shortest_Paths.cpp \
    -I/path/to/rapidjson/include
```
Replace /path/to/rapidjson/include with the actual path to the RapidJSON header files.
Run the program
```bash
./graph_shortest_paths
```


#### 📁 File Structure

| File                        | Purpose                                                                 |
|----------------------------|-------------------------------------------------------------------------|
| `main.cpp`                 | Entry point: Demonstrates usage and runs example computations           |
| `Graph.hpp / Graph.cpp`    | Core Graph class with shortest path algorithms and utilities          |
| `Graph_logger.hpp / .cpp`  | `Logger` class for recording operations, runtime stats, and path logs   |
| `Graph_Generator.cpp`      | Random connected graph generation routines                              |
| `Graph_json_processing.cpp`| Functions to import/export graphs and results in JSON format            |
| `exercise_baseline.json`   | Sample graph structure used for testing and demonstrations              |
| `logs.txt`                 | Output log file containing metrics, graph stats, and performance logs   |
| `Output/dijkstra_paths_to.json`   | Example output file for Dijkstra's algorithm results (to a single node) |




#### Usage

The main program performs the following operations by default:
- Generates a random connected graph
- Loads a graph from JSON (exercise_baseline.json)
- Performs various graph algorithms:
- Dijkstra's algorithm from a source node to all nodes
- Dijkstra's algorithm between two specific nodes
- Floyd-Warshall all-pairs shortest paths
- Dijkstra's algorithm from all nodes to a target node
- Logs performance metrics to logs.txt

#### 🗂 Customizing Input
Graphs should be provided in JSON format with the following structure:

```json 
{
    "edges": [
        {
            "from": "a",
            "to": "b",
            "transit_time": weight
        },
        ...
    ]
}
```
#### 📊 Output Files

The program generates several output files in the folder "Output":
- dijkstra_paths_from.json: Shortest paths from a source node
- dijkstra_paths_to.json: Shortest paths to a target node
- dijkstra_path_between_two_nodes.json: Shortest path between two specific nodes
- all_pairs_output.json: All-pairs shortest paths
- logs.txt: Performance metrics and operation logs
