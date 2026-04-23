#pragma once
#include <string>
#include <vector>

struct FlowShopInstance {
    int numJobs;
    int numMachines;
    std::vector<std::vector<int>> processingTimes;
};

class InstanceParser {
public:
    static FlowShopInstance parse(const std::string& filepath);
};