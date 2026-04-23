#include <iostream>
#include <filesystem>
#include <numeric>
#include "InstanceParser.hpp"
#include "FlowShopModeler.hpp"
#include "GraphAlgorithms.hpp"

namespace fs = std::filesystem;

int main() {
    std::string folder = "data/instances";
    
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            try {
                FlowShopInstance inst = InstanceParser::parse(entry.path().string());
                
                std::vector<int> seq(inst.numJobs);
                std::iota(seq.begin(), seq.end(), 0);

                Graph g = FlowShopModeler::buildGraph(inst, seq);
                std::vector<int> topo = GraphAlgorithms::topologicalSort(g);
                LongestPathResult res = GraphAlgorithms::calculateLongestPath(g, topo);

                std::cout << "Instancia: " << entry.path().filename() << " | Makespan: " << res.maxLength << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Erro no arquivo " << entry.path().filename() << ": " << e.what() << "\n";
            }
        }
    }
    return 0;
}