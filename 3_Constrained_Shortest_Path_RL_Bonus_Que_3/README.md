
## 🚚  Constrained Routing using Supervised Learning and Reinforcement Learning
This project implements and compares two machine learning approaches—Supervised Learning (Decision Tree and Random Forest) and Reinforcement Learning (Q-Learning)—to solve a constrained shortest path routing problem over a weighted, capacitated graph with multiple demands.


#### 🧩 Problem Statement
The **bonus** graph contains a list of demands, as well as capacities on the edges. A demand is a commodity specified by an origin node, a destination node, and a payload. 
#### Objectives:
- Minimize **total transit time** for all demands.
- **Respect edge capacities**.
- Allow **splitting of demands** across multiple feasible paths when needed.



#### 🧠 Approaches Implemented

##### 1. **Supervised Learning (Decision Tree & Random Forest)**
- Extracts shortest path-based features and labels
- Uses one-hot encoded node pairs and normalized demand
- Predicts next-hop for given source-destination-demand using trained classifiers

##### 2. **Reinforcement Learning (Q-Learning)**
- Trains a Q-table with reward shaped by negative transit time
- Handles payloads respecting edge capacities
- Learns routing policies from simulated episodes

---


#### ⚙️ How It Works

##### Run the project:

```bash
python main.py
```

##### Pipeline Summary:
- Load the graph and demand data

- Prepare features and train Decision Tree & Random Forest models 

- Predict routing paths with classifiers under capacity constraints

- Train Q-learning policy and extract learned routing paths

- Generate visualizations and a detailed PDF report with:
    - Classifier accuracy & metrics
    - Flow vs. capacity analysis
    - Path statistics


#### 🛠 Dependencies

Make sure you have the following Python packages installed:

> pip install numpy scikit-learn matplotlib networkx
#### 🗂 Input Format


The input file is `exercise_bonus.json` and contains:
- `nodes`: List of all node identifiers.
- `edges`: List of directed edges with:
  - `from`: source node
  - `to`: destination node
  - `transit_time`: edge weight
  - `capacity`: maximum allowable flow
- `demands`: List of demand entries with:
  - `from`: source
  - `to`: destination
  - `payload`: amount of flow to be routed

---

#### 📊 Output
| File                                                   | Description                                   |
| ------------------------------------------------------ | --------------------------------------------- |
| `Constrained_SP_Supervised_Learning.json`              | Decision Tree: Predicted paths and edge flows |
| `Constrained_SP_Supervised_Learning_RandomForest.json` | Random Forest: Predicted paths and edge flows |
| `Constrained_Q_Learning.json`                          | Q-Learning: Learned paths and edge flows      |
| `Flow_Graph_*.png`                                     | Visual graph with flow values on edges        |
| `Model_Analysis_Report.pdf`                            | Multi-page report comparing all approaches    |


##### 📌 Notes
- Graph is treated as undirected in parsing and routing
- Capacity-aware constraints are enforced in all routing solutions
- Q-learning is model-free and learns directly from demand routing episodes
- Random behavior in Q-learning can be made reproducible with random seeds





