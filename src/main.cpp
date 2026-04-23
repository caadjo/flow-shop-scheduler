#include <iostream>
#include <filesystem>
#include <numeric>
#include <vector>
#include <algorithm>
#include "InstanceParser.hpp"
#include "FlowShopModeler.hpp"
#include "GraphAlgorithms.hpp"

namespace fs = std::filesystem;

void printPathToTarget(int target, const std::vector<int>& predecessors) {
    std::vector<int> path;
    for (int at = target; at != -1; at = predecessors[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
    }
    std::cout << '\n';
}

void runFixedGraphTest() {
    Graph g(16);
    for (int i = 1; i <= 15; ++i) {
        g.setVertexWeight(i, 1);
    }

    g.addEdge(4, 3); g.addEdge(3, 5); g.addEdge(5, 15); g.addEdge(15, 6);
    g.addEdge(10, 1); g.addEdge(1, 11); g.addEdge(11, 14); g.addEdge(14, 13);
    g.addEdge(7, 8); g.addEdge(8, 2); g.addEdge(2, 12); g.addEdge(12, 9);
    
    g.addEdge(4, 1); g.addEdge(10, 3); g.addEdge(1, 2); g.addEdge(8, 14);
    g.addEdge(5, 14); g.addEdge(14, 6); g.addEdge(15, 9); g.addEdge(12, 13);

    std::vector<int> topo = GraphAlgorithms::topologicalSort(g);

    std::cout << "Topological Order: ";
    for (int v : topo) {
        if (v != 0) std::cout << v << ' ';
    }
    std::cout << "\n\n";

    LongestPathResult res = GraphAlgorithms::calculateLongestPath(g, topo);

    std::cout << "Global longest path length: " << res.maxLength << " | Path: ";
    printPathToTarget(res.path.back(), res.predecessors);

    std::cout << "Longest path to 6: Length " << res.distances[6] << " | Path: ";
    printPathToTarget(6, res.predecessors);

    std::cout << "Longest path to 13: Length " << res.distances[13] << " | Path: ";
    printPathToTarget(13, res.predecessors);

    std::cout << "Longest path to 9: Length " << res.distances[9] << " | Path: ";
    printPathToTarget(9, res.predecessors);
    std::cout << '\n';
}

void runFlowShopInstances() {
    std::vector<std::string> files;
    std::string folder = "data/instances";

    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        std::cerr << "Directory not found: " << folder << '\n';
        return;
    }

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    std::sort(files.begin(), files.end());

    for (const auto& filepath : files) {
        try {
            FlowShopInstance inst = InstanceParser::parse(filepath);

            std::vector<int> sequence(inst.numJobs);
            std::iota(sequence.begin(), sequence.end(), 0);

            Graph g = FlowShopModeler::buildGraph(inst, sequence);
            std::vector<int> topo = GraphAlgorithms::topologicalSort(g);
            LongestPathResult res = GraphAlgorithms::calculateLongestPath(g, topo);

            long long flowtime = 0;
            for (int j = 0; j < inst.numJobs; ++j) {
                int lastMachineNode = (j * inst.numMachines) + (inst.numMachines - 1);
                flowtime += res.distances[lastMachineNode];
            }

            std::cout << "Instance: " << fs::path(filepath).filename().string()
                      << " | Makespan: " << res.maxLength
                      << " | Flowtime: " << flowtime << '\n';

        } catch (const std::exception& e) {
            std::cerr << "Error processing " << filepath << ": " << e.what() << '\n';
        }
    }
}

int main() {
    runFixedGraphTest();
    runFlowShopInstances();
    return 0;
}