#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "FlowShopModeler.hpp"
#include "GraphAlgorithms.hpp"
#include "InstanceParser.hpp"

namespace fs = std::filesystem;

void printUsage(const char* programName) {
    std::cout << "Usage:\n"
              << "  " << programName << "                    # executar todas instâncias\n"
              << "  " << programName << " ta001              # executar uma instância pelo nome\n"
              << "  " << programName << " path/to/file       # executar uma instância pelo caminho\n"
              << "  " << programName << " --details ta001    # mostrar percurso do DAG e caminho mais longo\n"
              << "  " << programName << " --fixed-graph      # executar o grafo fixo do PDF\n";
}

void printSequence(const std::vector<int>& sequence) {
    for (size_t i = 0; i < sequence.size(); ++i) {
        std::cout << sequence[i] + 1;
        if (i + 1 < sequence.size()) {
            std::cout << ' ';
        }
    }
}

void printMachineSequences(const std::vector<std::vector<int>>& machineSequences) {
    for (size_t machine = 0; machine < machineSequences.size(); ++machine) {
        std::cout << "M" << machine + 1 << ":[";
        printSequence(machineSequences[machine]);
        std::cout << "]";
        if (machine + 1 < machineSequences.size()) {
            std::cout << ' ';
        }
    }
}

void printVectorOneBased(const std::vector<int>& values) {
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << values[i] + 1;
        if (i + 1 < values.size()) {
            std::cout << ' ';
        }
    }
}

void printVertexLabels(const std::vector<int>& vertices, const std::vector<int>& labels) {
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << labels[vertices[i]];
        if (i + 1 < vertices.size()) {
            std::cout << ' ';
        }
    }
}

std::vector<int> reconstructPathTo(int endVertex, const LongestPathResult& result) {
    std::vector<int> path;
    for (int at = endVertex; at != -1; at = result.predecessors[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::string> listInstanceFiles(const std::string& folder) {
    std::vector<std::string> files;

    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        std::cerr << "Directory not found: " << folder << '\n';
        return files;
    }

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    std::sort(files.begin(), files.end());
    return files;
}

std::string resolveInstancePath(const std::string& instanceArg) {
    if (fs::exists(instanceArg)) {
        return instanceArg;
    }

    std::string instancePath = "data/instances/" + instanceArg;
    if (fs::exists(instancePath)) {
        return instancePath;
    }

    return "";
}

Graph buildFixedGraphFromPdf(std::vector<int>& labels) {

    labels = {
        4, 3, 5, 15, 6,
        10, 1, 11, 14, 13,
        7, 8, 2, 12, 9
    };

    Graph graph(static_cast<int>(labels.size()));
    for (int i = 0; i < static_cast<int>(labels.size()); ++i) {
        graph.setVertexWeight(i, labels[i]);
    }

    const std::vector<std::pair<int, int>> edges = {
    // linha 1
    {0, 1},   // 4  -> 3
    {1, 2},   // 3  -> 5
    {2, 3},   // 5  -> 15
    {3, 4},   // 15 -> 6
    {3, 14},  // 15 -> 9
    // linha 2
    {5, 6},   // 10 -> 1
    {5, 1},   // 10 -> 3
    {6, 7},   // 1  -> 11
    {6, 12},  // 1  -> 2
    {7, 8},   // 11 -> 14
    {7, 3},   // 11 -> 15
    {8, 9},   // 14 -> 13
    {8, 2},   // 14 -> 5 
    {9, 4},   // 13 -> 6
    // linha 3
    {10, 5},  // 7  -> 10
    {10, 11}, // 7  -> 8
    {11, 12}, // 8  -> 2
    {11, 8},  // 8  -> 14
    {12, 13}, // 2  -> 12
    {13, 14}, // 12 -> 9
    {13, 9}   // 12 -> 13
};
    for (const auto& edge : edges) {
        graph.addEdge(edge.first, edge.second);
    }

    return graph;
}

void runFixedGraphTest() {
    std::vector<int> labels;
    Graph graph = buildFixedGraphFromPdf(labels);
    std::vector<int> topoOrder = GraphAlgorithms::topologicalSort(graph);
    LongestPathResult result = GraphAlgorithms::calculateLongestPath(graph, topoOrder);

    const std::vector<int> lineEnds = {4, 9, 14};

    std::cout << "Seção 2: Grafo Fixo\n";
    std::cout << "Modelagem: Dígrafo com pesos nos vértices.\n";
    std::cout << "Ordem que os vértices são percorridos (Caminhada Topológica): ";
    printVertexLabels(topoOrder, labels);
    std::cout << "\n\n";

    std::cout << "Caminho máximo de um elemento minimal para um elemento maximal\n";
    std::cout << "Comprimento: " << result.maxLength << "\n";
    std::cout << "Lista: ";
    printVertexLabels(result.path, labels);
    std::cout << "\n\n";

    std::cout << "Caminho máximo de um elemento minimal para cada elemento no final de cada linha\n";
    for (int endVertex : lineEnds) {
        std::vector<int> path = reconstructPathTo(endVertex, result);
        std::cout << "(Nó " << labels[endVertex] << "):\n"
                  << "  Comprimento: " << result.distances[endVertex] << "\n"
                  << "  Caminho: ";
        printVertexLabels(path, labels);
        std::cout << "\n";
    }
}

void printDetailedDagResult(const std::string& filepath) {
    FlowShopInstance inst = InstanceParser::parse(filepath);

    std::vector<std::vector<int>> naturalSequences = FlowShopModeler::buildNaturalMachineSequences(inst);
    FlowShopEvaluation result = FlowShopModeler::improveByAdjacentSwaps(inst, naturalSequences);
    Graph graph = FlowShopModeler::buildGraph(inst, result.machineSequences);
    std::vector<int> topoOrder = GraphAlgorithms::topologicalSort(graph);
    LongestPathResult longestPath = GraphAlgorithms::calculateLongestPath(graph, topoOrder);

    std::cout << "Problema Geral - Flow Shop (Non-Permutation)\n"
              << "Objetivo: Minimização de Makespan e Flowtime\n"
              << "Instância: " << fs::path(filepath).filename().string() << '\n'
              << "Jobs: " << inst.numJobs << " | Machines: " << inst.numMachines << '\n'
              << "Melhor Sequência por Máquina: ";
    printMachineSequences(result.machineSequences);
    std::cout << "\n\nMakespan (Tempo de Término): " << result.makespan
              << "\nFlowtime: " << result.flowtime
              << "\nObjetivo (Makespan + Flowtime): " << result.objective
              << "\n\nCaminhada Topológica: ";
    printVectorOneBased(topoOrder);
    std::cout << "\n\nComprimento do Caminho Máximo: " << longestPath.maxLength
              << "\nCaminho Máximoo Crítico: ";
    printVectorOneBased(longestPath.path);
    std::cout << '\n';
}

void runFlowShopInstance(const std::string& filepath, bool printSequences) {
    FlowShopInstance inst = InstanceParser::parse(filepath);

    std::vector<std::vector<int>> naturalSequences = FlowShopModeler::buildNaturalMachineSequences(inst);
    FlowShopEvaluation initial = FlowShopModeler::evaluateSequences(inst, naturalSequences);
    FlowShopEvaluation result = FlowShopModeler::improveByAdjacentSwaps(inst, naturalSequences);
    long long improvement = initial.objective - result.objective;

    std::cout << "Instance: " << fs::path(filepath).filename().string()
              << " | InitialObjective: " << initial.objective
              << " | FinalObjective: " << result.objective
              << " | Improvement: " << improvement
              << " | Makespan: " << result.makespan
              << " | Flowtime: " << result.flowtime;

    if (printSequences) {
        std::cout << " | MachineSequences: ";
        printMachineSequences(result.machineSequences);
    }
    std::cout << '\n';
}

void runFlowShopInstances() {
    std::vector<std::string> files = listInstanceFiles("data/instances");

    for (const auto& filepath : files) {
        try {
            runFlowShopInstance(filepath, false);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao carregar " << filepath << ": " << e.what() << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 3) {
        printUsage(argv[0]);
        return 1;
    }

    if (argc == 3) {
        std::string option = argv[1];
        if (option != "--details") {
            printUsage(argv[0]);
            return 1;
        }

        std::string filepath = resolveInstancePath(argv[2]);
        if (filepath.empty()) {
            std::cerr << "Instância não encontrada: " << argv[2] << '\n';
            printUsage(argv[0]);
            return 1;
        }

        try {
            printDetailedDagResult(filepath);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar " << filepath << ": " << e.what() << '\n';
            return 1;
        }
        return 0;
    }

    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        }

        if (arg == "--fixed-graph") {
            try {
                runFixedGraphTest();
            } catch (const std::exception& e) {
                std::cerr << "Erro ao processar grafo fixo: " << e.what() << '\n';
                return 1;
            }
            return 0;
        }

        std::string filepath = resolveInstancePath(arg);
        if (filepath.empty()) {
            std::cerr << "Instância não encontrada: " << arg << '\n';
            printUsage(argv[0]);
            return 1;
        }

        try {
            runFlowShopInstance(filepath, true);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar " << filepath << ": " << e.what() << '\n';
            return 1;
        }
        return 0;
    }

    runFlowShopInstances();
    return 0;
}
