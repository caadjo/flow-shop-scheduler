#include "FlowShopModeler.hpp"
#include "GraphAlgorithms.hpp"
#include <algorithm>

namespace {
const int MAX_LOCAL_SEARCH_PASSES = 5;
}

Graph FlowShopModeler::buildGraph(const FlowShopInstance& inst, const std::vector<int>& jobSeq) {
    int n = inst.numJobs, m = inst.numMachines;
    int totalVertices = (n * m) + 2;
    Graph g(totalVertices);
    int source = totalVertices - 2, sink = totalVertices - 1;

    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < m; ++k) {
            g.setVertexWeight((j * m) + k, inst.processingTimes[j][k]);
        }
    }
    g.setVertexWeight(source, 0);
    g.setVertexWeight(sink, 0);

    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < m - 1; ++k) g.addEdge((j * m) + k, (j * m) + k + 1);
    }

    for (int k = 0; k < m; ++k) {
        for (size_t i = 0; i < jobSeq.size() - 1; ++i) {
            g.addEdge((jobSeq[i] * m) + k, (jobSeq[i + 1] * m) + k);
        }
    }

    for (int j = 0; j < n; ++j) {
        g.addEdge(source, (j * m));
        g.addEdge((j * m) + m - 1, sink);
    }
    return g;
}

FlowShopEvaluation FlowShopModeler::evaluateSequence(const FlowShopInstance& inst, const std::vector<int>& jobSeq) {
    Graph g = buildGraph(inst, jobSeq);
    std::vector<int> topo = GraphAlgorithms::topologicalSort(g);
    LongestPathResult res = GraphAlgorithms::calculateLongestPath(g, topo);

    long long flowtime = 0;
    for (int j = 0; j < inst.numJobs; ++j) {
        int lastMachineNode = (j * inst.numMachines) + (inst.numMachines - 1);
        flowtime += res.distances[lastMachineNode];
    }

    return {res.maxLength, flowtime, flowtime + res.maxLength, jobSeq};
}

FlowShopEvaluation FlowShopModeler::improveByAdjacentSwaps(const FlowShopInstance& inst, const std::vector<int>& initialSeq) {
    std::vector<int> currentSeq = initialSeq;
    FlowShopEvaluation best = evaluateSequence(inst, currentSeq);

    bool improved = true;
    int pass = 0;
    while (improved && pass < MAX_LOCAL_SEARCH_PASSES) {
        improved = false;
        ++pass;

        for (size_t i = 0; i + 1 < currentSeq.size(); ++i) {
            std::swap(currentSeq[i], currentSeq[i + 1]);
            FlowShopEvaluation candidate = evaluateSequence(inst, currentSeq);

            if (candidate.objective < best.objective) {
                best = candidate;
                improved = true;
            } else {
                std::swap(currentSeq[i], currentSeq[i + 1]);
            }
        }
    }

    return best;
}
