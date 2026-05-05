#pragma once

#include <stdexcept>
#include <vector>

struct Vertex {
    int id;
    int weight;
    std::vector<int> adj;
};

class Graph {
private:
    std::vector<Vertex> vertices;

public:
    explicit Graph(int n);

    void setVertexWeight(int id, int weight);
    void addEdge(int from, int to);

    const std::vector<Vertex>& getVertices() const;
    int getNumVertices() const;
};
