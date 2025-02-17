// main.cpp

#include "threadedArbitrageSystem.hpp"
#include "config.hpp"


int main() {
    try {
        auto& config = Config::getInstance();
        if (!config.loadFromFile("../config.json")) {
            std::cerr << "Failed to load configuration" << std::endl;
            return 1;
        }

        ThreadedArbitrageSystem system;
        system.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}