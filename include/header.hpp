#pragma once

#include <string>
#include <format>
#include "./colors.hpp"

void printHeader(int day, const std::string &name) {
    const std::string nameStr = std::to_string(day) + " - " + name;
    const std::string adventStr = "Advent of Code 2022";
    size_t width = std::max(nameStr.length(), adventStr.length()) + 2;

    const std::string topBotStr = "+" + std::string(width, '-') + "+";
    const std::string fmt = "{:^" + std::to_string(width) + "}";
    const std::string adventLine = std::vformat(fmt, std::make_format_args(adventStr));
    const std::string nameLine = std::vformat(fmt, std::make_format_args(nameStr));

    std::cout << Colors::color(topBotStr + "\n|", Colors::GREEN);
    std::cout << Colors::color(adventLine, Colors::BOLD + Colors::BLUE);
    std::cout << Colors::color("|\n|", Colors::GREEN);
    std::cout << Colors::color(nameLine, Colors::BLUE);
    std::cout << Colors::color("|\n" + topBotStr + "\n\n", Colors::GREEN);
}