#pragma once

#include "Graph.hpp"
#include <vector>

struct LongestPathResult {
    int maxLength;
    std::vector<int> path;
    std::vector<int> distances; 
    std::vector<int> predecessors;  
};

class GraphAlgorithms {
public:
    static std::vector<int> topologicalSort(const Graph& g);
    static LongestPathResult calculateLongestPath(const Graph& g, const std::vector<int>& topoOrder);
};