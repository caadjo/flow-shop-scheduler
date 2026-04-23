#include "GraphAlgorithms.hpp"
#include <queue>
#include <algorithm>
#include <stdexcept>

std::vector<int> GraphAlgorithms::topologicalSort(const Graph& g) {
    int n = g.getNumVertices();
    std::vector<int> inDegree(n, 0);
    const auto& vertices = g.getVertices();

    for (int i = 0; i < n; ++i) {
        for (int neighbor : vertices[i].adj) {
            inDegree[neighbor]++;
        }
    }

    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }

    std::vector<int> topoOrder;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topoOrder.push_back(u);

        for (int v : vertices[u].adj) {
            inDegree[v]--;
            if (inDegree[v] == 0) q.push(v);
        }
    }

    if (topoOrder.size() != static_cast<size_t>(n)) {
        throw std::runtime_error("Erro: Grafo contem ciclo.");
    }
    return topoOrder;
}

LongestPathResult GraphAlgorithms::calculateLongestPath(const Graph& g, const std::vector<int>& topoOrder) {
    int n = g.getNumVertices();
    std::vector<int> dist(n, 0);
    std::vector<int> predecessor(n, -1);
    const auto& vertices = g.getVertices();

    for (int i = 0; i < n; ++i) dist[i] = vertices[i].weight;

    for (int u : topoOrder) {
        for (int v : vertices[u].adj) {
            if (dist[u] + vertices[v].weight > dist[v]) {
                dist[v] = dist[u] + vertices[v].weight;
                predecessor[v] = u;
            }
        }
    }

    int maxDist = -1;
    int endNode = -1;
    for (int i = 0; i < n; ++i) {
        if (dist[i] > maxDist) {
            maxDist = dist[i];
            endNode = i;
        }
    }

    std::vector<int> path;
    for (int at = endNode; at != -1; at = predecessor[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    return {maxDist, path, dist, predecessor};
}