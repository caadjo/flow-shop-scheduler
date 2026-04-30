#pragma once
#include "Graph.hpp"
#include "InstanceParser.hpp"
#include <vector>

struct FlowShopEvaluation {
    int makespan;
    long long flowtime;
    long long objective;
    std::vector<int> sequence;
};

class FlowShopModeler {
public:
    static Graph buildGraph(const FlowShopInstance& inst, const std::vector<int>& jobSeq);
    static FlowShopEvaluation evaluateSequence(const FlowShopInstance& inst, const std::vector<int>& jobSeq);
    static FlowShopEvaluation improveByAdjacentSwaps(const FlowShopInstance& inst, const std::vector<int>& initialSeq);
};
