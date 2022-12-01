#pragma once

#include <iostream>

namespace Colors {
    static const std::string RESET   = "\033[0m";
    static const std::string BLACK   = "\033[30m";
    static const std::string RED     = "\033[31m";
    static const std::string GREEN   = "\033[32m";
    static const std::string YELLOW  = "\033[33m";
    static const std::string BLUE    = "\033[34m";
    static const std::string MAGENTA = "\033[35m";
    static const std::string CYAN    = "\033[36m";
    static const std::string WHITE   = "\033[37m";
    static const std::string BOLD    = "\033[1m";

    std::string color(const std::string in, const std::string color) {
        return color + in + RESET;
    }
}
