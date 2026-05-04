#include "GraphAlgorithms.hpp"
#include <algorithm>
#include <stdexcept>

std::vector<int> GraphAlgorithms::topologicalSort(const Graph& g) {
    int n = g.getNumVertices();
    const std::vector<Vertex>& vertices = g.getVertices();

    std::vector<int> inDegree(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int i = 0; i < static_cast<int>(vertices[u].adj.size()); ++i) {
            int v = vertices[u].adj[i];
            inDegree[v]++;
        }
    }

    // Fila feita com vector para deixar o passo da caminhada bem explicito.
    std::vector<int> queue;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            queue.push_back(i);
        }
    }

    std::vector<int> topoOrder;
    int first = 0;
    while (first < static_cast<int>(queue.size())) {
        int u = queue[first];
        first++;

        topoOrder.push_back(u);

        for (int i = 0; i < static_cast<int>(vertices[u].adj.size()); ++i) {
            int v = vertices[u].adj[i];
            inDegree[v]--;
            if (inDegree[v] == 0) {
                queue.push_back(v);
            }
        }
    }

    if (static_cast<int>(topoOrder.size()) != n) {
        throw std::runtime_error("Erro: Grafo contem ciclo.");
    }

    return topoOrder;
}

LongestPathResult GraphAlgorithms::calculateLongestPath(const Graph& g, const std::vector<int>& topoOrder) {
    int n = g.getNumVertices();
    const std::vector<Vertex>& vertices = g.getVertices();

    std::vector<int> distance(n);
    std::vector<int> predecessor(n, -1);

    for (int i = 0; i < n; ++i) {
        distance[i] = vertices[i].weight;
    }

    // Caminho maximo em DAG: processa os vertices na ordem topologica.
    for (int i = 0; i < static_cast<int>(topoOrder.size()); ++i) {
        int u = topoOrder[i];

        for (int j = 0; j < static_cast<int>(vertices[u].adj.size()); ++j) {
            int v = vertices[u].adj[j];
            int candidate = distance[u] + vertices[v].weight;

            if (candidate > distance[v]) {
                distance[v] = candidate;
                predecessor[v] = u;
            }
        }
    }

    int endVertex = 0;
    for (int i = 1; i < n; ++i) {
        if (distance[i] > distance[endVertex]) {
            endVertex = i;
        }
    }

    std::vector<int> path;
    int current = endVertex;
    while (current != -1) {
        path.push_back(current);
        current = predecessor[current];
    }
    std::reverse(path.begin(), path.end());

    return {distance[endVertex], path, distance, predecessor};
}
