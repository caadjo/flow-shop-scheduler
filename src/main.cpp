#include <iostream>
#include <filesystem>
#include <numeric>
#include <vector>
#include <algorithm>
#include <string>
#include "InstanceParser.hpp"
#include "FlowShopModeler.hpp"

namespace fs = std::filesystem;

void printUsage(const char* programName) {
    std::cout << "Usage:\n"
              << "  " << programName << "              # run all instances in data/instances\n"
              << "  " << programName << " ta001        # run one instance by name\n"
              << "  " << programName << " path/to/file # run one instance by path\n";
}

void printSequence(const std::vector<int>& sequence) {
    for (size_t i = 0; i < sequence.size(); ++i) {
        std::cout << sequence[i] + 1;
        if (i + 1 < sequence.size()) {
            std::cout << ' ';
        }
    }
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

void runFlowShopInstance(const std::string& filepath) {
    FlowShopInstance inst = InstanceParser::parse(filepath);

    std::vector<int> naturalSequence(inst.numJobs);
    std::iota(naturalSequence.begin(), naturalSequence.end(), 0);

    FlowShopEvaluation initial = FlowShopModeler::evaluateSequence(inst, naturalSequence);
    FlowShopEvaluation result = FlowShopModeler::improveByAdjacentSwaps(inst, naturalSequence);
    long long improvement = initial.objective - result.objective;

    std::cout << "Instance: " << fs::path(filepath).filename().string()
              << " | InitialObjective: " << initial.objective
              << " | FinalObjective: " << result.objective
              << " | Improvement: " << improvement
              << " | Makespan: " << result.makespan
              << " | Flowtime: " << result.flowtime
              << " | Sequence: ";

    printSequence(result.sequence);
    std::cout << '\n';
}

void runFlowShopInstances() {
    std::vector<std::string> files = listInstanceFiles("data/instances");

    for (const auto& filepath : files) {
        try {
            runFlowShopInstance(filepath);
        } catch (const std::exception& e) {
            std::cerr << "Error processing " << filepath << ": " << e.what() << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        printUsage(argv[0]);
        return 1;
    }

    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        }

        std::string filepath = resolveInstancePath(arg);
        if (filepath.empty()) {
            std::cerr << "Instance not found: " << arg << '\n';
            printUsage(argv[0]);
            return 1;
        }

        try {
            runFlowShopInstance(filepath);
        } catch (const std::exception& e) {
            std::cerr << "Error processing " << filepath << ": " << e.what() << '\n';
            return 1;
        }
        return 0;
    }

    runFlowShopInstances();
    return 0;
}
