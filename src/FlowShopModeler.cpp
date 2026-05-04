#include "FlowShopModeler.hpp"
#include "GraphAlgorithms.hpp"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace {
const int MAX_LOCAL_SEARCH_PASSES = 1;

void validateMachineSequences(const FlowShopInstance& inst, const std::vector<std::vector<int>>& machineSequences) {
    if (static_cast<int>(machineSequences.size()) != inst.numMachines) {
        throw std::invalid_argument("Deve existir uma sequencia para cada maquina.");
    }

    for (const auto& sequence : machineSequences) {
        if (static_cast<int>(sequence.size()) != inst.numJobs) {
            throw std::invalid_argument("Cada sequencia deve conter todos os jobs da instancia.");
        }

        std::vector<bool> seen(inst.numJobs, false);
        for (int job : sequence) {
            if (job < 0 || job >= inst.numJobs) {
                throw std::invalid_argument("Uma sequencia contem job fora do intervalo da instancia.");
            }
            if (seen[job]) {
                throw std::invalid_argument("Cada sequencia deve ser uma permutacao, sem jobs repetidos.");
            }
            seen[job] = true;
        }
    }
}
}

std::vector<std::vector<int>> FlowShopModeler::buildNaturalMachineSequences(const FlowShopInstance& inst) {
    std::vector<int> naturalSequence(inst.numJobs);
    std::iota(naturalSequence.begin(), naturalSequence.end(), 0);
    return std::vector<std::vector<int>>(inst.numMachines, naturalSequence);
}

Graph FlowShopModeler::buildGraph(const FlowShopInstance& inst, const std::vector<std::vector<int>>& machineSequences) {
    validateMachineSequences(inst, machineSequences);

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
        const auto& machineSequence = machineSequences[k];
        for (size_t i = 0; i + 1 < machineSequence.size(); ++i) {
            g.addEdge((machineSequence[i] * m) + k, (machineSequence[i + 1] * m) + k);
        }
    }

    for (int j = 0; j < n; ++j) {
        g.addEdge(source, (j * m));
        g.addEdge((j * m) + m - 1, sink);
    }
    return g;
}

FlowShopEvaluation FlowShopModeler::evaluateSequences(const FlowShopInstance& inst, const std::vector<std::vector<int>>& machineSequences) {
    Graph g = buildGraph(inst, machineSequences);
    std::vector<int> topo = GraphAlgorithms::topologicalSort(g);
    LongestPathResult res = GraphAlgorithms::calculateLongestPath(g, topo);

    long long flowtime = 0;
    for (int j = 0; j < inst.numJobs; ++j) {
        int lastMachineNode = (j * inst.numMachines) + (inst.numMachines - 1);
        flowtime += res.distances[lastMachineNode];
    }

    return {res.maxLength, flowtime, flowtime + res.maxLength, machineSequences};
}

FlowShopEvaluation FlowShopModeler::improveByAdjacentSwaps(const FlowShopInstance& inst, const std::vector<std::vector<int>>& initialSequences) {
    std::vector<std::vector<int>> currentSequences = initialSequences;
    FlowShopEvaluation best = evaluateSequences(inst, currentSequences);

    bool improved = true;
    int pass = 0;
    while (improved && pass < MAX_LOCAL_SEARCH_PASSES) {
        improved = false;
        ++pass;

        for (int machine = 0; machine < inst.numMachines; ++machine) {
            for (size_t i = 0; i + 1 < currentSequences[machine].size(); ++i) {
                std::swap(currentSequences[machine][i], currentSequences[machine][i + 1]);

                try {
                    FlowShopEvaluation candidate = evaluateSequences(inst, currentSequences);
                    if (candidate.objective < best.objective) {
                        best = candidate;
                        improved = true;
                    } else {
                        std::swap(currentSequences[machine][i], currentSequences[machine][i + 1]);
                    }
                } catch (const std::runtime_error&) {
                    std::swap(currentSequences[machine][i], currentSequences[machine][i + 1]);
                }
            }
        }
    }

    return best;
}
