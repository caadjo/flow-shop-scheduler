#pragma once

#include "Graph.hpp"
#include <vector>

struct LongestPathResult {
    int maxLength;
    std::vector<int> path;
};

class GraphAlgorithms {
public:
    // (Algoritmo de Kahn)
    static std::vector<int> topologicalSort(const Graph& g);

    // retorna o tamanho e a rota do caminho critico
    static LongestPathResult calculateLongestPath(const Graph& g, const std::vector<int>& topoOrder);
};