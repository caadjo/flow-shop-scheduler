#pragma once
#include "Graph.hpp"
#include "InstanceParser.hpp"
#include <vector>

struct FlowShopEvaluation {
    int makespan;
    long long flowtime;
    long long objective;
    std::vector<std::vector<int>> machineSequences;
};

class FlowShopModeler {
public:
    static std::vector<std::vector<int>> buildNaturalMachineSequences(const FlowShopInstance& inst);
    static Graph buildGraph(const FlowShopInstance& inst, const std::vector<std::vector<int>>& machineSequences);
    static FlowShopEvaluation evaluateSequences(const FlowShopInstance& inst, const std::vector<std::vector<int>>& machineSequences);
    static FlowShopEvaluation improveByAdjacentSwaps(const FlowShopInstance& inst, const std::vector<std::vector<int>>& initialSequences);
};
