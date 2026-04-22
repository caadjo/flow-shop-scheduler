#include "Graph.hpp"

Graph::Graph(int n) {
    vertices.resize(n);
    for (int i = 0; i < n; ++i) {
        vertices[i].id = i;
        vertices[i].weight = 0; 
    }
}

void Graph::setVertexWeight(int id, int weight) {
    if (id < 0 || id >= static_cast<int>(vertices.size())) {
        throw std::out_of_range("LIMITE");
    }
    vertices[id].weight = weight;
}

void Graph::addEdge(int from, int to) {
    if (from < 0 || from >= static_cast<int>(vertices.size()) || 
        to < 0 || to >= static_cast<int>(vertices.size())) {
        throw std::out_of_range("LIMITE");
    }
    vertices[from].adj.push_back(to);
}

const std::vector<Vertex>& Graph::getVertices() const {
    return vertices;
}

int Graph::getNumVertices() const {
    return vertices.size();
}