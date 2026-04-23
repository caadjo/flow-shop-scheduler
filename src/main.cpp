#include <iostream>
#include <filesystem>
#include <numeric>
#include <vector>
#include <algorithm>
#include "InstanceParser.hpp"
#include "FlowShopModeler.hpp"
#include "GraphAlgorithms.hpp"

namespace fs = std::filesystem;

int main() {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator("data/instances")) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }
    
    // ordena alfabetico
    std::sort(files.begin(), files.end()); 

    for (const auto& filepath : files) {
        try {
            FlowShopInstance inst = InstanceParser::parse(filepath);
            std::vector<int> seq(inst.numJobs);
            std::iota(seq.begin(), seq.end(), 0); 

            Graph g = FlowShopModeler::buildGraph(inst, seq);
            std::vector<int> topo = GraphAlgorithms::topologicalSort(g);
            LongestPathResult res = GraphAlgorithms::calculateLongestPath(g, topo);

            std::cout << "Instancia: " << fs::path(filepath).filename().string() 
                      << " | Makespan: " << res.maxLength << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Erro no arquivo " << filepath << ": " << e.what() << "\n";
        }
    }
    return 0;
}