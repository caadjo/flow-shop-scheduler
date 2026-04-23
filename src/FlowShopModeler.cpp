#include "FlowShopModeler.hpp"

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