#include <colors.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <vector>
#include <algorithm>

static const int DAY = 1;
static const std::string NAME = "Calorie Counting";

void printHeader(void) {
    const std::string nameStr = std::to_string(DAY) + " - " + NAME;
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

void task1(std::stringstream &input) {
    std::string buf;
    uint32_t maxElf = 0;
    uint32_t curElf = 0;

    while (std::getline(input, buf)) {
        if (buf.empty()) {
            maxElf = std::max(curElf, maxElf);
            curElf = 0;
        } else {
            curElf += std::stoi(buf);
        }
    }

    std::cout << "Max weight is: " << maxElf << std::endl;
}

void task2(std::stringstream &input) {
    std::string buf;
    uint32_t curElf = 0;
    std::vector<uint32_t> elfs;

    while (std::getline(input, buf)) {
        if (buf.empty()) {
            elfs.push_back(curElf);
            curElf = 0;
        } else {
            curElf += std::stoi(buf);
        }
    }

    std::sort(elfs.begin(), elfs.end(), std::greater<int>());
    uint32_t threeElfs = elfs[0] + elfs[1] + elfs[2];

    std::cout << "Max three weights is: " << threeElfs << std::endl;
}

int main(int, char**) {
    printHeader();

    std::ifstream inputFile("./src/Day " + std::to_string(DAY) + "/input.txt");
    std::stringstream buffer;

    if (inputFile.fail()) {
        std::cout << "No input file!" << std::endl;
        exit(-1);
    }

    buffer << inputFile.rdbuf();

    std::cout << "Task 1" << std::endl;
    task1(buffer);

    buffer.clear();
    buffer.seekg(0);

    std::cout << std::endl << "Task 2" << std::endl;
    task2(buffer);
}