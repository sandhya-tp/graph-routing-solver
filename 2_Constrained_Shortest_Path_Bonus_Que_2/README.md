
##  🚚  Constrained Shortest Path Solver
This project provides a C++ implementation to solve a constrained shortest path routing problem over a weighted, capacitated graph with multiple demands, using the Gurobi Optimizer.
#### 🧩 Problem Statement

The **bonus** graph contains a list of demands, as well as capacities on the edges. A demand is a commodity specified by an origin node, a destination node, and a payload. 

The objective is to implement a shortest-path-based algorithm that:

- Minimizes the **overall transit time** for all demands.
- **Respects edge capacities**.
- **Splits** demands across multiple feasible routes when necessary.

---

#### 🧠 Theoretical Background

This problem can be modeled as a **Multicommodity Minimum Cost Flow (MCMCF)** problem defined as follows.

##### Given:

- An undirected network **G = (V, E)**, where:
  - **V** is the set of nodes.
  - **E** ⊆ V × V is the set of edges.

Each edge (i, j) ∈ E has:
- A **capacity** `c_ij ≥ 0`, shared by all commodities.
- A **transit time (cost)** `cost_ij ≥ 0` per unit flow.

A set of **commodities** K = {1, 2, ..., K}, where each commodity k ∈ K has:
- A **source** node `s_k ∈ V`
- A **destination** node `t_k ∈ V`
- A **demand** `d_k > 0`, which may be split across multiple paths

##### Objective:

Route the flow of each commodity such that:
- Its demand is satisfied.
- The total flow on any edge does not exceed its capacity.
- The total transit cost is **minimized**.

---

### Implemented Solution

#### ✅ Multi-Commodity Min Cost Flow Solver using Gurobi

- Reads graph and demand from a JSON file.
- Formulates and solves the MCMCF using the Gurobi optimizer.
- Outputs optimal routing and flow distribution in JSON format.

> 💡 See `MCMinFlow_solver.cpp` and `main.cpp` for the implementation.

---

#### Dependencies

- **Gurobi 9.0+** (Academic or commercial license required)
- **C++17 compiler** (Tested with `g++`)
- **RapidJSON** (Header-only, no separate installation required)

---

#### ⚙️ How to Build and Run

```bash
g++ -std=c++17 -I$GUROBI_HOME/include main.cpp MCMinFlow_solver.cpp \
    -L$GUROBI_HOME/lib -lgurobi_c++ -lgurobi120 -o mcmf_solver

./mcmf_solver
```

#### 🗂 Input Format
```json
{
  "nodes": ["a", "b", "c"],
  "edges": [
    { "from": "a", "to": "b", "transit_time": 5, "capacity": 10 },
    { "from": "b", "to": "c", "transit_time": 3, "capacity": 15 }
  ],
  "demands": [
    { "from": "a", "to": "c", "payload": 7.5 }
  ]
}
```
#### 📊 Output Format (Routing_output.json)
```json
{
  "commodities": [
    {
      "demands": [
        {
          "source": "a",
          "destination": "c",
          "demand": 7.5
        }
      ],
      "paths": [
        {
          "edge": "a-b",
          "transit_time": 5,
          "capacity": 10
        },
        {
          "edge": "b-c",
          "transit_time": 3,
          "capacity": 15
        }
      ]
    }
  ],
  "total_transit_time": 60.0
}
```
#### 📁Project Structure
```
project-root/
|
├── data/
│   └── exercise_bonus.json            # Graph input with edges and demands
│
├── config/
│   └── config.py                      # Constants like paths, hyperparameters
│
├── src/
│   ├── data_loader.py                 # Functions to load and parse JSON data
│   ├── preprocess.py                  # Path generation, one-hot encoding, etc.
│   ├── features.py                    # Builds training features and labels
│   ├── model.py                       # Trains classifier and evaluates
│   ├── utils.py                       # Node mappings, helpers
│   ├── visualize.py                   # Graph and path visualizations
│
├── main.py                            # Main script tying everything together
├── requirements.txt                   # Python dependencies
└── README.md                          # You're here
```
