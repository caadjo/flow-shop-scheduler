#pragma once
#include "Graph.hpp"
#include "InstanceParser.hpp"
#include <vector>

class FlowShopModeler {
public:
    static Graph buildGraph(const FlowShopInstance& inst, const std::vector<int>& jobSeq);
};