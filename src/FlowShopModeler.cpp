#include "FlowShopModeler.hpp"
#include "GraphAlgorithms.hpp"
#include <algorithm>
#include <stdexcept>
#include <vector>

const int MAX_LOCAL_SEARCH_PASSES = 1;

namespace {
int operationVertex(int job, int machine, int numberOfMachines) {
    return (job * numberOfMachines) + machine;
}

void validateOneMachineSequence(const FlowShopInstance& inst, const std::vector<int>& sequence) {
    if (static_cast<int>(sequence.size()) != inst.numJobs) {
        throw std::invalid_argument("Cada máquina deve ter todos os jobs.");
    }

    std::vector<int> count(inst.numJobs, 0);

    for (int i = 0; i < static_cast<int>(sequence.size()); ++i) {
        int job = sequence[i];
        if (job < 0 || job >= inst.numJobs) {
            throw std::invalid_argument("Sequência de máquina tem job fora do intervalo.");
        }
        count[job]++;
    }

    for (int job = 0; job < inst.numJobs; ++job) {
        if (count[job] != 1) {
            throw std::invalid_argument("Sequência de máquina não é uma permutação válida.");
        }
    }
}

void validateMachineSequences(const FlowShopInstance& inst, const std::vector<std::vector<int>>& machineSequences) {
    if (static_cast<int>(machineSequences.size()) != inst.numMachines) {
        throw std::invalid_argument("Deve existir uma sequência para cada máquina.");
    }

    for (int machine = 0; machine < inst.numMachines; ++machine) {
        validateOneMachineSequence(inst, machineSequences[machine]);
    }
}
}

std::vector<std::vector<int>> FlowShopModeler::buildNaturalMachineSequences(const FlowShopInstance& inst) {
    std::vector<std::vector<int>> sequences;

    for (int machine = 0; machine < inst.numMachines; ++machine) {
        std::vector<int> sequence;

        for (int job = 0; job < inst.numJobs; ++job) {
            sequence.push_back(job);
        }

        sequences.push_back(sequence);
    }

    return sequences;
}

Graph FlowShopModeler::buildGraph(const FlowShopInstance& inst, const std::vector<std::vector<int>>& machineSequences) {
    validateMachineSequences(inst, machineSequences);

    int totalOperations = inst.numJobs * inst.numMachines;
    int source = totalOperations;
    int sink = totalOperations + 1;

    Graph graph(totalOperations + 2);

    for (int job = 0; job < inst.numJobs; ++job) {
        for (int machine = 0; machine < inst.numMachines; ++machine) {
            int vertex = operationVertex(job, machine, inst.numMachines);
            graph.setVertexWeight(vertex, inst.processingTimes[job][machine]);
        }
    }

    graph.setVertexWeight(source, 0);
    graph.setVertexWeight(sink, 0);

    // cada job passa pelas maquinas na ordem 0, 1, 2, ...
    for (int job = 0; job < inst.numJobs; ++job) {
        for (int machine = 0; machine + 1 < inst.numMachines; ++machine) {
            int from = operationVertex(job, machine, inst.numMachines);
            int to = operationVertex(job, machine + 1, inst.numMachines);
            graph.addEdge(from, to);
        }
    }

    // em cada maquina existe uma ordem propria dos jobs.
    for (int machine = 0; machine < inst.numMachines; ++machine) {
        for (int position = 0; position + 1 < inst.numJobs; ++position) {
            int firstJob = machineSequences[machine][position];
            int secondJob = machineSequences[machine][position + 1];

            int from = operationVertex(firstJob, machine, inst.numMachines);
            int to = operationVertex(secondJob, machine, inst.numMachines);
            graph.addEdge(from, to);
        }
    }

    for (int job = 0; job < inst.numJobs; ++job) {
        graph.addEdge(source, operationVertex(job, 0, inst.numMachines));
        graph.addEdge(operationVertex(job, inst.numMachines - 1, inst.numMachines), sink);
    }

    return graph;
}

FlowShopEvaluation FlowShopModeler::evaluateSequences(
    const FlowShopInstance& inst,
    const std::vector<std::vector<int>>& machineSequences
) {
    Graph graph = buildGraph(inst, machineSequences);
    std::vector<int> topoOrder = GraphAlgorithms::topologicalSort(graph);
    LongestPathResult longestPath = GraphAlgorithms::calculateLongestPath(graph, topoOrder);

    long long flowtime = 0;
    for (int job = 0; job < inst.numJobs; ++job) {
        int lastOperation = operationVertex(job, inst.numMachines - 1, inst.numMachines);
        flowtime += longestPath.distances[lastOperation];
    }

    FlowShopEvaluation evaluation;
    evaluation.makespan = longestPath.maxLength;
    evaluation.flowtime = flowtime;
    evaluation.objective = flowtime + longestPath.maxLength;
    evaluation.machineSequences = machineSequences;

    return evaluation;
}

FlowShopEvaluation FlowShopModeler::improveByAdjacentSwaps(
    const FlowShopInstance& inst,
    const std::vector<std::vector<int>>& initialSequences
) {
    std::vector<std::vector<int>> currentSequences = initialSequences;
    FlowShopEvaluation best = evaluateSequences(inst, currentSequences);

    for (int pass = 0; pass < MAX_LOCAL_SEARCH_PASSES; ++pass) {
        for (int machine = 0; machine < inst.numMachines; ++machine) {
            for (int position = 0; position + 1 < inst.numJobs; ++position) {
                std::swap(currentSequences[machine][position], currentSequences[machine][position + 1]);

                try {
                    FlowShopEvaluation candidate = evaluateSequences(inst, currentSequences);

                    if (candidate.objective < best.objective) {
                        best = candidate;
                    } else {
                        std::swap(currentSequences[machine][position], currentSequences[machine][position + 1]);
                    }
                } catch (const std::runtime_error&) {
                    // se criou ciclo, a ordem testada nao é viável.
                    std::swap(currentSequences[machine][position], currentSequences[machine][position + 1]);
                }
            }
        }
    }

    return best;
}
