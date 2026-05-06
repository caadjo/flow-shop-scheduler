#include "InstanceParser.hpp"
#include <fstream>
#include <stdexcept>

FlowShopInstance InstanceParser::parse(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Erro ao abrir arquivo");

    FlowShopInstance inst;
    file >> inst.numJobs >> inst.numMachines;
    inst.processingTimes.resize(inst.numJobs, std::vector<int>(inst.numMachines));

    for (int j = 0; j < inst.numJobs; ++j) {
        for (int m = 0; m < inst.numMachines; ++m) {
            int machineId, time;
            file >> machineId >> time;
            inst.processingTimes[j][machineId] = time; // salva o tempo na matriz
        }
    }
    return inst;
}