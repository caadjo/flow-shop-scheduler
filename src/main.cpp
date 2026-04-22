#include <iostream>
#include "Graph.hpp"

int main() {
    std::cout << "--- Flow Shop Scheduler ---" << std::endl;

    try {
     
        Graph g(3);

       
        g.setVertexWeight(0, 10);
        g.setVertexWeight(1, 20);
        g.setVertexWeight(2, 30);

        g.addEdge(0, 1);
        g.addEdge(1, 2);

        std::cout << "Grafo criado com sucesso!" << std::endl;
        std::cout << "Numero de vertices: " << g.getNumVertices() << std::endl;
        
        const auto& vertices = g.getVertices();
        for (const auto& v : vertices) {
            std::cout << "Vertice " << v.id << " | Peso: " << v.weight 
                      << " | Arestas de saida: " << v.adj.size() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    return 0;
}