// safeOutput.hpp

#pragma once

#include <mutex>
#include <iostream>

namespace Utils {
    extern std::mutex cout_mutex;

    template<typename T>
    void safe_print(const T& msg) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << msg << std::endl;
    }
}