# Non-Permutation Flow Shop Scheduler

![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![Status](https://img.shields.io/badge/status-Academic%20Project-orange)
![License](https://img.shields.io/badge/license-MIT-green)

An educational project for the **Graph Algorithms** course. This application evaluates and optimizes job sequences for the **Non-Permutation Flow Shop Scheduling Problem (FSP)** by minimizing a combined objective function:

```text
Objective = Flowtime + Makespan
```

In a Non-Permutation FSP, each machine can have its own unique processing sequence. Therefore, a solution consists of a set of sequences (one per machine) that minimizes the total completion time and the sum of completion times.

## Algorithm & Modeling

The problem is modeled as a **Weighted Directed Acyclic Graph (DAG)**, where:
- **Vertices:** Each operation (job $i$ on machine $j$) is a vertex with a weight equal to its processing time.
- **Edges:** Represent precedence constraints:
    - **Job Constraints:** Operation $(i, j)$ must finish before $(i, j+1)$ starts.
    - **Machine Constraints:** Job sequence on each specific machine.

### Processing Pipeline
1. **DAG Construction:** Build the graph based on the provided machine sequences.
2. **Topological Sort:** Determine a valid linear ordering of operations in $O(V + E)$ time.
3. **Critical Path (Longest Path):** Calculate the maximum distance from the source to each vertex to obtain completion times.
4. **Objective Evaluation:** Compute the Makespan (max completion time) and Flowtime (sum of job completion times).

The complexity of the evaluation is **linear**, $O(V + E)$, relative to the size of the graph.

## Heuristics

The project implements and compares several heuristics on the **Taillard Benchmark**:

- **Makespan + Flowtime:** Custom implementation using a First-Improvement local search with adjacent swaps.
- **NEH (Nawaz-Enscore-Ham):** Constructive heuristic for makespan minimization.
- **CDS (Campbell-Dudek-Smith):** Multi-stage extension of Johnson's algorithm.
- **Palmer's Slope Index:** Priority-based heuristic using processing time trends.
- **FCFS (First-Come, First-Served):** Baseline using the natural sequence of jobs.

## Results

Performance is measured using the **Relative Percentage Deviation (RPD)**:
$$RPD = \frac{f(\pi) - f^*}{f^*} \times 100$$
*Where $f^*$ is the best-known value for the instance.*

### Global Average RPD (Taillard Benchmark)

| Heuristic | Global Average RPD (%) |
| :--- | :---: |
| **NEH** | **3.33%** |
| CDS | 9.96% |
| Palmer | 10.74% |
| FCFS | 21.57% |
| Makespan + Flowtime | 21.67% |

*Note: The "Makespan + Flowtime" algorithm occasionally accepts slight increases in Makespan to significantly reduce Flowtime, adhering to the multi-objective nature of the implemented function.*

## Project Structure

```text
.
├── build/              # Compiled binaries and object files
├── data/
│   └── instances/      # Taillard benchmark instances (ta001 - ta120)
├── include/            # Header files (.hpp)
├── src/                # Implementation files (.cpp)
├── scripts/            # Support scripts for analysis
└── README.md           # Project documentation
```

## Getting Started

### Prerequisites
- GCC/Clang with C++17 support
- Make

### Compilation
```bash
make
```
To clean build files:
```bash
make clean
```

### Execution

**Run all instances:**
```bash
./build/bin/flowshop
```

**Run a specific instance by name:**
```bash
./build/bin/flowshop ta001
```

**Show DAG details (Topological Order & Critical Path):**
```bash
./build/bin/flowshop --details ta001
```

**Run the Fixed 15-Vertex Graph Test:**
Used to verify the DAG implementation against the known results from the course material.
```bash
./build/bin/flowshop --fixed-graph
```
*Fixed Graph Results:*
- Max Path (Minimal to Maximal): 7 → 10 → 1 → 11 → 14 → 5 → 15 → 9 (Weight: 72)
- Node 6 Max Dist: 69
- Node 13 Max Dist: 56
- Node 9 Max Dist: 72

## Academic Context

Developed as a final project for the **Graph Algorithms** course at **IFNMG - Campus Montes Claros**.

**Objective:** Demonstrate the application of Directed Acyclic Graphs, Topological Sorting, and Critical Path algorithms in industrial optimization problems.
