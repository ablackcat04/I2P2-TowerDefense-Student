#include "TimeHelper.hpp"
#include <chrono>
#include <ctime>

char* GetCurrentTime() {
    auto tm = std::chrono::system_clock::now();
    std::time_t tmd = std::chrono::system_clock::to_time_t(tm);

    static char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%b-%d_%H:%M", std::localtime(&tmd));
    return buffer;
}